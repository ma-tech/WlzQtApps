#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _LandmarkModel_cpp[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         LandmarkModel.cpp
* \author       Zsolt Husz
* \date         October 2008
* \version      $Id$
* \par
* Address:
*               MRC Human Genetics Unit,
*               Western General Hospital,
*               Edinburgh, EH4 2XU, UK.
* \par
* Copyright (C) 2008 Medical research Council, UK.
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
* \brief        Model of landmarks.
* \ingroup      Control
*
*/

//Qt includes
#include <QAction>
#include <QMessageBox>
#include <QTextStream>
#include <QUndoStack>
#include <QtXml/QXmlStreamWriter>
#include <QtXml/QDomElement>
#include <QFile>

//project includes
#include "LandmarkModel.h"
#include "LandmarkView.h"
#include "WoolzObject.h"

#define COIN2DOUBLE_EPLSILON 1e-4

#ifdef DEBUG_LANDMARK_DUMP
// for debug only
#include <QFile>
#include <QDate>
#include <QTime>

#endif

const char* LandmarkModel::xmlTag = "LandmarkSet";
const char* LandmarkModel::xmlTagWarping = "Warping";

LandmarkModel::LandmarkModel (QObject * parent):  QAbstractItemModel (parent)  {
  is3D = true;
  basisTr = NULL;
  m_mesh = NULL;
  m_delta = 0.02;
  m_snapToFitDist = 16.0;
  m_basisFnType = basis_IMQ;
}

int LandmarkModel::rowCount(const QModelIndex & parent) const {
    if (!parent.isValid()) //this is the root node
      return listPointPair.size();
    else 
      return 0;
}

int LandmarkModel::columnCount(const QModelIndex & /*parent*/) const {
    return is3D ? 7 : 5;
}

QVariant LandmarkModel::data(const QModelIndex & index, int role ) const {
  if (!index.isValid())
    return QVariant();
  if (role == Qt::DisplayRole) {
    PointPair *pp = listPointPair.at(index.row());
    Q_ASSERT(pp);
    if (is3D) {
      switch (index.column()) {
          case 0:
            return index.row();
          case 1:
            return QString("%1").arg(pp->V[sourceV].vtX,  0, 'f', 2);
          case 2:
            return QString("%1").arg(pp->V[sourceV].vtY,  0, 'f', 2);
          case 3:
            return QString("%1").arg(pp->V[sourceV].vtZ,  0, 'f', 2);
          case 4:
            return QString("%1").arg(pp->V[targetV].vtX,  0, 'f', 2);
          case 5:
            return QString("%1").arg(pp->V[targetV].vtY,  0, 'f', 2);
          case 6:
            return QString("%1").arg(pp->V[targetV].vtZ,  0, 'f', 2);
      } //switch
    } else {
      switch (index.column()) {
          case 0:
            return index.row();
          case 1:
            return QString("%1").arg(pp->V[sourceV].vtX,  0, 'f', 2);
          case 2:
            return QString("%1").arg(pp->V[sourceV].vtY,  0, 'f', 2);
          case 3:
            return QString("%1").arg(pp->V[targetV].vtX,  0, 'f', 2);
          case 4:
            return QString("%1").arg(pp->V[targetV].vtY,  0, 'f', 2);
      } //switch
    }
  } else if (role == Qt::TextColorRole) {
      PointPair *pp = listPointPair.at(index.row());
      Q_ASSERT(pp);
      if (!isDraggerValid(sourceV, pp) ||
          !isDraggerValid(targetV, pp))
          return Qt::red;
      else
          return Qt::black;
  } else if (role == Qt::EditRole) {
    PointPair *pp = listPointPair.at(index.row());
    Q_ASSERT(pp);
    if (is3D) {
      switch (index.column()) {
          case 1:
            return pp->V[sourceV].vtX;
          case 2:
            return pp->V[sourceV].vtY;
          case 3:
            return pp->V[sourceV].vtZ;
          case 4:
            return pp->V[targetV].vtX;
          case 5:
            return pp->V[targetV].vtY;
          case 6:
            return pp->V[targetV].vtZ;
      } //switch
    } else {
      switch (index.column()) {
          case 0:
            return index.row();
          case 1:
            return pp->V[sourceV].vtX;
          case 2:
            return pp->V[sourceV].vtY;
          case 3:
            return pp->V[targetV].vtX;
          case 4:
            return pp->V[targetV].vtY;
      } //switch
    } //role
  }
  return QVariant();
}

QVariant LandmarkModel::headerData(int section, Qt::Orientation orientation, int role ) const {
  if (role == Qt::DisplayRole && orientation == Qt::Horizontal) {
     if (is3D)
       switch (section) {
        case 0:
          return QString(tr("No"));
        case 1:
          return QString(tr("Source X"));
        case 2:
          return QString(tr("Source Y"));
        case 3:
          return QString(tr("Source Z"));
        case 4:
          return QString(tr("Target X"));
        case 5:
          return QString(tr("Target Y"));
        case 6:
          return QString(tr("Target Z"));
    } else 
      switch (section) {
        case 0:
          return QString(tr("Number"));
        case 1:
          return QString(tr("Source X"));
        case 2:
          return QString(tr("Source Y"));
        case 3:
          return QString(tr("Target X"));
        case 4:
          return QString(tr("Target Y"));
    }
  }
  return QVariant();
}

LandmarkModel::~LandmarkModel ( ) { 
   WlzBasisFnFreeTransform(basisTr);
   listPointPair.clear();
}

Qt::ItemFlags LandmarkModel::flags(const QModelIndex &index) const {
  Qt::ItemFlags flags = QAbstractItemModel::flags(index);
  if (index.column()>0 )
      flags |= Qt::ItemIsEditable;
  return flags;
}

bool LandmarkModel::setData(const QModelIndex &index, const QVariant &value, int role) {
  if (!index.isValid())
      return false;

  if (role == Qt::EditRole) {
   double dval = value.toDouble();
   int row=index.row();
   PointPair *pp = listPointPair.at(row);
   if (pp) {
     if (is3D)
       switch (index.column()) {
        case 1:
          pp->V[0].vtX = dval;
          emit movedSourceLandmark(row, pp->V[0]);
          break;
        case 2:
          pp->V[0].vtY = dval;
          emit movedSourceLandmark(row, pp->V[0]);
          break;
        case 3:
          pp->V[0].vtZ = dval;
          emit movedSourceLandmark(row, pp->V[0]);
          break;
        case 4:
          pp->V[1].vtX = dval;
          emit movedTargetLandmark(row, pp->V[1]);
          break;
        case 5:
          pp->V[1].vtY = dval;
          emit movedTargetLandmark(row, pp->V[1]);
          break;
        case 6:
          pp->V[1].vtZ = dval;
          emit movedTargetLandmark(row, pp->V[1]);
          break;
      } else switch (index.column()) {
        case 1:
          pp->V[0].vtX = dval;
          emit movedSourceLandmark(row, pp->V[0]);
          break;
        case 2:
          pp->V[0].vtY = dval;
          emit movedSourceLandmark(row, pp->V[0]);
          break;
        case 3:
          pp->V[1].vtX = dval;
          emit movedTargetLandmark(row, pp->V[1]);
          break;
        case 4:
          pp->V[1].vtY = dval;
          emit movedTargetLandmark(row, pp->V[1]);
          break;
      }
    }
  }
  return false;
}


QModelIndex LandmarkModel::index(int row, int column, const QModelIndex &parent ) const  {
  if (row<0 || column <0)
    return QModelIndex();
  if (!parent.isValid()) {//type
      return createIndex(row, column, (void*)0);
  }
  return QModelIndex();
}

QModelIndex LandmarkModel::parent(const QModelIndex &/*child*/) const {
  return QModelIndex();
}

void LandmarkModel::insertLandmarkPair(int row, PointPair &pp){
  if (row==-1)
      row = listPointPair.size();
  listPointPair.insert(row, new PointPair(pp));
  beginInsertRows(QModelIndex(), row, row);
  endInsertRows();

  emit addedLandmarkPair(row);
}

void LandmarkModel::removeLast(){
  int index=listPointPair.size()-1;

  listPointPair.removeAt(index);

  #ifdef DEBUG_LANDMARK_DUMP
  writeLandmarkDebug(DEBUG_LANDMARK_DUMP);
  #endif

  emit removedLandmark(index);
  beginRemoveRows(QModelIndex(), index, index );
  endRemoveRows();
}

PointPair* LandmarkModel::getPointPair(int index) {
  Q_ASSERT( index >= 0 && index<listPointPair.size());
  return listPointPair.at(index);
}

SbVec3f LandmarkModel::getHalfPointPair(int index, IndexType indexType) {
  PointPair *pp = getPointPair(index);
  SbVec3f p;
  p[0]=(*pp)[indexType].vtX;
  p[1]=(*pp)[indexType].vtY;
  p[2]=(*pp)[indexType].vtZ;
  return p;
}




void LandmarkModel::removeLandmark(int index) {
  Q_ASSERT(index>=0 && index<listPointPair.size());
  listPointPair.removeAt(index);

  #ifdef DEBUG_LANDMARK_DUMP
  writeLandmarkDebug(DEBUG_LANDMARK_DUMP);
  #endif

  emit removedLandmark(index);
  beginRemoveRows(QModelIndex(), index, index ); 
  endRemoveRows();
}

WlzDVertex2* LandmarkModel::extractVertex2D(IndexType indexType, int& n){
  n= listPointPair.size();
  if (n == 0) 
    return NULL;
  int i;
  WlzDVertex2 * vertex = new WlzDVertex2[n];
  for (i=0; i<n; i++) {
    PointPair *pp = listPointPair.at(i);
    vertex[i].vtX=(*pp)[indexType].vtX;
    vertex[i].vtY=(*pp)[indexType].vtY;
  }
  return vertex;
}

WlzDVertex3* LandmarkModel::extractVertex3D(IndexType indexType, int& n){
  n= listPointPair.size();
  if (n == 0) 
    return NULL;
  int i;
  WlzDVertex3 * vertex = new WlzDVertex3[n];
  for (i=0; i<n; i++) {
    PointPair *pp = listPointPair.at(i);
    vertex[i].vtX=(*pp)[indexType].vtX;
    vertex[i].vtY=(*pp)[indexType].vtY;
    vertex[i].vtZ=(*pp)[indexType].vtZ;
  }
  return vertex;
}

void LandmarkModel::move(int index, PointPair newPP,
                         LandmarkModel::IndexType indexType) {
 move(index, newPP[indexType], indexType);
}

void LandmarkModel::move(int index, const SbVec3f newPosition,
                         LandmarkModel::IndexType indexType) {
 WlzDVertex3 p;
 p.vtX=newPosition[0];
 p.vtY=newPosition[1];
 p.vtZ=newPosition[2];
 move(index, p, indexType);
}

void LandmarkModel::move(int index, const WlzDVertex3 newPosition,
                         LandmarkModel::IndexType indexType) {
  WlzDVertex3 p3 = newPosition;
  PointPair *pp=listPointPair.at(index);
  Q_ASSERT(pp);
  if(m_snapToFitDist > WLZ_MESH_TOLERANCE) {
    if(is3D) {
      WlzCMesh3D *mesh = m_mesh->getObj()->domain.cm3;
      (void )WlzCMeshElmClosestPosIn3D(mesh, &p3, p3, m_snapToFitDist);
    } else {
      WlzDVertex2 p2;
      WlzCMesh2D *mesh = m_mesh->getObj()->domain.cm2;
      p2.vtX = p3.vtX;
      p2.vtY = p3.vtY;
      if(WlzCMeshElmClosestPosIn2D(mesh, &p2, p2, m_snapToFitDist) >= 0) {
	p3.vtX = p2.vtX;
	p3.vtY = p2.vtY;
      }
    }
  }
  (*pp)[indexType] = p3;

  #ifdef DEBUG_LANDMARK_DUMP
  writeLandmarkDebug(DEBUG_LANDMARK_DUMP);
  #endif

  const int element = is3D ? 3 : 2;
  changePersistentIndex(createIndex(index,
                                    (index == sourceV)? 1 : (1 + element), 0),
                        createIndex(index,
			            (index == sourceV)? element : (2 * element),
				    0));  //force update
  emit layoutChanged();
  if (indexType == sourceV)
     emit movedSourceLandmark(index, pp->V[sourceV]);
  else
     emit movedTargetLandmark(index, pp->V[targetV]);

  bool b = isDraggerValid(indexType, pp);
  if (indexType == sourceV)
     emit setSourceLandmarkValid(index, b);
  else
     emit setTargetLandmarkValid(index, b);
}

bool LandmarkModel::saveAsXml(QXmlStreamWriter *xmlWriter) {
  int i;
  PointPair *pp;
  QString string;

  xmlWriter->writeStartElement(xmlTag);
  xmlWriter->writeTextElement("Type", is3D ? "3D" : "2D");
  for (i=0; i<listPointPair.size(); i++) { 
    pp = listPointPair.at(i);
    Q_ASSERT(pp); //should never be NULL
    if (pp) {
      xmlWriter->writeStartElement("Landmark");
      xmlWriter->writeStartElement("Source");
      xmlWriter->writeTextElement("X", QString("%1").arg((*pp)[sourceV].vtX));
      xmlWriter->writeTextElement("Y", QString("%1").arg((*pp)[sourceV].vtY));
      if (is3D)
        xmlWriter->writeTextElement("Z", QString("%1").arg((*pp)[sourceV].vtZ));
      xmlWriter->writeEndElement();
      xmlWriter->writeStartElement("Target");
      xmlWriter->writeTextElement("X", QString("%1").arg((*pp)[targetV].vtX));
      xmlWriter->writeTextElement("Y", QString("%1").arg((*pp)[targetV].vtY));
      if (is3D)
        xmlWriter->writeTextElement("Z", QString("%1").arg((*pp)[targetV].vtZ));
      xmlWriter->writeEndElement();
      xmlWriter->writeEndElement();
    }
 }
 xmlWriter->writeEndElement();
 return true;
}

bool LandmarkModel::saveWarpingAsXml(QXmlStreamWriter *xmlWriter) {
 xmlWriter->writeStartElement(xmlTagWarping);
 xmlWriter->writeTextElement("BasisFunctionType", m_basisFnType == basis_IMQ ? "IMQ":"MQ");
 xmlWriter->writeTextElement("Delta", QString("%1").arg(m_delta));
 xmlWriter->writeTextElement("SnapToFitDist", QString("%1").arg(m_snapToFitDist));
 xmlWriter->writeEndElement();
 return true;
}

bool LandmarkModel::saveAsText(QFile *file) {
  int i;
  PointPair *pp;
  QTextStream stream(file);
  for (i=0; i<listPointPair.size(); i++) {
    pp = listPointPair.at(i);
    Q_ASSERT(pp); //should never be NULL
    if (pp) {
      stream << (*pp)[sourceV].vtX << ' ' << (*pp)[sourceV].vtY;
      if (is3D)
        stream << ' ' << (*pp)[sourceV].vtZ;

      stream << ' ' << (*pp)[targetV].vtX-(*pp)[sourceV].vtX << ' ' << (*pp)[targetV].vtY-(*pp)[sourceV].vtY;
      if (is3D)
        stream << ' ' << (*pp)[targetV].vtZ-(*pp)[sourceV].vtZ ;
      stream << '\n';
    }
  }
  return true;
}

#ifdef DEBUG_LANDMARK_DUMP
bool LandmarkModel::writeLandmarkDebug(QString filename) {
   filename += QDate::currentDate().toString("dd.MM_")+QTime::currentTime().toString("HH:mm:ss") +".wrp";
   QFile file(filename);
   if (!file.open(QFile::WriteOnly | QFile::Text)) {
     return false;
   }
   QXmlStreamWriter xmlWriter(&file);
   saveAsXml(&xmlWriter);

   return true;
}
#endif


void LandmarkModel::addLandmarkPair(const PointPair &pp) {
  int row=listPointPair.size()-1;

  beginInsertRows(QModelIndex(), row, row);
  listPointPair.append(new PointPair(pp));
  endInsertRows();

  #ifdef WRITE_DEBUG_LANDMAKS
  writeLandmarkDebug(WRITE_DEBUG_LANDMAKS);
  #endif

  row++;
  emit addedLandmarkPair(row);
//  emit addedLandmarkPair(listPointPair.size()-1);

  emit setSourceLandmarkValid(row, isDraggerValid(sourceV, &pp));
  emit setTargetLandmarkValid(row, isDraggerValid(targetV, &pp));
  //highlight(row, true);
}

bool LandmarkModel::parseWarpingDOM(const QDomElement &element) {
  if (element.tagName() != xmlTagWarping)
        return false;
  QDomNode child = element.firstChild();
  while (!child.isNull()) {
      if (child.toElement().tagName() == "BasisFunctionType") {
       QString str = child.toElement().text().toUpper();
       if (str == "IMQ")
           m_basisFnType = basis_IMQ;
       else
           m_basisFnType = basis_MQ;
      } else if (child.toElement().tagName() == "Delta") {
          m_delta = child.toElement().text().toDouble();
      } else if (child.toElement().tagName() == "SnapToFitDist") {
          m_snapToFitDist = child.toElement().text().toDouble();
      } else if (child.toElement().tagName() == "") {
        QString type=child.toElement().text();
      }
      child = child.nextSibling();
  }
  return true;
}

bool LandmarkModel::parseDOM(const QDomElement &element) {
  if (element.tagName() != xmlTag)
        return false;
  QDomNode child = element.firstChild();
  PointPair p;
  while (!child.isNull()) {
      if (child.toElement().tagName() == "Landmark") {
          if (readDOMLandmark(child.toElement(),p)) {
            addLandmarkPair(p);
          }
      }
      child = child.nextSibling();
  }
  return true;
}

bool LandmarkModel::readDOMLandmark(const QDomElement &element, PointPair&p) {
  bool isS=false,isT=false;
  if (element.tagName() != "Landmark")
        return false;
  QDomNode child = element.firstChild();
  while (!child.isNull()) {
      if (child.toElement().tagName() == "Source") {
        if (readDOMPoint(child.toElement(), p.V[sourceV])) {
           isS=true;
        }
      } else
      if (child.toElement().tagName() == "Target") {
        if (readDOMPoint(child.toElement(), p.V[targetV])) {
           isT=true;
        }
      }
      child = child.nextSibling();
  }
  return isS && isT;
}

bool LandmarkModel::readDOMPoint(const QDomElement &element, WlzDVertex3 &p) {
  bool isX=false, isY=false, isZ=false;

  QDomNode child = element.firstChild();
  while (!child.isNull()) {
      QDomElement element = child.toElement();
      if (element.tagName() == "X") {
        p.vtX = element.text().toDouble();
        isX = true;
      } else
      if (element.tagName() == "Y") {
        p.vtY = element.text().toDouble();
        isY = true;
      } else
      if (element.tagName() == "Z") {
        p.vtZ = element.text().toDouble();
        isZ = true;
      }
      child = child.nextSibling();
  }
  return isX && isY && (isZ || !is3D);
}

WlzBasisFnTransform *LandmarkModel::getBasisTransform(WoolzObject *cMesh, WlzErrorNum& errNum,  bool isSource) {
   int ns, nd;
   WlzVertexP  source, target;
   const bool useIMQ = m_basisFnType == basis_IMQ;
   source.v = NULL;
   target.v = NULL;

   if (!cMesh || !(cMesh->getObj()) ) {
       errNum = WLZ_ERR_OBJECT_NULL;
    }

   if (is3D) {
     source.d3 = extractVertex3D(LandmarkModel::sourceV, ns);
     target.d3 = extractVertex3D(LandmarkModel::targetV, nd);
     if (!isSource) {
       WlzVertexP  temp = source;
       source = target;
       target = temp;
     }
   } else {
     source.d2 = extractVertex2D(LandmarkModel::sourceV, ns);
     target.d2 = extractVertex2D(LandmarkModel::targetV, nd);
     if (!isSource) {
       WlzVertexP temp = source;
       source = target;
       target = temp;
     }
   }
   Q_ASSERT(ns == nd);
   if (ns!=nd || nd<=0) {
      errNum = WLZ_ERR_UNSPECIFIED;
   }
   if (errNum == WLZ_ERR_NONE) {
     if (!basisTr) {
       if((basisTr = WlzMakeBasisFnTransform(NULL)) == NULL) {
             errNum = WLZ_ERR_MEM_ALLOC;
       } else {
          basisTr->basisFn = NULL;
       }
     }
   }
   if (errNum == WLZ_ERR_NONE) {
       WlzBasisFn  *basisFn;
       basisTr->type = is3D ? WLZ_TRANSFORM_3D_BASISFN : WLZ_TRANSFORM_2D_BASISFN;
       if (is3D)
         basisFn = useIMQ ?
              WlzBasisFnIMQ3DFromCPts(ns,
               source.d3, target.d3,
               m_delta,
               basisTr->basisFn,
               cMesh->getObj()->domain.cm3, &errNum) :
              WlzBasisFnMQ3DFromCPts(ns,
               source.d3, target.d3,
               m_delta,
               basisTr->basisFn,
               cMesh->getObj()->domain.cm3, &errNum);
       else
         basisFn = useIMQ ?
             WlzBasisFnIMQ2DFromCPts(ns,
               source.d2, target.d2,
               m_delta,
               basisTr->basisFn, cMesh->getObj()->domain.cm2, &errNum) :
             WlzBasisFnMQ2DFromCPts(ns,
               source.d2, target.d2,
               m_delta,
               basisTr->basisFn, cMesh->getObj()->domain.cm2, &errNum);

       if (basisTr->basisFn)
         WlzBasisFnFree(basisTr->basisFn);
       basisTr->basisFn = basisFn;
   }
   if (source.v) {
     if (is3D) {
       delete [] source.d3;
       delete [] target.d3;
     } else {
       delete [] source.d2;
       delete [] target.d2;
     }
   }
   return basisTr;
}

void LandmarkModel::setMeshData(WoolzObject *mesh) {
  if (m_mesh == mesh)
     return;
  if (m_mesh)
    disconnect(m_mesh, SIGNAL(objectChanged()), this, SLOT(meshChanged()));
  m_mesh =  mesh;

  if (m_mesh) {
      connect(mesh, SIGNAL(objectChanged()), this, SLOT(meshChanged()));
      invalidateBasisTr();
      updateAllValidity();
      emit warpingChanged();
  }
}

void LandmarkModel::meshChanged() {
  invalidateBasisTr();
  updateAllValidity();
  emit warpingChanged();
}

bool LandmarkModel::isDraggerValid(IndexType indexType, const PointPair *pp)
	const {
  if ( !m_mesh  || (m_mesh->isSource() != (indexType == sourceV)) ) {
    //if mesh type is different then indexType
    return true;
  }

  if ( !m_mesh->getObj() ) {   //if no mesh loaded/computed yet
     return true;
  }

  if (is3D) {
    WlzCMesh3D *mesh=m_mesh->getObj()->domain.cm3;
    if (mesh) { //if mesh loaded or computed
      return WlzCMeshElmEnclosingPos3D (mesh, -1,
                                        pp->V[indexType].vtX,
					pp->V[indexType].vtY,
					pp->V[indexType].vtZ, 0, NULL)>=0;
    }
  } else {
    WlzCMesh2D *mesh=m_mesh->getObj()->domain.cm2;
    if (mesh) { //if mesh loaded or computed
      return WlzCMeshElmEnclosingPos2D (mesh, -1,
                                        pp->V[indexType].vtX,
					pp->V[indexType].vtY, 0, NULL)>=0;
    }
  }
  return true;
}

void LandmarkModel::invalidateBasisTr() {
  if (basisTr) {
     WlzBasisFnFreeTransform(basisTr);
    basisTr = NULL;
  }
}

void LandmarkModel::removeAllLandmarks() {
  invalidateBasisTr();
  beginRemoveRows(QModelIndex(), 0, listPointPair.size()-1);
  for (int i = listPointPair.size()-1;i>=0;i--) {
    emit removedLandmark(i);
  }
  listPointPair.clear();
  endRemoveRows();
}

int LandmarkModel::indexOf(PointPair *pp) const {
  return listPointPair.indexOf(pp);
}

void LandmarkModel::updateAllValidity(){
  int n = listPointPair.size();
  for (int i=0; i<n; i++) {
    const PointPair *pp = listPointPair.at(i);
    emit setSourceLandmarkValid(i, isDraggerValid(sourceV, pp));
    emit setTargetLandmarkValid(i, isDraggerValid(targetV, pp));
  }
}

bool LandmarkModel::isValidLandmakSet(){
  int n = listPointPair.size();
  for (int i=0; i<n; i++) {
    const PointPair *pp = listPointPair.at(i);
    if (!isDraggerValid(sourceV, pp) || !isDraggerValid(targetV, pp))
        return false;
  }
  return true;
}

void LandmarkModel::set3D(bool b) {
    if (is3D !=b) {
        is3D = b;
        if (is3D) {
            beginInsertColumns(QModelIndex(), 5, 6);
            endInsertColumns();
        } else {
            beginRemoveColumns(QModelIndex(), 5, 6);
            endRemoveColumns();
        }
    }
}
