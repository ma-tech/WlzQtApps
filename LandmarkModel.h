#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _LandmarkModel_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         LandmarkModel.h
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

#ifndef LANDMARKMODEL_H
#define LANDMARKMODEL_H

//#define DEBUG_LANDMARK_DUMP "/net/homehost/export/home/zsolth/ma-week/Warp/debug/debug"  // if defined, any landmark operation will dump the landmark set to this text file

#include <QWidget>
#include <QAbstractItemModel>

#include <Wlz.h>
#include <Inventor/SbLinear.h>

#include <QtXml/QXmlStreamReader>

class WoolzObject;
class QFile;
class QDomElement;

/*!
* \brief	Vertex pair structure, for storing target and source correspondences
* \ingroup      Control
*/
struct PointPair
{
  WlzDVertex3 V[2];
  WlzDVertex3 & operator[] (const int i) {return V[i];}
};


/*!
* \brief	Model of landmarks. Stores and manages landmark corespondences.
* \ingroup      Control
*/
class LandmarkModel : public QAbstractItemModel
{
  Q_OBJECT
public:
  typedef enum {basis_IMQ, basis_MQ} BasisFnType;  /*!< basis function type */

 /*!
  * \brief       Index constants
  * \ingroup     Control
  */
  typedef enum { sourceV = 0, targetV = 1} IndexType ;
  static const char * xmlTag;          /*!< xml section tag string */
  static const char * xmlTagWarping;   /*!< xml section tag string for warping section*/

public:

 /*!
  * \ingroup      Control
  * \brief        Constructor
  * \param        parent parent object
  * \return       void
  * \par      Source:
  *                LandmarkModel.cpp
  */
  LandmarkModel ( QObject * parent = 0 );

 /*!
  * \ingroup      Control
  * \brief        Destructor
  * \return       void
  * \par      Source:
  *                LandmarkModel.cpp
  */
  virtual ~LandmarkModel ( );

 /*!
  * \ingroup      Control
  * \brief        Returns the number of rows.

  * \param        parent parent index
  * \return       the number of landmarks for the root index and 0 for any other index.
  * \par      Source:
  *                LandmarkModel.cpp
  */
  int rowCount(const QModelIndex & parent) const;

 /*!
  * \ingroup      Control
  * \brief        Returns the number of columns.
  * \param        parent parent index
  * \return       5 for 2D landmarks and 7 for 3D landmarks.
  * \par      Source:
  *                LandmarkModel.cpp
  */
  int columnCount(const QModelIndex & parent) const;

 /*!
  * \ingroup      Control
  * \brief        Returns the data stored under the given role for the item referred to by the index.
  * \param        index of requested data
  * \param        role requested role
  * \return       data requested
  * \par      Source:
  *                LandmarkModel.cpp
  */
  QVariant data(const QModelIndex & index, int role ) const;

 /*!
  * \ingroup      Control
  * \brief        Returns the data for the given role and section in the header with the specified orientation.
  * \param        section requested section
  * \param        orientation requested orientation 
  * \param        role requested role
  * \return       data requested
  * \par      Source:
  *                LandmarkModel.cpp
  */
  QVariant headerData(int section, Qt::Orientation orientation, int role ) const;

 /*!
  * \ingroup      Control
  * \brief        Returns the item flags for the given index.
  * \param        index of requested data
  * \return       index flags
  * \par      Source:
  *                LandmarkModel.cpp
  */
  Qt::ItemFlags flags(const QModelIndex &index) const;

 /*!
  * \ingroup      Control
  * \brief        Returns the index of the item in the model specified by the given row, column and parent index.
  * \param        row row number
  * \param        column column number
  * \param        parent index of the parent item
  * \return       index of the requested item
  * \par      Source:
  *                LandmarkModel.cpp
  */
  QModelIndex index(int row, int column,
                              const QModelIndex &parent) const ;
 /*!
  * \ingroup      Control
  * \brief        Returns the parent of the model index, or QModelIndex() if it has no parent.
  * \param        child index of the child
  * \return       index of the parent item
  * \par      Source:
  *                LandmarkModel.cpp
  */
  QModelIndex parent(const QModelIndex &child) const ;

 /*!
  * \ingroup      Control
  * \brief        Sets the role data for the item at index to value. Returns true if successful; otherwise returns false.
  * \param        index to be set
  * \param        value new value
  * \param        role to be set
  * \return       true if success, false if failed
  * \par      Source:
  *                LandmarkModel.cpp
  */
  bool setData(const QModelIndex &index, const QVariant &value, int role);

 /*!
  * \ingroup      Control
  * \brief        Returns the PointPair at the requeted index.
  *
  *               Generates assertion if invalid index is requested.
  * \param        index of the pointpair requested
  * \return       point pair
  * \par      Source:
  *                LandmarkModel.cpp
  */
  PointPair* getPointPair(int index);

 /*!
  * \ingroup      Control
  * \brief        Returns the coordinates of the source or target landmarks of a PointPair
  * \param        index of the pointpair requested
  * \param        indexType part type requested (sourceV or targetV)
  * \return       landmark point coordinates
  * \par      Source:
  *                LandmarkModel.cpp
  */
  SbVec3f getHalfPointPair(int index, IndexType indexType);

 /*!
  * \ingroup      Control
  * \brief        Returns the array of the source or target 2D landmarks.
  *
  *               Note: To avoid memory leaks, array must be deleted by the user.
  * \param        indexType part type requested (sourceV or targetV)
  * \param        n size of the returned array
  * \return       array of 2D landmark coordiantes
  * \par      Source:
  *                LandmarkModel.cpp
  */
  WlzDVertex2* extractVertex2D(IndexType indexType, int& n);

 /*!
  * \ingroup      Control
  * \brief        Returns the array of the source or target 3D landmarks.
  *
  *               Note: To avoid memory leaks, array must be deleted by the user.
  * \param        indexType part type requested (sourceV or targetV)
  * \param        n size of the returned array
  * \return       array of 3D landmark coordiantes
  * \par      Source:
  *                LandmarkModel.cpp
  */
  WlzDVertex3* extractVertex3D(IndexType indexType, int& n);

 /*!
  * \ingroup      Control
  * \brief        Remove a landmark pair
  * \param        index landmark index to remove
  * \return       void
  * \par      Source:
  *                LandmarkModel.cpp
  */
  void removeLandmark(int index) ;

 /*!
  * \ingroup      Control
  * \brief        Remove last landmark pair
  * \return       void
  * \par      Source:
  *                LandmarkModel.cpp
  */
  void removeLast();

 /*!
  * \ingroup      Control
  * \brief        Insert a new landmark pair at a specific location
  * \param        index landmark index location to add
  * \param        pp landmark pair
  * \return       void
  * \par      Source:
  *                LandmarkModel.cpp
  */
  void insertLandmarkPair(int index, PointPair &pp);

 /*!
  * \ingroup      Control
  * \brief        Return the number of landmarks
  * \return       number of landmarks
  * \par      Source:
  *                LandmarkModel.cpp
  */
  int landmarkNumber() {return listPointPair.size();}

 /*!
  * \ingroup      Control
  * \brief        Move source or target component of a landmark pair copying half of a new landmark pair
  * \param        index index of landmark to be moved
  * \param        newPP point pair with new location
  * \param        indexType part type to be moved
  * \return       void
  * \par      Source:
  *                LandmarkModel.cpp
  */
  void move(int index, PointPair newPP, LandmarkModel::IndexType indexType);

 /*!
  * \ingroup      Control
  * \brief        Move source of target component of a landmark pair
  * \param        index index of landmark to be moved
  * \param        newPosition new position
  * \param        indexType part type to be moved
  * \return       void
  * \par      Source:
  *                LandmarkModel.cpp
  */
  void move(int index, const SbVec3f newPosition, LandmarkModel::IndexType indexType);

 /*!
  * \ingroup      Control
  * \brief        Set 3D flag of the model. Should be used only after the model is created.
  * \param        b true if model is 3D, false if model is 2D
  * \return       void
  * \par      Source:
  *                LandmarkModel.cpp
  */
  void set3D(bool b);

 /*!
  * \ingroup      Control
  * \brief        Returns the 3D flag of the model.
  * \return       true if model is 3D, false if model is 2D
  * \par      Source:
  *                LandmarkModel.cpp
  */
  virtual bool get3D() {return is3D;}

 /*!
  * \ingroup      Control
  * \brief        Saves model in xml format.
  * \param        xmlWriter output xml stream
  * \return       true if succeded, false if not
  * \par      Source:
  *                LandmarkModel.cpp
  */
  virtual bool saveAsXml(QXmlStreamWriter *xmlWriter);

/*!
  * \ingroup      Control
  * \brief        Saves model in xml format.
  * \param        xmlWriter output xml stream
  * \return       true if succeded, false if not
  * \par      Source:
  *                LandmarkModel.cpp
  */
  virtual bool saveWarpingAsXml(QXmlStreamWriter *xmlWriter);

 /*!
  * \ingroup      Control
  * \brief        Reads landmark model from DOM tree
  * \param        element current element of the DOM tree
  * \return       true if succeded, false if not
  * \par      Source:
  *                LandmarkModel.cpp
  */
  bool parseDOM(const QDomElement &element);

  /*!
  * \ingroup      Control
  * \brief        Reads warping parameters of the model from DOM tree
  * \param        element current element of the DOM tree
  * \return       true if succeded, false if not
  * \par      Source:
  *                LandmarkModel.cpp
  */
  bool parseWarpingDOM(const QDomElement &element);

 /*!
  * \ingroup      Control
  * \brief        Generates the basis function tranfrom
  * \param        cMesh mesh to be transformed
  * \param        errNum error number
  * \param        isSource true if transformation uses source Mesh, false if uses target mesh
  * \return       basis function transform sturcture.
  * \par      Source:
  *                LandmarkModel.cpp
  */
  WlzBasisFnTransform *getBasisTransform(WoolzObject *cMesh, WlzErrorNum& errNum,
       bool isSource);
 /*!
  * \ingroup      Control
  * \brief        Returns if landmark is valid or not
  * \param        indexType view type requested (sourceV or targetV)
  * \param        pp landmark pointpair to check 
  * \return       true if landmark is valid, false otherwise
  * \par      Source:
  *                LandmarkModel.cpp
  */
  bool isDraggerValid(IndexType indexType, const PointPair *pp) const;

 /*!
  * \ingroup      Control
  * \brief        Sets the mesh data used for warping
  * \param        mesh mesh data
  * \return       void
  * \par      Source:
  *                LandmarkModel.cpp
  */
  void setMeshData(WoolzObject *mesh);

 /*!
  * \ingroup      Control
  * \brief        Adds the new PointPair to the internal list of landmarks.
  * \return       void
  * \par      Source:
  *                LandmarkModel.cpp
  */
  void addLandmarkPair(const PointPair &pp);

 /*!
  * \ingroup      Control
  * \brief        Finds index number of a PointPair
  * \param        parent parent index
  * \return       5 for 2D landmarks and 7 for 3D landmarks.
  * \par      Source:
  *                LandmarkModel.cpp
  */
  int indexOf(PointPair *pp) const;

 /*!
  * \ingroup      Control
  * \brief        Sets mesh generation distances
  * \param        delta new delta value
  * \return       void
  * \par      Source:
  *                LandmarkModel.cpp
  */
  void setDelta (double delta) {
      if (m_delta != delta) {
        m_delta = delta;
        invalidateBasisTr();
        emit warpingChanged();
      }
  }

 /*!
  * \ingroup      Control
  * \brief        Return the current delta value
  * \return       delta value
  * \par      Source:
  *                LandmarkModel.cpp
  */
  double delta() { return  m_delta;}

 /*!
  * \ingroup      Control
  * \brief        Sets new basis function to be used for warping
  * \param        basisFnType new type
  * \return       void
  * \par      Source:
  *                LandmarkModel.cpp
  */
  void setBasisFnType (BasisFnType basisFnType) {
    if (m_basisFnType != basisFnType) {
      m_basisFnType = basisFnType;
      invalidateBasisTr();
      emit warpingChanged();
    }
  }

 /*!
  * \ingroup      Control
  * \brief        Return the current basis function type
  * \return       basis function type
  * \par      Source:
  *                LandmarkModel.cpp
  */
  BasisFnType basisFnType() { return  m_basisFnType;}

public slots:
 /*!
  * \ingroup      Control
  * \brief        Removes all landmarks.
  * \return       void
  * \par      Source:
  *                LandmarkModel.cpp
  */
  virtual void removeAllLandmarks();

 /*!
  * \ingroup      Control
  * \brief        Updates the validity of all landmarks.
  * \return       void
  * \par      Source:
  *                LandmarkModel.cpp
  */
  virtual void updateAllValidity();

 /*!
  * \ingroup      Control
  * \brief        Processes mesh changes.
  * \return       void
  * \par      Source:
  *                LandmarkModel.cpp
  */
  virtual void meshChanged();

 /*!
  * \ingroup      Control
  * \brief        Checks if all landmarks are valid
  * \return       true if all landmark are valid, false otherwise
  * \par      Source:
  *                LandmarkModel.cpp
  */
  virtual  bool isValidLandmakSet();

 /*!
  * \ingroup      Control
  * \brief        Writes landmarks into the file specified.
  *
  *               Text file format is space delimited:
  * \verbatim      SourceX SourceY SourceZ TargetX TargetY TargetZ
  *                 for 3D landmarks
  * \verbatim      SourceX SourceY TargetX TargetY
  *                 2D landmarks
  * \param        filename name of the file
  * \return       true if succeded, false otherwise
  * \par      Source:
  *                LandmarkModel.cpp
  */
  virtual bool saveAsText(QFile *file);

protected:
#ifdef DEBUG_LANDMARK_DUMP
 /*!
  * \ingroup      Control
  * \brief        Writes landmarks into a text file for DEBUG purpose only.
  * \param        filename name of the file
  * \return       true if succeded, false otherwise
  * \par      Source:
  *                LandmarkModel.cpp
  */
  virtual bool writeLandmarkDebug(QString filename);
#endif

signals:

 /*!
  * \ingroup      Control
  * \brief        Signals addition of a landmark pair.
  * \param        index new landmark PointPair index
  * \return       void
  * \par      Source:
  *                LandmarkModel.cpp
  */
  void addedLandmarkPair(const int index);

 /*!
  * \ingroup      Control
  * \brief        Signals removal of a landmark pair.
  * \param        index landmark PointPair index
  * \return       void
  * \par      Source:
  *                LandmarkModel.cpp
  */
  void removedLandmark(const int index);

 /*!
  * \ingroup      Control
  * \brief        Signals change of a source landmark.
  * \param        index landmark PointPair index
  * \param        point new landmark coordinate
  * \return       void
  * \par      Source:
  *                LandmarkModel.cpp
  */
  void movedSourceLandmark(const int index, const WlzDVertex3 point);

 /*!
  * \ingroup      Control
  * \brief        Signals change of a target landmark.
  * \param        index landmark PointPair index
  * \param        point new landmark coordinate
  * \return       void
  * \par      Source:
  *                LandmarkModel.cpp
  */
  void movedTargetLandmark(const int index, const WlzDVertex3 point);

 /*!
  * \ingroup      Control
  * \brief        Signals change of the validity of a source landmark.
  * \param        index landmark PointPair index
  * \param        on new validity of the landmark
  * \return       void
  * \par      Source:
  *                LandmarkModel.cpp
  */
  void setSourceLandmarkValid(const int index, const bool on);

 /*!
  * \ingroup      Control
  * \brief        Signals change of the validity of a target landmark.
  * \param        index landmark PointPair index
  * \param        on new validity of the landmark
  * \return       void
  * \par      Source:
  *                LandmarkModel.cpp
  */
  void setTargetLandmarkValid(const int index, const bool on);

 /*!
  * \ingroup      Control
  * \brief        Signals change of warping settings.
  * \return       void
  * \par      Source:
  *                LandmarkModel.cpp
  */
  void warpingChanged();

private:
 /*!
  * \ingroup      Control
  * \brief        Reads a point from a DOM tree
  * \param        element current element of the DOM tree
  * \param        p read result (output only)
  * \return       true if succeded, false otherwise
  * \par      Source:
  *                LandmarkModel.cpp
  */
  bool readDOMPoint(const QDomElement &element, WlzDVertex3 &p);

 /*!
  * \ingroup      Control
  * \brief        Reads from a landmark PointPair from a DOM tree
  * \param        element current element of the DOM tree
  * \param        p read result (output only)
  * \return       true if succeded, false otherwise
  * \par      Source:
  *                LandmarkModel.cpp
  */
  bool readDOMLandmark(const QDomElement &element, PointPair&p);

private:
 /*!
  * \ingroup      Control
  * \brief        Invalidates basis transform
  * \return       void
  * \par      Source:
  *                LandmarkModel.cpp
  */
  void invalidateBasisTr();

protected:
  bool is3D;                               /*!< if this is a 3D or 2D landmark model */

  QList <PointPair*> listPointPair;        /*!< list of node corespondences */
  WlzBasisFnTransform *basisTr;            /*!< basis function transform defined by the landmark set */

  WoolzObject *m_mesh;                     /*!< mesh data for warping */

  BasisFnType m_basisFnType;               /*!< basis function type*/
  double m_delta;                          /*!< basis function delta value*/
};


#endif // LANDMARKMODEL_H
