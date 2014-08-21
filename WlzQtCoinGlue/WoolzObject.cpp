#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _WoolzObject_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         WoolzObject.cpp
* \author       Zsolt Husz, Bill Hill
* \date         October 2008
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
* \brief        Container class for WlzObject type
* \ingroup      Control
*
*/

#include "WoolzObject.h"

//inventor includes
#include <Inventor/nodes/SoGroup.h>

//Qt onlcudes
#include <QString>
#include <QFileInfo>

#include "TransferFunction.h"
#include "BinaryTransferFunction.h"

#include <QtXml/QXmlStreamWriter>

#include <QtXml/QDomElement>


unsigned WoolzObject::colourCounter = 2;  // skip black and white;

// Constructors/Destructors
//
WoolzObject::
WoolzObject():
QObject()
{
  m_obj = NULL;
  m_removable = true;
  m_type = none;
  m_cachedVisualisation = NULL;
  m_validVisualisation = false;
  m_cachedVisualisationScale = 0;
  m_visible = true;
  m_ID = -1;
  m_transferFunction = NULL;
}

WoolzObject::
~WoolzObject()
{
  close();
  if(m_transferFunction)
  {
    m_transferFunction->unref();
    m_transferFunction = NULL;
  }
}

// Other methods
//
void WoolzObject::
close()
{
  removeCachedVisualisation();
  if(m_obj)
  {
    emit objectRemoved();
    WlzFreeObj(m_obj);
    m_obj = NULL;
  }
}

bool WoolzObject::
isFile()
{
  return(false);
}

bool WoolzObject::
isMesh()
{
  return((m_obj && ((m_obj->type == WLZ_CMESH_2D) ||
                    (m_obj->type == WLZ_CMESH_3D))));
}

bool WoolzObject::
isValue()
{
  return((m_obj && ((m_obj->type == WLZ_3D_DOMAINOBJ) ||
                    (m_obj->type == WLZ_2D_DOMAINOBJ))));
}

bool WoolzObject::
isContour()
{
  bool is = false;

  if(m_obj && (m_obj->type == WLZ_CONTOUR))
  {
    WlzContour *ctr;

    ctr = m_obj->domain.ctr;
    if(ctr)
    {
      is = (ctr->model->type ==  WLZ_GMMOD_2I) ||
	   (ctr->model->type ==  WLZ_GMMOD_2N) ||
	   (ctr->model->type ==  WLZ_GMMOD_2D) ||
	   (ctr->model->type ==  WLZ_GMMOD_3I) ||
	   (ctr->model->type ==  WLZ_GMMOD_3N) ||
	   (ctr->model->type ==  WLZ_GMMOD_3D);
    }
  }
  return(is);
}

bool WoolzObject::
isConvHull()
{
  bool is = false;

  if(m_obj && (m_obj->type == WLZ_CONV_HULL))
  {
    WlzDomain dom;

    dom = m_obj->domain;
    if(dom.core)
    {
      is = (dom.core->type ==  WLZ_CONVHULL_DOMAIN_2D) ||
	   (dom.core->type ==  WLZ_CONVHULL_DOMAIN_3D);
    }
  }
  return(is);
}

bool WoolzObject::
isValueSet()
{
  return(m_obj && ((m_obj->type == WLZ_3D_DOMAINOBJ) ||
                   (m_obj->type == WLZ_2D_DOMAINOBJ)) &&
	  m_obj->values.core);
}


bool WoolzObject::
isColour()
{
  return(getWoolzGreyType() == WLZ_GREY_RGBA);
}

WlzGreyType WoolzObject::
getWoolzGreyType()
{
  if(!isValueSet())
  {
    return WLZ_GREY_ERROR;
  }

  WlzGreyType   gType;
  WlzErrorNum errNum = WLZ_ERR_NONE;

  gType = WlzGreyTypeFromObj(m_obj, &errNum);
  Q_ASSERT(errNum == WLZ_ERR_NONE);
  {
    return gType;
  }
}

bool WoolzObject::
is2D()
{
  bool is = false;

  if(m_obj)
  {
    WlzDomain dom;

    dom = m_obj->domain;
    switch(m_obj->type)
    {
      case WLZ_2D_DOMAINOBJ:
	is = true;
	break;
      case WLZ_CONTOUR:
	is = dom.core && dom.ctr->model &&
	     ((dom.ctr->model->type == WLZ_GMMOD_2I) ||
	      (dom.ctr->model->type == WLZ_GMMOD_2D) ||
	      (dom.ctr->model->type == WLZ_GMMOD_2N));
	break;
      case WLZ_CONV_HULL:
	is = dom.core && (dom.core->type == WLZ_CONVHULL_DOMAIN_2D);
	break;
      default:
	break;
    }
  }
  return(is);
}

bool WoolzObject::
is3D()
{
  bool is = false;

  if(m_obj)
  {
    WlzDomain dom;

    dom = m_obj->domain;
    switch(m_obj->type)
    {
      case WLZ_3D_DOMAINOBJ:
        is = true;
	break;
      case WLZ_CONTOUR:
	is = dom.core && dom.ctr->model &&
	     ((dom.ctr->model->type == WLZ_GMMOD_3I) ||
	      (dom.ctr->model->type == WLZ_GMMOD_3D) ||
	      (dom.ctr->model->type == WLZ_GMMOD_3N));
        break;
      case WLZ_CONV_HULL:
	is = dom.core && (dom.core->type == WLZ_CONVHULL_DOMAIN_3D);
        break;
      default:
        break;
    }
  }
  return(is);
}

SbColor WoolzObject::
sbColour()
{
  int r,g,b;
  m_colour.getRgb(&r, &g, &b);
  return(SbColor((float)r/255, (float)g/255, (float)b/255));
}

QColor WoolzObject::
qColour()
{
  return(m_colour);
}

void WoolzObject::
setQColour(
  const QColor newColour)
{
  if(m_colour != newColour)
  {
    m_colour = newColour;
    emit objectVisualisationChange();
    emit objectPropertyChanged();
  }
}

bool WoolzObject::
saveAs(QString filename)
{
  if(!m_obj)
  {
    return false;
  }

  QString ext = QFileInfo(filename).suffix().toUpper();
  if(ext.isEmpty())
  {
    filename += ".wlz";
  };

  WlzErrorNum errNum = WLZ_ERR_NONE;
  FILE *fp;
  fp=fopen(filename.toAscii(),"wb");

  if(!fp)
  {
    return false;
  }
  errNum = WlzWriteObj( fp, m_obj);

  if(fp)
  {
    fclose(fp);
  }
  return(errNum == WLZ_ERR_NONE);
}

void WoolzObject::
removeCachedVisualisation()
{
  m_validVisualisation = false;
  if(m_cachedVisualisation)
  {
    m_cachedVisualisation->unref();
    m_cachedVisualisation = NULL;
    m_cachedVisualisationScale = 0;
  }
}

void WoolzObject::
setVisualisation(
  SoGroup * newVis,
  int newScale)
{
  removeCachedVisualisation();
  m_cachedVisualisationScale = 0;
  m_validVisualisation = newVis != NULL;
  if(m_validVisualisation)
  {
    m_cachedVisualisation = newVis;
    m_cachedVisualisation->ref();
    m_cachedVisualisationScale = newScale;
  }
}

void WoolzObject::
changeWoolzObjectType(WoolzObjectType type)
{
  if(type != m_type)
  {
    m_type = type;
    emit objectTypeChanged();
  }
}

QColor WoolzObject::
getNextColour()
{
  QColor color;
  do 
  {
    color = QColor(QColor::colorNames().at(colourCounter++));
  } while ((qGray(color.rgb()) == 0) ||
           (qGray(color.rgb())== 255)); //  while color is not black nor white
  return(color);
}

void WoolzObject::
generateNewColour()
{
  if(isValue())
  {
    if(isValueSet())
    {
      setQColour(QColor(255,255,255));  //use white colour
    }
    else
    {
      setQColour(QColor(127,127,127));  //use grey colour
    }
  }
  else
  {
    setQColour(getNextColour());  //new colour
  }
}

void WoolzObject::setVisible( bool visible ) {
  m_visible = visible;
  emit objectVisibilityChanged();
}

void WoolzObject::
setupTransferFunction(
  bool force)
{
  if(m_obj && !m_transferFunction &&
     (force ||(is3D() && isValue())))
  {
    m_transferFunction = (isValueSet())?
                         new TransferFunction : new BinaryTransferFunction;
    m_transferFunction->ref();
    connect(m_transferFunction, SIGNAL(updated()),
            this, SIGNAL(objectPropertyChanged()));
  }
}

void WoolzObject::
replaceTransferFunction(
  TransferFunction *tf)
{
  if(m_transferFunction)
  {
    m_transferFunction->unref();
    disconnect(m_transferFunction, SIGNAL(updated()),
               this, SIGNAL(objectPropertyChanged()));
  }
  m_transferFunction = tf;
  if(tf)
  {
    m_transferFunction->ref();
    connect(m_transferFunction, SIGNAL(updated()),
            this, SIGNAL(objectPropertyChanged()));
  }
}

void WoolzObject::
setupConnections(
  QObject *target)
{
  connect(this, SIGNAL(objectPropertyChanged()),
          target, SLOT(objectChanged()));
  connect(this, SIGNAL(objectVisibilityChanged()),
          target, SLOT(objectChanged()));
  connect(this, SIGNAL(objectTypeChanged()),
          target, SLOT(objectTypeChanged()));
  connect(this, SIGNAL(objectSelected()),
          target, SLOT(selectAnObject()));
  connect(this, SIGNAL(statusChange(QString, int)),
          target, SIGNAL(statusChanged(QString, int)));
  connect(target, SIGNAL(updateAllSignal(bool)),
          this, SLOT(update(bool)));
}


bool WoolzObject::
saveAsXmlProperties(
  QXmlStreamWriter *xmlWriter)
{
  xmlWriter->writeTextElement("Name", m_name);
  xmlWriter->writeTextElement("Type", type() == source ? "source":"target");
  xmlWriter->writeTextElement("Color",
      QString("%1,%2,%3").arg(m_colour.red()).
                          arg(m_colour.green()).
                          arg(m_colour.blue()));
  xmlWriter->writeTextElement("Removable", m_removable ? "Yes" : "No");
  xmlWriter->writeTextElement("Visible", m_visible ? "Yes" : "No");
  xmlWriter->writeTextElement("ID", QString("%1").arg(m_ID));
  if(m_transferFunction)
  {
    m_transferFunction->saveAsXml(xmlWriter);
  }
  return(true);
}


bool WoolzObject::
parseDOMLine(
  const QDomElement &element)
{
  bool st = true;

  if(element.tagName() == "Name")
  {
    m_name = element.text();
  }
  else if(element.tagName() == "Type")
  {
    QString str = element.text().toUpper();
    if(str == "SOURCE")
    {
      m_type = source;
    }
    else if (str == "TARGET")
    {
      m_type = target;
    }
  }
  else if(element.tagName() == "Color")
  {
    QString str = element.text();

    int c1 = str.indexOf(",");
    int c2 = str.indexOf(",", c1+1);
    int r = str.left(c1).toInt();
    int g = str.mid(c1+1,c2-c1-1).toInt();
    int b = str.right(str.size()-c2-1).toInt();

    if(c1>=0 && c2>c1)
    {
      m_colour = QColor(r,g,b);
    }
  }
  else if(element.tagName() == "Removable")
  {
    QString str = element.text().toUpper();
    m_removable = str == "YES";
    st = false;
  }
  else if(element.tagName() == "Visible")
  {
    QString str = element.text().toUpper();
    m_visible = str == "YES";
  }
  else if((element.tagName() == BinaryTransferFunction::xmlTag) ||
          (element.tagName() == TransferFunction::xmlTag))
  {
    TransferFunction *tf =
        (element.tagName() == BinaryTransferFunction::xmlTag)?
	new BinaryTransferFunction: new TransferFunction;
    Q_ASSERT(tf);
    replaceTransferFunction(tf);
    st = tf->parseDOM(element);
  }
  else if(element.tagName() == "ID")
  {
    m_ID = element.text().toInt();
  }
  return(st);
}

bool WoolzObject::
parseDOM(
  const QDomElement &element)
{
  QDomNode child = element.firstChild();
  while(!child.isNull())
  {
    parseDOMLine(child.toElement());
    child = child.nextSibling();
  }
  return(true);
}

void WoolzObject::
removeMeshElement(const WlzDVertex3 point)
{
  if(isMesh() && is3D())
  {
    const int dd=2;
    const float dist = 1.0f;
    for(int x = -dd; x <= dd; ++x)
    {
      for(int y = -dd; y < dd; ++y)
      {
	for(int z = -dd; z < dd; ++z)
	{
	  WlzCMesh3D *mesh = m_obj->domain.cm3;
	  int idE = WlzCMeshElmEnclosingPos3D(mesh, 0,
	                                      point.vtX + x * dist,
					      point.vtY + y * dist,
					      point.vtZ + z * dist, 0, NULL);
	  if(idE>0)
	  {
	    WlzCMeshElm3D *elm = (WlzCMeshElm3D *)
	    			 AlcVectorItemGet(mesh->res.elm.vec, idE);
	    if(elm)
	    {
	      statusChange("Element to be removed", 5);
	      removeMeshElement(idE); // must be before mesh is changed
	      WlzCMeshDelElm3D(mesh, elm);
	      statusChange("Element removed", 5);
	    }
	  }
	}
      }
    }
  }
}

