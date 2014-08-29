#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _WoolzDynContourISO_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         WoolzDynContourISO.cpp
* \author       Zsolt Husz
* \date         June 2009
* \version      $Id$
* \par
* Address:
*               MRC Human Genetics Unit,
*               MRC Institute of Genetics and Molecular Medicine,
*               University of Edinburgh,
*               Western General Hospital,
*               Edinburgh, EH4 2XU, UK.
* \par
* Copyright (C), [2014],
* The University Court of the University of Edinburgh,
* Old College, Edinburgh, UK.
* 
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be
* useful but WITHOUT ANY WARRANTY; without even the implied
* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
* PURPOSE.  See the GNU General Public License for more
* details.
*
* You should have received a copy of the GNU General Public
* License along with this program; if not, write to the Free
* Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
* Boston, MA  02110-1301, USA.
* \brief        Dynamicaly generated contour and ISO Woolz objects
* \ingroup      Control
*/

#include "WoolzDynContourISO.h"
#include "ObjectListModel.h"
#include "WarperConfig.h"

#include <QtXml/QDomElement>
#include <QtXml/QXmlStreamWriter>

const char* WoolzDynContourISO::xmlTag = "CountourObject";

/*!
* \brief	Stores shell inforamtion
* \ingroup      Control
*/
class ShellElement
{
  public:
    int indx;
    int cnt;
    WlzGMShell    *cS;
    bool operator< (const ShellElement & elem) const
    {
      return cnt > elem.cnt;
    }
    ShellElement(int i, int c, WlzGMShell  *s): indx(i), cnt(c), cS(s) {}
} ;

WoolzDynContourISO::
WoolzDynContourISO(
  ObjectListModel * objectListModel):
WoolzDynObject(
  objectListModel),
m_value(255),
m_filter(false),
m_boundary(true),
m_subsample(4),
m_selection(0xFFFF),
m_shellNo(0),
m_fullContourObject(NULL),
max_no_checkboxes(15),
m_valueUsed(0),
m_filterUsed(true),
m_boundaryUsed(false),
m_subsampleUsed(4),
m_selectionUsed(0xFFFF)
{
}

WoolzDynContourISO::
~WoolzDynContourISO()
{
  if(m_fullContourObject)
  {
    WlzFreeObj(m_fullContourObject);
  }
}

void WoolzDynContourISO::
doUpdate()
{
  recomputeFullContour();
  recomputeShells_intern();  // computes the real m_obj
  saveUsedParameters();
  emit objectChanged();
  return;
}

void WoolzDynContourISO::
recomputeShells()
{
  if((m_autoUpdate && config.globalAutoUpdate()))
  {
    removeCachedVisualisation();
    recomputeShells_intern();
    emit objectChanged();
  }
}

void WoolzDynContourISO::
recomputeShells_intern()
{
  if((m_fullContourObject == NULL) ||
     (m_fullContourObject->domain.ctr == NULL) ||
     (m_fullContourObject->domain.ctr->model == NULL))
  {
    return;
  }
  statusChange("Contour " + m_name + " selecting shells.", 0);
  if(m_obj)
  {
    WlzFreeObj(m_obj);
    m_obj = NULL;
  }
  WlzErrorNum   errNum = WLZ_ERR_NONE;
  /* 1) if m_fullContourObject is empty, it can't be copied and there is
   *    need shells exists so non can be removed.
   * 2) if all shells are selected, then use a refference of the original
   * object
   */
  if((m_shellNo == 0) || (m_selection == 0xFFFF))
  {
    m_obj  = WlzAssignObject(m_fullContourObject, NULL);
    statusChange("Contour " + m_name + " finished.", 0);
    return;
  }

  m_obj = WlzCopyObject(m_fullContourObject, &errNum);
  if(errNum != WLZ_ERR_NONE)
  {
    WlzFreeObj(m_obj);
    m_obj  = NULL;
    statusChange("Contour " + m_name + " shell failed.", 0);
    return;
  }

  Q_ASSERT(m_obj);
  m_obj  = WlzAssignObject(m_obj , NULL);
  WlzGMModel *model = m_obj->domain.ctr->model;
  if(model == NULL)
  {
    return;
  }

  WlzGMShell    *cS,
                *fS,
                *nS;
  int i;
  QList <ShellElement> list;

  if((fS = model->child) == NULL)
  {
    // no shells. this should not be the case, we already checked
    return ;
  }
  nS = fS->next;
  do
  {
    cS = nS;
    nS = nS->next;
    list << ShellElement(cS->idx, WlzGMShellSimplexCnt(cS), cS);
  }
  while(cS != fS);

  Q_ASSERT(m_shellNo == list.size());
  qSort(list.begin(), list.end());
  int length1 = m_shellNo;
  int selection = m_selection;
  if(length1 > max_no_checkboxes)
  {
    length1 = max_no_checkboxes;
  }
  for(i = 0; i < length1; i++)
  {
    if((selection & 1) == 0)
      WlzGMModelDeleteS(model, list.at(i).cS);
    selection = selection >> 1;
  }

  // check and remove rest
  if(selection == 0)
  {
    for(i = max_no_checkboxes ; i < m_shellNo; i++)
    {
      WlzGMModelDeleteS(model, list.at(i).cS);
    }
  }
  statusChange("Contour " + m_name + " shells selected.", 0);
}


void WoolzDynContourISO::
recomputeFullContour()
{
  WoolzObject *srcObj = sourceObj();
  if(!srcObj || !(srcObj->getObj()) || !(srcObj->isValueSet()))
  {
    return;
  }

  statusChange("Contour " + m_name + " computing.", 0);

  WlzErrorNum	errNum = WLZ_ERR_NONE;
  if(m_fullContourObject)
  {
    WlzFreeObj(m_fullContourObject);
  }

  WlzObject * tempObj = NULL;
  if(m_subsample != 1)
  {
    WlzAffineTransform *transform =
      WlzAffineTransformFromScale((srcObj->is3D())?
	  WLZ_TRANSFORM_3D_AFFINE: WLZ_TRANSFORM_2D_AFFINE,
	  1.0 / m_subsample, 1.0 / m_subsample, 1.0 / m_subsample, &errNum);
    if(errNum == WLZ_ERR_NONE)
    {
      tempObj = WlzAssignObject(
	        WlzAffineTransformObj(srcObj->getObj(), transform,
		                      WLZ_INTERPOLATION_NEAREST,
				      &errNum), NULL);
      WlzFreeAffineTransform(transform);
    }
  }
  else
  {
    tempObj = WlzAssignObject(srcObj->getObj(), NULL);
  }

  WlzDomain     dom;
  WlzValues     val;
  val.core = NULL;
  if(errNum == WLZ_ERR_NONE)
  {
    if(m_boundary)
    {
      dom.ctr = WlzContourObj(tempObj, WLZ_CONTOUR_MTD_BND, 0, 1, 0, &errNum);
    }
    else
    {
      dom.ctr = WlzContourObj(tempObj, WLZ_CONTOUR_MTD_ISO, m_value, 1, 0,
	                      &errNum);
    }
  }

  // do filtering of 3D surface is requested
  if(m_filter && srcObj->is3D())
  {
    double        lambda,
                  mu,
                  filterPB = 0.1,
                  filterSB = 1.1;
    const double  filterDPB = 0.25,
                  filterDSB = 0.10;

    int           nonMan = 0,
                  nItr = 10;

    errNum = WlzGMFilterGeomLPParam(&lambda, &mu, &nItr,
                                    filterPB, filterSB, filterDPB, filterDSB);
    if(errNum == WLZ_ERR_NONE)
    {
      errNum = WlzGMFilterGeomLPLM(dom.ctr->model, lambda, mu, nItr, nonMan);
    }
  }

  if(errNum == WLZ_ERR_NONE && m_subsample != 1)
  {
    WlzAffineTransform *transform =
      WlzAffineTransformFromScale(
	  (srcObj->is3D())?  WLZ_TRANSFORM_3D_AFFINE: WLZ_TRANSFORM_2D_AFFINE,
	  m_subsample, m_subsample, m_subsample, &errNum);
    if(errNum == WLZ_ERR_NONE)
    {
      dom.ctr = WlzAffineTransformContour(dom.ctr, transform, 0, &errNum);
      WlzFreeAffineTransform(transform);
    }
  }

  if(errNum == WLZ_ERR_NONE)
    m_fullContourObject = WlzMakeMain(WLZ_CONTOUR, dom, val, NULL, NULL,
	                              &errNum);

  WlzFreeObj(tempObj);
  if(errNum != WLZ_ERR_NONE)
  {
    if(m_fullContourObject)
      WlzFreeObj(m_fullContourObject);
    m_fullContourObject = NULL;
    m_type = WoolzObject::none;
  }
  else
  {
    m_fullContourObject = WlzAssignObject(m_fullContourObject, NULL);
    m_type = srcObj->type();
  }

  WlzGMModel *model = m_fullContourObject->domain.ctr->model;

  if(model == NULL)
  {
    statusChange("Contour " + m_name + " failed.", 0);
    return;
  }
  //count number of shells
  WlzGMShell    *fS, *nS;
  m_shellNo = 0;
  if((fS = model->child) != NULL)
  {
    nS = fS;
    do
    {
      nS = nS->next;
      m_shellNo++;
    }
    while(nS != fS);
  }
  statusChange("Contour " + m_name + " finished.", 0);
  emit objectPropertyChanged();
}

bool WoolzDynContourISO::
saveAsXml(
  QXmlStreamWriter *xmlWriter)
{
  xmlWriter->writeStartElement(xmlTag);
  saveAsXmlProperties(xmlWriter);
  xmlWriter->writeEndElement();
  return(true);
}

bool WoolzDynContourISO::
saveAsXmlProperties(
  QXmlStreamWriter *xmlWriter)
{
  WoolzDynObject::saveAsXmlProperties(xmlWriter);
  xmlWriter->writeTextElement("Boundary", m_boundary ? "Yes" : "No");
  if(!m_boundary)
    xmlWriter->writeTextElement("Value", QString("%1").arg(m_value));
  xmlWriter->writeTextElement("Filter", m_filter ? "Yes" : "No");
  xmlWriter->writeTextElement("Subsample", QString("%1").arg(m_subsample));
  xmlWriter->writeTextElement("Shells", QString("%1").arg(m_selection));
  xmlWriter->writeTextElement("ShellNo", QString("%1").arg(m_shellNo));
  return(true);
}

bool WoolzDynContourISO::
parseDOMLine(
  const QDomElement &element)
{
  if(element.tagName() == "Boundary")
  {
    m_boundary = element.text().toUpper() == "YES";
    return(true);
  }
  else if(element.tagName() == "Filter")
  {
    m_filter = element.text().toUpper() == "YES";
    return(true);
  }
  else if(element.tagName() == "Value")
  {
    m_value = element.text().toInt();
    return(true);
  }
  else if(element.tagName() == "Subsample")
  {
    m_subsample = element.text().toInt();
    return(true);
  }
  else if(element.tagName() == "Shells")
  {
    m_selection = element.text().toInt();
    return(true);
  }
  else if(element.tagName() == "ShellNo")
  {
    m_shellNo = element.text().toInt();
    return(true);
  }
  else
  {
    return(WoolzDynObject::parseDOMLine(element));
  }
  return(false);
}

bool  WoolzDynContourISO::needsUpdate()
{
  return((m_value != m_valueUsed) ||
         (m_filter != m_filterUsed) ||
	 (m_boundary  != m_boundaryUsed) ||
         (m_subsample != m_subsampleUsed) ||
	 (m_selection != m_selectionUsed) ||
	 WoolzDynObject::needsUpdate());
}

void WoolzDynContourISO::
saveUsedParameters()
{
  m_valueUsed      = m_value;
  m_filterUsed     = m_filter;
  m_boundaryUsed   = m_boundary;
  m_subsampleUsed  = m_subsample;
  m_selectionUsed  = m_selection;

  WoolzDynObject::saveUsedParameters();
}

void WoolzDynContourISO::
setupConnections(
    QObject *target)
{
  WoolzDynObject::setupConnections(target);
  connect(target, SIGNAL(loadAllSignal()), this, SLOT(update()));
}
