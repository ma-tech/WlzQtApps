#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _WoolzObject_h[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         WoolzObject.h
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
* \brief	Container class for WlzObject type
* \ingroup	Control
*/

#ifndef WOOLZOBJECT_H
#define WOOLZOBJECT_H

//Woolz includes
#include <Wlz.h>
#include <list>

//Qt includes
#include <QString>
#include <QObject>
#include <QColor>

//Inventor  includes
#include <Inventor/SbColor.h>

class SoGroup;
class ObjectListModel;
class TransferFunction;
class QDomElement;
#include <QtXml/QXmlStreamWriter>

/*!
* \brief	Container class for WlzObject
*
*               Inherits QObject, hence signaling mechanism is available
* \ingroup	Control
*/
class WoolzObject: public QObject
{
  Q_OBJECT

  public:

    /*!
     * \brief	Object type definition
     */
    typedef enum
    {
      none = 0,
      source = 1,
      target = 2,
      generated = 4
    } WoolzObjectType;

    /*!
     * \ingroup	Control
     * \brief	Constructor
     */
    WoolzObject();

    /*!
     * \ingroup	Control
     * \brief	Destructor
     */
    virtual ~WoolzObject();

    /*!
     * \return	true if operation succeded, false otherwise
     * \ingroup	Control
     * \brief	Saves the stored WlzObject into a file
     * \param	filename		filename to write the object to
     */
    virtual bool saveAs(QString filename);

    /*!
     * \return	true if succeded, false if not
     * \ingroup	Control
     * \brief	Saves object in xml format.
     * \param	xmlWriter		output xml stream
     */
    virtual bool saveAsXml(QXmlStreamWriter * /*xmlWriter*/) {return true;}

    /*!
     * \return	true if succeded, false if not
     * \ingroup	Control
     * \brief	Saves object details in xml format.
     * \param	xmlWriter		output xml stream
     */
    virtual bool saveAsXmlProperties(QXmlStreamWriter *xmlWriter);

    /*!
     * \ingroup	Control
     * \brief	Deallocates the stored object
     */
    virtual void close();

  public:
    // attribute accessor methods
    //
    /*!
     * \return	WlzObject stored
     * \ingroup	Control
     * \brief	Returns the stored WlzObject
     */
    virtual WlzObject * getObj()
    {
      if(!m_obj)     // if no object is available try update
      {
	update(true);
      }
      return(m_obj);
    }

    /*!
     * \ingroup	Control
     * \brief	Sets the removable flag.
     * \param	newRemovable		if the object is removable
     */
    virtual void setRemovable(bool newRemovable) {m_removable = newRemovable;}

    /*!
     * \return	true if removable, false otherwise
     * \ingroup	Control
     * \brief	Returns the removable flag
     */
    virtual bool removable() {return(m_removable);}

    /*!
     * \ingroup	Control
     * \brief	Sets the visible flag.
     * \param	visible			if the object is visible
     */
    virtual void setVisible(bool visible);

    /*!
     * \return	true if removable, false otherwise
     * \ingroup	Control
     * \brief	Returns the visible flag
     */
    virtual bool visible() {return(m_visible);}

    /*!
     * \ingroup	Control
     * \brief	Sets the name of the object.
     * \param	Removable			if the object is removable
     */
    virtual void setName(QString  newName)
    {
      if(newName != m_name)
      {
	m_name = newName;
	emit objectPropertyChanged();
      }
    }

    /*!
     * \return	object name
     * \ingroup	Control
     * \brief	Returns the name of the object
     */
    virtual QString name() {return(m_name);}

    /*!
     * \return	Notes (parameters of the object)
     * \ingroup	Control
     * \brief	Returns notes of the object.
     *
     *               Must be reimplemented in subclasses
     */
    virtual QString notes() {return("");}

    /*!
     * \return	Object type
     * \ingroup	Control
     * \brief	Returns the type of the object.
     */
    virtual WoolzObjectType type() {return(m_type);}

    /*!
     * \return	true if is a 2D or 3D mesh
     * \ingroup	Control
     * \brief	Checkes if the object is a Mesh
     */
    virtual bool isMesh();

    /*!
     * \return	true if is a 2D or 3D contour
     * \ingroup	Control
     * \brief	Checkes if the object is a contour
     */
    virtual bool isContour();

    /*!
     * \return	true if is a 2D or 3D convex hull
     * \ingroup	Control
     * \brief	Checkes if the object is a contour
     */
    virtual bool isConvHull();

    /*!
     * \return	true if is a value object
     * \ingroup	Control
     * \brief	Checkes if the object is a value object
     */
    virtual bool isValue();

    /*!
     * \return	true if is a value object with values set
     * \ingroup	Control
     * \brief	Checkes if the object is a value object and has values
     */
    virtual bool isValueSet();

    /*!
     * \ingroup	Control
     * \brief	Checkes if the object is a colour value object
     * \return	true if is a colour value object 
     */
    virtual bool isColour();

    /*!
     * \ingroup	Control
     * \brief	Returns the grey type of the object
     * \return	grey type or WLZ_GREY_ERROR on error
     */
    WlzGreyType getWoolzGreyType();

    /*!
     * \ingroup	Control
     * \brief	Checks without reading the whole file if the object is a Mesh
     * \return	true if is a 2D or 3D mesh
     */
    virtual bool isMeshPreRead() {return(isMesh());}

    /*!
     * \ingroup	Control
     * \brief	Checks without reading the whole file if the object is a
     * 		Value object
     * \return	true if is a 2D or 3D value object
     */
    virtual bool isValuePreRead() {return(isValue());}

    /*!
     * \ingroup	Control
     * \brief	Checks without reading the whole file if the object is a Contour
     * \return	true if is a 2D or 3D contour
     */
    virtual bool isContourPreRead() {return(isContour());}

    /*!
     * \ingroup	Control
     * \brief	Checks without reading the whole file if the object is
     * 		  a convex hull
     * \return	true if is a 2D or 3D convex hull
     */
    virtual bool isConvHullPreRead() {return(isConvHull());}

    /*!
     * \ingroup	Control
     * \brief	Checks without reading the whole file if the object is colour
     * \return	true if is a colour value object
     */
    virtual bool isColourPreRead() {return(isColour());}

    /*!
     * \ingroup	Control
     * \brief	Checkes if the object is  3D
     * \return	true if is a 3D and false if not
     */
    virtual bool is3D();

    /*!
     * \ingroup	Control
     * \brief	Checkes if the object is 2D
     * \return	true if is a 2D and false if not
     */
    virtual bool is2D();

    /*!
     * \ingroup	Control
     * \brief	Checks without reading the whole file if the object is 3D
     * \return	true if is a 3D and false if not
     */
    virtual bool is3DPreRead() {return(is3D());}

    /*!
     * \ingroup	Control
     * \brief	Checks without reading the whole file if the object is 2D
     * \return	true if is a 2D and false if not
     */
    virtual bool is2DPreRead() {return(is2D());}

    /*!
     * \ingroup	Control
     * \brief	Checkes if the object is set
     * \return	true if the object is empty, false if is set
     */
    virtual bool isEmpty() {return(m_obj == NULL);}

    /*!
     * \ingroup	Control
     * \brief	Checks if the object was loaded form a file.
     * \return	true if the object was loaded from a file, false if it was generated
     */
    virtual bool isFile();

    /*!
     * \ingroup	Control
     * \brief	Return the colour of the object as a SbColor inventor object.
     * \return	object colour
     */
    virtual SbColor sbColour();

    /*!
     * \ingroup	Control
     * \brief	Return the colour of the object as a QColor Qt object.
     * \return	object colour
     */
    virtual QColor qColour();

    /*!
     * \ingroup	Control
     * \brief	Sets the colour of the object.
     *
     *               Emits the objectVisualisationChange() signal
     * \param	newColor			new object colour
     */
    void setQColour(const QColor newColor);

    /*!
     * \ingroup	Control
     * \brief	Return the transfer function if 3D volumte, otherwise null
     * \return	transfer function
     */
    virtual TransferFunction * transferFunction() {return m_transferFunction;}

    /*!
     * \ingroup	Control
     * \brief	Return the ID of the object. -1 if object is temporary
     * 		(not inserted in the object list model)
     * \return	object ID
     */
    virtual int ID() {return m_ID;}

    /*!
     * \ingroup	Control
     * \brief	Sets the ID of the object.
     * \param	ID			new object ID
     */
    virtual void setID(const int ID) { m_ID=ID;}

    /*!
     * \ingroup	Control
     * \brief	Sets a visualisation
     * \param	newVis			visualisation
     * \param	newScale		scale of the object

     */
    virtual void setVisualisation(SoGroup * newVis, int newScale=1);

    /*!
     * \ingroup	Control
     * \brief	Returns if cached visualisation is valid
     * \return	true if valid, false otherwise
     */
    virtual bool isCachedVisualisation() {return m_validVisualisation;}

    /*!
     * \ingroup	Control
     * \brief	Removes cached visualisation
     */
    virtual void removeCachedVisualisation();

    /*!
     * \ingroup	Control
     * \brief	Returns cached visualisation
     * \return	cached visualisation, can be NULL if not allocated yet
     */
    virtual SoGroup* cachedVisualisation() {return m_cachedVisualisation;}

    /*!
     * \ingroup	Control
     * \brief	Returns cached visualisation scale
     * \return	cached visualisation scale
     */
    virtual int cachedVisualisationScale() {return m_cachedVisualisationScale;}


    /*!
     * \ingroup	Control
     * \brief	Returns if the object is warped.
     * \return	false
     */
    virtual bool isWarped() {return false;}

    /*!
     * \ingroup	Control
     * \brief	Checks without reading the whole file if the object is Warped
     * \return	false
     */
    virtual bool isWarpedPreRead() {return isWarped();}

    /*!
     * \ingroup	Control
     * \brief	Checkes if the object is a source object
     * \return	true if is source
     */
    virtual bool isSource() {return((m_type & source) !=0);}

    /*!
     * \ingroup	Control
     * \brief	Checkes if the object is a target object
     * \return	true if is target
     */
    virtual bool isTarget() { return(m_type & target);}

    /*!
     * \ingroup	Control
     * \brief	Changes the type of the object
     * \param	type			new type
     *
     */
    virtual void changeWoolzObjectType (WoolzObjectType type);

    /*!
     * \ingroup	Control
     * \brief	Generates a new colour for the object
     */
    virtual void generateNewColour();

    /*!
     * \ingroup	Control
     * \brief	Select the object
     */
    virtual void selectObject() {emit objectSelected();}

    /*!
     * \ingroup	Control
     * \brief	Sets up connections to target
     * \param	target			connection target
     */
    virtual void setupConnections(QObject *target);

    /*!
     * \ingroup	Control
     * \brief	Reads object from DOM tree
     * \param	element			current element of the DOM tree
     * \return	true if succeded, false if not
     */
    virtual bool parseDOM(const QDomElement &element);

    /*!
     * \ingroup	Control
     * \brief	Reads current token if known
     * \param	element			current element of the DOM tree
     * \return	true if succeded, false if not
     */
    virtual bool parseDOMLine(const QDomElement &element);

    /*!
     * \ingroup	Control
     * \brief	Checks if object parameters changes requireing update
     * \return	true if object needs update
     */
    virtual bool needsUpdate() {return false;}

    public slots:
      /*!
       * \ingroup	Control
       * \brief	Updates object if auto update is on or if forced.
       *
       *        Uses doUpdate() for updateing.
       * \param	force			forces update, even if auto update
       * 				is false
       */
      virtual void update ( bool force = false) = 0;

    /*!
     * \ingroup	Control
     * \brief	Processes mess element removal
     * \param	point			coordinate of mesh element to be
     * 					removed
     */
    virtual void removeMeshElement(const WlzDVertex3 point);

  protected:
    /*!
     * \ingroup	Control
     * \brief	Returns the next available colour for visualising objects.
     * \return	colour
     */
    virtual QColor getNextColour();

    /*!
     * \ingroup	Control
     * \brief	Replaces a previous object with newly generated object.
     *
     *               Must be implemented in subclasses.
     */
    virtual void doUpdate () =0;

    /*!
     * \ingroup	Control
     * \brief	Initialises transfer function
     * \param	forces			the creation of the transfer function,
     *               			used if object type is not know, but
     *               			transfer function is being read from
     *               			a file
     */
    virtual void setupTransferFunction(bool force = false);

    /*!
     * \ingroup	Control
     * \brief	Replaces the transfer function
     * \param	tf			new transfer function
     */
    void replaceTransferFunction(TransferFunction *tf);

signals:
    /*!
     * \ingroup	Control
     * \brief	Signals when an mesh element is removed.
     */
    void removeMeshElement(int);

    /*!
     * \ingroup	Control
     * \brief	Signals when an object (non visual) property changes.
     */
    void objectPropertyChanged();

    /*!
     * \ingroup	Control
     * \brief	Signals when an object changes and visualisation must
     * 		be updated.
     */
    void objectVisualisationChange();

    /*!
     * \ingroup	Control
     * \brief	Signals when an object vsibility was changed
     */
    void objectVisibilityChanged();

    /*!
     * \ingroup	Control
     * \brief	Signals before an object is removed.
     */
    void objectRemoved();

    /*!
     * \ingroup	Control
     * \brief	Signals when an object is changed. \todo  same as
     *          visualisation changed?
     */
    void objectChanged();

    /*!
     * \ingroup	Control
     * \brief	Signals when an object type is changed.
     */
    void objectTypeChanged();

    /*!
     * \ingroup	Control
     * \brief	Signals when an object is selected.
     */
    void objectSelected();

    /*!
     * \ingroup	Control
     * \brief	Signals the status bar update request
     * \param	message			new message
     * \param	timeout			timeout of the new message
     */
    void statusChange(QString message, int timeout);

    /*!
     * \ingroup	Control
     * \brief	Signals when an object is updatable state changes
     */
    void updated(bool);

  protected:
    // protected attributes
    WlzObject * m_obj;                	/*!< stored Woolz object*/
    QString m_name;                   	/*!< name of the object*/
    QColor m_colour;                  	/*!< colour of the object*/
    WoolzObjectType m_type;           	/*!< type of the object*/
    bool m_removable;           	/*!< removable flag: if true object
    					     can be removed, otherwise it can
					     be only replaced*/
    SoGroup *m_cachedVisualisation;   	/*!< cached visualisation of the
    					     object to share between multiple
					     views */
    bool m_validVisualisation;        	/*!< if visualisation is valid */
    bool m_visible;                   	/*!< global flag if the object is
    					     visualised*/
    int m_cachedVisualisationScale;   	/*!< scale of the cached
    					     visualisation */
    int m_ID;                         	/*!< object ID to identify object
    					     troughout the application run-time
					     TODO REMOVE*/
    TransferFunction * m_transferFunction; /*!< transfer function */
  private:
    static unsigned colourCounter;   	/*!< colour counter */

};
#endif // WOOLZOBJECT_H
