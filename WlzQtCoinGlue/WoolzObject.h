#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _WoolzObject_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         WoolzObject.h
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
 License along with this program; if not, write to the Free
* Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
* Boston, MA  02110-1301, USA.
* \brief        Container class for WlzObject type
* \ingroup      Control
*
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
* \ingroup      Control
*/
class WoolzObject : public QObject
{
  Q_OBJECT
public:

 /*!
  * \brief	Object type definition
  */
  typedef enum {none = 0, source = 1, target = 2, generated = 4} WoolzObjectType;

 /*!
  * \ingroup      Control
  * \brief        Constructor
  * \return       void
  * \par      Source:
  *                WoolzObject.cpp
  */
  WoolzObject ( );

 /*!
  * \ingroup      Control
  * \brief        Destructor
  * \return       void
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual ~WoolzObject ( );

 /*!
  * \ingroup      Control
  * \brief        Saves the stored WlzObject into a file
  * \param        filename filename to write the object to
  * \return       true if operation succeded, false otherwise
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual bool saveAs( QString filename);

 /*!
  * \ingroup      Control
  * \brief        Saves object in xml format.
  * \param        xmlWriter output xml stream
  * \return       true if succeded, false if not
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual bool saveAsXml(QXmlStreamWriter */*xmlWriter*/) { return true;}

 /*!
  * \ingroup      Control
  * \brief        Saves object details in xml format.
  * \param        xmlWriter output xml stream
  * \return       true if succeded, false if not
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual bool saveAsXmlProperties(QXmlStreamWriter *xmlWriter);

 /*!
  * \ingroup      Control
  * \brief        Deallocates the stored object
  * \return       void 
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual void close ( );

public:
  // attribute accessor methods
  //
 /*!
  * \ingroup      Control
  * \brief        Returns the stored WlzObject
  * \return       WlzObject stored
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual WlzObject * getObj ( ) {
    if (!m_obj)     //if no object is available try update
        update(true);
    return m_obj;
  }

 /*!
  * \ingroup      Control
  * \brief        Sets the removable flag.
  * \param        newRemovable if the object is removable
  * \return       void
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual void setRemovable ( bool newRemovable)   {
        m_removable = newRemovable;
  }

 /*!
  * \ingroup      Control
  * \brief        Returns the removable flag
  * \return       true if removable, false otherwise
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual bool removable ( )   {
    return m_removable;
  }

 /*!
  * \ingroup      Control
  * \brief        Sets the visible flag.
  * \param        visible if the object is visible
  * \return       void
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual void setVisible ( bool visible);

 /*!
  * \ingroup      Control
  * \brief        Returns the visible flag
  * \return       true if removable, false otherwise
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual bool visible ( )   {
    return m_visible;
  }

 /*!
  * \ingroup      Control
  * \brief        Sets the name of the object.
  * \param        Removable if the object is removable
  * \return       void
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual void setName ( QString  newName )   {
      if (newName != m_name) {
        m_name = newName;
        emit objectPropertyChanged();
      }
  }

 /*!
  * \ingroup      Control
  * \brief        Returns the name of the object
  * \return       object name
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual QString name ( )   {
    return m_name;
  }

 /*!
  * \ingroup      Control
  * \brief        Returns notes of the object.
  *
  *               Must be reimplemented in subclasses
  * \return       Notes (parameters of the object)
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual QString notes ( ) {return "";}

 /*!
  * \ingroup      Control
  * \brief        Returns the type of the object.
  * \return       Object type
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual WoolzObjectType type ( )   {
    return m_type;
  }

 /*!
  * \ingroup      Control
  * \brief        Checkes if the object is a Mesh
  * \return       true if is a 2D or 3D mesh
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual bool isMesh ( ) ;

 /*!
  * \ingroup      Control
  * \brief        Checkes if the object is a contour
  * \return       true if is a 2D or 3D contour
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual bool isContour ( ) ;

 /*!
  * \ingroup      Control
  * \brief        Checkes if the object is a value object
  * \return       true if is a value object
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual bool isValue ( ) ;

 /*!
  * \ingroup      Control
  * \brief        Checkes if the object is a value object and has values
  * \return       true if is a value object with values set
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual bool isValueSet ( ) ;

 /*!
  * \ingroup      Control
  * \brief        Checkes if the object is a colour value object
  * \return       true if is a colour value object 
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual bool isColour ( );

  /*!
   * \ingroup      Control
   * \brief        Returns the grey type of the object
   * \return       grey type or WLZ_GREY_ERROR on error
   * \par      Source:
   *                WoolzObject.cpp
   */
   WlzGreyType getWoolzGreyType();

 /*!
  * \ingroup      Control
  * \brief        Checks without reading the whole file if the object is a Mesh
  * \return       true if is a 2D or 3D mesh
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual bool isMeshPreRead () { return isMesh();}

 /*!
  * \ingroup      Control
  * \brief        Checks without reading the whole file if the object is a Value object
  * \return       true if is a 2D or 3D value object
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual bool isValuePreRead () { return isValue(); }

 /*!
  * \ingroup      Control
  * \brief        Checks without reading the whole file if the object is a Contour
  * \return       true if is a 2D or 3D contour
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual bool isContourPreRead () { return isColour(); }

 /*!
  * \ingroup      Control
  * \brief        Checks without reading the whole file if the object is colour
  * \return       true if is a colour value object
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual bool isColourPreRead () { return isColour(); }

  /*!
  * \ingroup      Control
  * \brief        Checkes if the object is  3D
  * \return       true if is a 3D and false if not
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual bool is3D ( ) ;

 /*!
  * \ingroup      Control
  * \brief        Checkes if the object is 2D
  * \return       true if is a 2D and false if not
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual bool is2D ( ) ;

 /*!
  * \ingroup      Control
  * \brief        Checks without reading the whole file if the object is 3D
  * \return       true if is a 3D and false if not
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual bool is3DPreRead ( ) { return is3D();}

  /*!
  * \ingroup      Control
  * \brief        Checks without reading the whole file if the object is 2D
  * \return       true if is a 2D and false if not
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual bool is2DPreRead ( ) { return is2D();}

 /*!
  * \ingroup      Control
  * \brief        Checkes if the object is set
  * \return       true if the object is empty, false if is set
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual bool isEmpty ( ) {return m_obj==NULL;}

 /*!
  * \ingroup      Control
  * \brief        Checks if the object was loaded form a file.
  * \return       true if the object was loaded from a file, false if it was generated
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual bool isFile ( );

 /*!
  * \ingroup      Control
  * \brief        Return the colour of the object as a SbColor inventor object.
  * \return       object colour
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual SbColor sbColour();

 /*!
  * \ingroup      Control
  * \brief        Return the colour of the object as a QColor Qt object.
  * \return       object colour
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual QColor qColour();

 /*!
  * \ingroup      Control
  * \brief        Sets the colour of the object.
  *
  *               Emits the objectVisualisationChange() signal
  * \param        newColor new object colour
  * \return       void
  * \par      Source:
  *                WoolzObject.cpp
  */
  void setQColour(const QColor newColor);

 /*!
  * \ingroup      Control
  * \brief        Return the transfer function if 3D volumte, otherwise null
  * \return       transfer function
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual TransferFunction * transferFunction() {return m_transferFunction;}

 /*!
  * \ingroup      Control
  * \brief        Return the ID of the object. -1 if object is temporary (not inserted in the object list model)
  * \return       object ID
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual int ID() {return m_ID;}

 /*!
  * \ingroup      Control
  * \brief        Sets the ID of the object.
  * \param        ID new object ID
  * \return       void
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual void setID(const int ID) { m_ID=ID;}

 /*!
  * \ingroup      Control
  * \brief        Sets a visualisation
  * \param        newVis visualisation
  * \param        newScale scale of the object

  * \return       void
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual void setVisualisation(SoGroup * newVis, int newScale=1);

 /*!
  * \ingroup      Control
  * \brief        Returns if cached visualisation is valid
  * \return       true if valid, false otherwise
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual bool isCachedVisualisation() { return m_validVisualisation;}

 /*!
  * \ingroup      Control
  * \brief        Removes cached visualisation
  * \return       void
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual void removeCachedVisualisation();

 /*!
  * \ingroup      Control
  * \brief        Returns cached visualisation
  * \return       cached visualisation, can be NULL if not allocated yet
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual SoGroup* cachedVisualisation() { return m_cachedVisualisation;}

  /*!
   * \ingroup      Control
   * \brief        Returns cached visualisation scale
   * \return       cached visualisation scale
   * \par      Source:
   *                WoolzObject.cpp
   */
   virtual int cachedVisualisationScale() { return m_cachedVisualisationScale;}


 /*!
  * \ingroup      Control
  * \brief        Returns if the object is warped.
  * \return       false
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual bool isWarped() {return false;}

 /*!
  * \ingroup      Control
  * \brief        Checks without reading the whole file if the object is Warped
  * \return       false
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual bool isWarpedPreRead() {return isWarped();}

 /*!
  * \ingroup      Control
  * \brief        Checkes if the object is a source object
  * \return       true if is source
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual bool isSource( ) { return (m_type & source) !=0 ;}

 /*!
  * \ingroup      Control
  * \brief        Checkes if the object is a target object
  * \return       true if is target
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual bool isTarget( ) { return m_type & target;}

 /*!
  * \ingroup      Control
  * \brief        Changes the type of the object
  * \param        type new type
  *
  * \return       void
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual void changeWoolzObjectType (WoolzObjectType type);

 /*!
  * \ingroup      Control
  * \brief        Generates a new colour for the object
  * \return       void
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual void generateNewColour();

 /*!
  * \ingroup      Control
  * \brief        Select the object
  * \return       void
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual void selectObject() { emit objectSelected(); }

 /*!
  * \ingroup      Control
  * \brief        Sets up connections to target
  * \param        target connection target
  * \return       void
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual void setupConnections(QObject *target);

 /*!
  * \ingroup      Control
  * \brief        Reads object from DOM tree
  * \param        element current element of the DOM tree
  * \return       true if succeded, false if not
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual bool parseDOM(const QDomElement &element);

 /*!
  * \ingroup      Control
  * \brief        Reads current token if known
  * \param        element current element of the DOM tree
  * \return       true if succeded, false if not
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual bool parseDOMLine(const QDomElement &element);

  /*!
   * \ingroup      Control
   * \brief        Checks if object parameters changes requireing update
   * \return       true if object needs update
   * \par      Source:
   *                WoolzObject.cpp
   */
  virtual bool needsUpdate() {return false;}

public slots:
 /*!
  * \ingroup      Control
  * \brief        Updates object if auto update is on or if forced.
  *
  *               Uses doUpdate() for updateing.
  * \param        force forces update, even if auto update is false
  * \return       void
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual void update ( bool force = false) = 0;

 /*!
  * \ingroup      Control
  * \brief        Processes mess element removal
  * \param        point coordinate of mesh element to be removed
  * \return       void
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual void removeMeshElement(const WlzDVertex3 point);

protected:
 /*!
  * \ingroup      Control
  * \brief        Returns the next available colour for visualising objects.
  * \return       colour
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual QColor getNextColour();

 /*!
  * \ingroup      Control
  * \brief        Replaces a previous object with newly generated object.
  *
  *               Must be implemented in subclasses.
  * \return       void
  * \par      Source:
  *                WoolzDynObject.cpp
  */
  virtual void doUpdate () =0;

 /*!
  * \ingroup      Control
  * \brief        Initialises transfer function
  * \param        forces the creation of the transfer function,
  *               used if object type is not know, but transfer
  *               function is being read from a file
  * \return       void
  * \par      Source:
  *                WoolzObject.cpp
  */
  virtual void setupTransferFunction(bool force = false);

  /*!
   * \ingroup      Control
   * \brief        Replaces the transfer function
   * \param        tf new transfer function
   * \return       void
   * \par      Source:
   *                WoolzObject.cpp
   */
   void replaceTransferFunction(TransferFunction *tf);

signals:
 /*!
  * \ingroup      Control
  * \brief        Signals when an mesh element is removed.
  * \return       void
  * \par      Source:
  *                WoolzObject.cpp
  */
  void removeMeshElement(int);

 /*!
  * \ingroup      Control
  * \brief        Signals when an object (non visual) property changes.
  * \return       void
  * \par      Source:
  *                WoolzObject.cpp
  */
  void objectPropertyChanged();

 /*!
  * \ingroup      Control
  * \brief        Signals when an object changes and visualisation must be updated.
  * \return       void
  * \par      Source:
  *                WoolzObject.cpp
  */
   void objectVisualisationChange();

 /*!
  * \ingroup      Control
  * \brief        Signals when an object vsibility was changed
  * \return       void
  * \par      Source:
  *                WoolzObject.cpp
  */
  void objectVisibilityChanged();

 /*!
  * \ingroup      Control
  * \brief        Signals before an object is removed.
  * \return       void
  * \par      Source:
  *                WoolzObject.cpp
  */
  void objectRemoved();

 /*!
  * \ingroup      Control
  * \brief        Signals when an object is changed. \todo  same as visualisation changed?
  * \return       void
  * \par      Source:
  *                WoolzObject.cpp
  */
  void objectChanged();

 /*!
  * \ingroup      Control
  * \brief        Signals when an object type is changed.
  * \return       void
  * \par      Source:
  *                WoolzObject.cpp
  */
  void objectTypeChanged();

 /*!
  * \ingroup      Control
  * \brief        Signals when an object is selected.
  * \return       void
  * \par      Source:
  *                WoolzObject.cpp
  */
  void objectSelected();

  /*!
   * \ingroup      Control
   * \brief        Signals the status bar update request
   * \param        message new message
   * \param        timeout timeout of the new message
   * \return       void
   * \par      Source:
   *                WoolzObject.cpp
   */
   void statusChange(QString message, int timeout);

 /*!
  * \ingroup      Control
  * \brief        Signals when an object is updatable state changes
  * \return       void
  * \par      Source:
  *                WoolzObject.cpp
  */
  void updated(bool);

protected:
  // protected attributes
  WlzObject * m_obj;                /*!< stored Woolz object*/
  QString m_name;                   /*!< name of the object*/
  QColor m_colour;                  /*!< colour of the object*/
  WoolzObjectType m_type;           /*!< type of the object*/
  bool m_removable;                 /*!< removable flag: if true object can be removed,
                                               otherwise it can be only replaced*/
  SoGroup *m_cachedVisualisation;   /*!< cached visualisation of the object to share between multiple views */
  bool m_validVisualisation;        /*!< if visualisation is valid */
  bool m_visible;                   /*!< global flag if the object is visualised*/
  int m_cachedVisualisationScale;   /*!< scale of the cached visualisation*/
  int m_ID;                         /*!< object ID to identify object troughout the application run-time TODO REMOVE*/
  TransferFunction * m_transferFunction;                  /*!< transfer function */
private:
  static unsigned colourCounter;                          /*!< colour counter */

};
#endif // WOOLZOBJECT_H
