#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _LandmarkModel_h[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         LandmarkModel.h
* \author       Zsolt Husz
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
* \brief	Model of landmarks.
* \ingroup	Control
*/

#ifndef LANDMARKMODEL_H
#define LANDMARKMODEL_H

#include <QWidget>
#include <QAbstractItemModel>

#include <Wlz.h>
#include <Inventor/SbLinear.h>

#include <QtXml/QXmlStreamReader>

// define to dump landmark set to file in landmark operations
//#define DEBUG_LANDMARK_DUMP "/tmp/dumfile"

class WoolzObject;
class QFile;
class QDomElement;

/*!
* \brief	Vertex pair structure, for storing target and source correspondences
* \ingroup	Control
*/
struct PointPair
{
  WlzDVertex3 V[2];
  WlzDVertex3 & operator[](const int i) {return(V[i]);}
};


/*!
* \brief	Model of landmarks. Stores and manages landmark corespondences.
* \ingroup	Control
*/
class LandmarkModel : public QAbstractItemModel
{
  Q_OBJECT
  public:
    typedef enum {basis_IMQ, basis_MQ} BasisFnType; /*!< basis function type */

    /*!
     * \brief	Index constants
     * \ingroup	Control
     */
    typedef enum {sourceV = 0, targetV = 1} IndexType ;
    static const char * xmlTag;          	/*!< xml section tag string */
    static const char * xmlTagWarping;   	/*!< xml section tag string for
    					      	     warping section*/

  public:

    /*!
     * \ingroup	Control
     * \brief	Constructor
     * \param	parent			parent object
     */
    LandmarkModel(QObject * parent = 0);

    /*!
     * \ingroup	Control
     * \brief	Destructor
     */
    virtual ~LandmarkModel();

    /*!
     * \return	the number of landmarks for the root index and 0 for any
     * 		other index.
     * \ingroup	Control
     * \brief	Returns the number of rows.

     * \param	parent			parent index
     */
    int rowCount(const QModelIndex & parent) const;

    /*!
     * \return	5 for 2D landmarks and 7 for 3D landmarks.
     * \ingroup	Control
     * \brief	Returns the number of columns.
     * \param	parent			parent index
     */
    int columnCount(const QModelIndex & parent) const;

    /*!
     * \return	data requested
     * \ingroup	Control
     * \brief	Returns the data stored under the given role for the item
     *          referred to by the index.
     * \param	index			of requested data
     * \param	role			requested role
     */
    QVariant data(const QModelIndex & index, int role) const;

    /*!
     * \return	data requested
     * \ingroup	Control
     * \brief	Returns the data for the given role and section in the
     *          header with the specified orientation.
     * \param	section			requested section
     * \param	orientation			requested orientation 
     * \param	role			requested role
     */
    QVariant headerData(int section, Qt::Orientation orientation, int role)
      const;

    /*!
     * \return	index flags
     * \ingroup	Control
     * \brief	Returns the item flags for the given index.
     * \param	index			of requested data
     */
    Qt::ItemFlags flags(const QModelIndex &index) const;

    /*!
     * \return	index of the requested item
     * \ingroup	Control
     * \brief	Returns the index of the item in the model specified by the
     * 		given row, column and parent index.
     * \param	row			row number
     * \param	column			column number
     * \param	parent			index of the parent item
     */
    QModelIndex index(int row, int column,
	const QModelIndex &parent) const ;
    /*!
     * \return	index of the parent item
     * \ingroup	Control
     * \brief	Returns the parent of the model index, or QModelIndex() if
     *               it has no parent.
     * \param	child			index of the child
     */
    QModelIndex parent(const QModelIndex &child) const ;

    /*!
     * \return	true if success, false if failed
     * \ingroup	Control
     * \brief	Sets the role data for the item at index to value. Returns
     *   	true if successful; otherwise returns false.
     * \param	index			to be set
     * \param	value			new value
     * \param	role			to be set
     */
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    /*!
     * \return	point pair
     * \ingroup	Control
     * \brief	Returns the PointPair at the requeted index.
     *
     *          Generates assertion if invalid index is requested.
     * \param	index			of the pointpair requested
     */
    PointPair* getPointPair(int index);

    /*!
     * \return	landmark point coordinates
     * \ingroup	Control
     * \brief	Returns the coordinates of the source or target landmarks
     * 		of a PointPair
     * \param	index			of the pointpair requested
     * \param	indexType			part type requested (sourceV or
     * 					targetV)
     */
    SbVec3f getHalfPointPair(int index, IndexType indexType);

    /*!
     * \return	array of 2D landmark coordiantes
     * \ingroup	Control
     * \brief	Returns the array of the source or target 2D landmarks.
     *
     *          Note: To avoid memory leaks, array must be deleted by the user.
     * \param	indexType			part type requested (sourceV or
     * 					targetV)
     * \param	n			size of the returned array
     */
    WlzDVertex2* extractVertex2D(IndexType indexType, int& n);

    /*!
     * \ingroup	Control
     * \brief	Returns the array of the source or target 3D landmarks.
     *
     *          Note: To avoid memory leaks, array must be deleted by the
     *          user.
     * \param	indexType		part type requested (sourceV or
     * 					targetV)
     * \param	n			size of the returned array
     * \return	array of 3D landmark coordiantes
     */
    WlzDVertex3* extractVertex3D(IndexType indexType, int& n);

    /*!
     * \ingroup	Control
     * \brief	Remove a landmark pair
     * \param	index			landmark index to remove
     */
    void removeLandmark(int index) ;

    /*!
     * \ingroup	Control
     * \brief	Remove last landmark pair
     */
    void removeLast();

    /*!
     * \ingroup	Control
     * \brief	Insert a new landmark pair at a specific location
     * \param	index			landmark index location to add
     * \param	pp			landmark pair
     */
    void insertLandmarkPair(int index, PointPair &pp);

    /*!
     * \ingroup	Control
     * \brief	Return the number of landmarks
     * \return	number of landmarks
     */
    int landmarkNumber() {return(listPointPair.size());}

    /*!
     * \ingroup	Control
     * \brief	Move source or target component of a landmark pair copying
     * 		  half of a new landmark pair
     * \param	index			index of landmark to be moved
     * \param	newPP			point pair with new location
     * \param	indexType			part type to be moved
     */
    void move(int index, PointPair newPP, LandmarkModel::IndexType indexType);

    /*!
     * \ingroup	Control
     * \brief	Move source of target component of a landmark pair
     * \param	index			index of landmark to be moved
     * \param	newPosition			new position
     * \param	indexType			part type to be moved
     */
    void move(int index, const SbVec3f newPosition,
	LandmarkModel::IndexType indexType);

    /*!
     * \ingroup	Control
     * \brief	Move source of target component of a landmark pair
     * \param	index			index of landmark to be moved
     * \param	newPosition			new position
     * \param	indexType			part type to be moved
     */
    void move(int index, const WlzDVertex3 newPosition,
	LandmarkModel::IndexType indexType);

    /*!
     * \ingroup	Control
     * \brief	Set 3D flag of the model. Should be used only after the
     * 		model is created.
     * \param	b		true if model is 3D, false if model is 2D
     */
    void set3D(bool b);

    /*!
     * \ingroup	Control
     * \brief	Returns the 3D flag of the model.
     * \return	true if model is 3D, false if model is 2D
     */
    virtual bool get3D() {return(is3D);}

    /*!
     * \ingroup	Control
     * \brief	Saves model in xml format.
     * \param	xmlWriter			output xml stream
     * \return	true if succeded, false if not
     */
    virtual bool saveAsXml(QXmlStreamWriter *xmlWriter);

    /*!
     * \ingroup	Control
     * \brief	Saves model in xml format.
     * \param	xmlWriter			output xml stream
     * \return	true if succeded, false if not
     */
    virtual bool saveWarpingAsXml(QXmlStreamWriter *xmlWriter);

    /*!
     * \ingroup	Control
     * \brief	Reads landmark model from DOM tree
     * \param	element			current element of the DOM tree
     * \return	true if succeded, false if not
     */
    bool parseDOM(const QDomElement &element);

    /*!
     * \ingroup	Control
     * \brief	Reads warping parameters of the model from DOM tree
     * \param	element			current element of the DOM tree
     * \return	true if succeded, false if not
     */
    bool parseWarpingDOM(const QDomElement &element);

    /*!
     * \ingroup	Control
     * \brief	Generates the basis function tranfrom
     * \param	cMesh			mesh to be transformed
     * \param	errNum			error number
     * \param	isSource		true if transformation uses source
     * 					Mesh, false if uses target mesh
     * \return	basis function transform sturcture.
     */
    WlzBasisFnTransform *getBasisTransform(WoolzObject *cMesh,
	WlzErrorNum& errNum,
	bool isSource);
    /*!
     * \ingroup	Control
     * \brief	Returns if landmark is valid or not
     * \param	indexType		view type requested (sourceV or
     * 					targetV)
     * \param	pp			landmark pointpair to check 
     * \return	true if landmark is valid, false otherwise
     */
    bool isDraggerValid(IndexType indexType, const PointPair *pp) const;

    /*!
     * \ingroup	Control
     * \brief	Sets the mesh data used for warping
     * \param	mesh			mesh data
     */
    void setMeshData(WoolzObject *mesh);

    /*!
     * \ingroup	Control
     * \brief	Adds the new PointPair to the internal list of landmarks.
     */
    void addLandmarkPair(const PointPair &pp);

    /*!
     * \ingroup	Control
     * \brief	Finds index number of a PointPair
     * \param	parent			parent index
     * \return	5 for 2D landmarks and 7 for 3D landmarks.
     */
    int indexOf(PointPair *pp) const;

    /*!
     * \ingroup	Control
     * \brief	Sets radial basis function delta value
     * \param	delta			new delta value
     */
    void setDelta(double delta) 
    {
      if(m_delta != delta) 
      {
	m_delta = delta;
	invalidateBasisTr();
	emit warpingChanged();
      }
    }

    /*!
     * \ingroup	Control
     * \brief	Sets landmark snap to fit distance
     * \param	stfd			new snap to fit distance
     */
    void setSnapToFitDist(double stfd) 
    {
      if(m_snapToFitDist != stfd) 
      {
	m_snapToFitDist = stfd;
      }
    }

    /*!
     * \ingroup	Control
     * \brief	Return the current delta value
     * \return	delta value
     */
    double delta() {return(m_delta);}

    /*!
     * \ingroup	Control
     * \brief	Return the current snap to fit distance
     * \return	snap to fit distance
     */
    double snapToFitDist() {return(m_snapToFitDist);}

    /*!
     * \ingroup	Control
     * \brief	Sets new basis function to be used for warping
     * \param	basisFnType			new type
     */
    void setBasisFnType(BasisFnType basisFnType) 
    {
      if(m_basisFnType != basisFnType) 
      {
	m_basisFnType = basisFnType;
	invalidateBasisTr();
	emit warpingChanged();
      }
    }

    /*!
     * \ingroup	Control
     * \brief	Return the current basis function type
     * \return	basis function type
     */
    BasisFnType basisFnType() {return(m_basisFnType);}

  public slots:
    /*!
     * \ingroup	Control
     * \brief	Removes all landmarks.
     */
    virtual void removeAllLandmarks();

    /*!
     * \ingroup	Control
     * \brief	Updates the validity of all landmarks.
     */
    virtual void updateAllValidity();

    /*!
     * \ingroup	Control
     * \brief	Processes mesh changes.
     */
    virtual void meshChanged();

    /*!
     * \ingroup	Control
     * \brief	Checks if all landmarks are valid
     * \return	true if all landmark are valid, false otherwise
     */
    virtual  bool isValidLandmakSet();

    /*!
     * \ingroup	Control
     * \brief	Writes landmarks into the file specified.
     *
     *               Text file format is space delimited:
     * \verbatim      SourceX SourceY SourceZ TargetX TargetY TargetZ
     *                 for 3D landmarks
     * \verbatim      SourceX SourceY TargetX TargetY
     *                 2D landmarks
     * \param	filename			name of the file
     * \return	true if succeded, false otherwise
     */
    virtual bool saveAsText(QFile *file);

  protected:
#ifdef DEBUG_LANDMARK_DUMP
    /*!
     * \ingroup	Control
     * \brief	Writes landmarks into a text file for DEBUG purpose only.
     * \param	filename			name of the file
     * \return	true if succeded, false otherwise
     */
    virtual bool writeLandmarkDebug(QString filename);
#endif

signals:

    /*!
     * \ingroup	Control
     * \brief	Signals addition of a landmark pair.
     * \param	index			new landmark PointPair index
     */
    void addedLandmarkPair(const int index);

    /*!
     * \ingroup	Control
     * \brief	Signals removal of a landmark pair.
     * \param	index			landmark PointPair index
     */
    void removedLandmark(const int index);

    /*!
     * \ingroup	Control
     * \brief	Signals change of a source landmark.
     * \param	index			landmark PointPair index
     * \param	point			new landmark coordinate
     */
    void movedSourceLandmark(const int index, const WlzDVertex3 point);

    /*!
     * \ingroup	Control
     * \brief	Signals change of a target landmark.
     * \param	index			landmark PointPair index
     * \param	point			new landmark coordinate
     */
    void movedTargetLandmark(const int index, const WlzDVertex3 point);

    /*!
     * \ingroup	Control
     * \brief	Signals change of the validity of a source landmark.
     * \param	index			landmark PointPair index
     * \param	on			new validity of the landmark
     */
    void setSourceLandmarkValid(const int index, const bool on);

    /*!
     * \ingroup	Control
     * \brief	Signals change of the validity of a target landmark.
     * \param	index			landmark PointPair index
     * \param	on			new validity of the landmark
     */
    void setTargetLandmarkValid(const int index, const bool on);

    /*!
     * \ingroup	Control
     * \brief	Signals change of warping settings.
     */
    void warpingChanged();

  private:
    /*!
     * \ingroup	Control
     * \brief	Reads a point from a DOM tree
     * \param	element			current element of the DOM tree
     * \param	p			read result (output only)
     * \return	true if succeded, false otherwise
     */
    bool readDOMPoint(const QDomElement &element, WlzDVertex3 &p);

    /*!
     * \ingroup	Control
     * \brief	Reads from a landmark PointPair from a DOM tree
     * \param	element			current element of the DOM tree
     * \param	p			read result (output only)
     * \return	true if succeded, false otherwise
     */
    bool readDOMLandmark(const QDomElement &element, PointPair&p);

  private:
    /*!
     * \ingroup	Control
     * \brief	Invalidates basis transform
     */
    void invalidateBasisTr();

  protected:
    bool is3D;               		/*!< if this is a 3D or 2D landmark
    					     model */

    QList <PointPair*> listPointPair;  	/*!< list of node corespondences */
    WlzBasisFnTransform *basisTr;      	/*!< basis function transform
					     defined by the landmark set */

    WoolzObject *m_mesh;               	/*!< mesh data for warping */

    BasisFnType m_basisFnType;         	/*!< basis function type */
    double m_delta;                    	/*!< basis function delta value */
    double m_snapToFitDist;		/*!< snap to mesh distance */
};


#endif // LANDMARKMODEL_H
