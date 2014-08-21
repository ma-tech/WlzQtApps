#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _ObjectView_h[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         ObjectView.h
* \author	Zsolt Husz
* \date		October 2008
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
* \brief	View for generic Woolz object
* \ingroup	Views
*/

#ifndef OBJECTVIEW_H
#define OBJECTVIEW_H

#include "View.h"
#include <QColor>

class WoolzObject;
class QXmlStreamWriter;

/*!
* \brief	Generic interface for views of Woolz objects
* \ingroup	Views
*/
class ObjectView: public View
{
  Q_OBJECT

  public:
   /*!
    * \ingroup	Views
    * \brief	Constructor
    * \param	parent 			parent viewer
    * \param	object 			Woolz object to be visualised
    */
    ObjectView(QObject * parent, WoolzObject *	obj);

   /*!
    * \ingroup	Views
    * \brief	Destructor
    */
    virtual ~ObjectView();

   /*!
    * \return	true if View uses testobj, false otherwise
    * \ingroup	Views
    * \brief	Checks if view is using the given Woolz object
    * \param	testobj 		Woolz object 
    */
    bool isUsing(WoolzObject * testobj) {return(obj == testobj);}

   /*!
    * \return	true if View uses testobj, false otherwise
    * \ingroup	Views
    * \brief	Checks if view is using the given Woolz object
    * \param	testobjID		Woolz object ID
    */
    bool isUsing(int testobjID);

   /*!
    * \return	object colour
    * \ingroup	Views
    * \brief	Returns the colour of the view
    */
    virtual QColor getQColour();

   /*!
    * \return	object title
    * \ingroup	Views
    * \brief	Returns the title (name) of the view
    */
    virtual QString getTitle();

   /*!
    * \return	view properties
    * \ingroup	Views
    * \brief	Returns the properties of the view
    */
    virtual QString getProperties();

   /*!
    * \return	true, if the view must be added in front of other views
    * 		in the scene graph
    * \ingroup	Views
    * \brief	Returns if the object has to be added to the head of the
    * 		scene graph.
    */
    virtual bool addAsFirst() {return false;}

   /*!
    * \return	Parent viewer.
    * \ingroup	Visualisation
    * \brief	Returns the parent viewer of the view.
    */
    QObject * getParent() {return m_parent;}

   /*!
    * \return	Parent viewer.
    * \ingroup	Visualisation
    * \brief	Returns the object viewed.
    */
    WoolzObject * object() {return obj;}

   /*!
    * \ingroup	Visualisation
    * \brief	Sets visibility of the view.
    *
    * 		Reimplemented from View, set the visibility using also the
    * 		object visibility
    * \param	visibility 		new visibility value
    */
    virtual void setVisibility(bool visibility);

   /*!
    * \return	true of view is visible, false if not.
    * \ingroup	Visualisation
    * \brief	Returns the visibility of the view.
    *
    *    	Reimplemented from View, considers also the object visibility 
    */
    virtual bool getVisibility() {return m_maskVisibility;}

   /*!
    * \return	true of object is compatible, false if not.
    * \ingroup	Visualisation
    * \brief	Returns if the current object is compatible with the view
    *
    *    	Will be reimplemented in each of the subclasses
    */
    virtual bool compatible() = 0;

  public slots:

   /*!
    * \ingroup	Views
    * \brief	Called after Woolz object was changed, but no visual view
    * 		update is necessary.
    */
    void objectPropertyChanged();

   /*!
    * \ingroup	Views
    * \brief	Called after Woolz object visibility was changed
    */
    void objectVisibilityChanged();


   /*!
    * \ingroup	Views
    * \brief	Updates view after Woolz object changes.
    */
    void objectChanged();

   /*!
    * \ingroup	Views
    * \brief	Updates view transparency to the new value (if object is
    * 		generated or to its complement if is not generated.
    */
    void updateTransparency(int transparency);

   /*!
    * \ingroup	Views
    * \brief	Selects the view
    */
    void selectView();

  signals:
   /*!
    * \ingroup	Views
    * \brief	Signals view type change.
    */
    void regerateView();

  protected:
   /*!
    * \return	true if succeded, false if not
    * \ingroup	Control
    * \brief	Saves view details in xml format.
    * \param	xmlWriter 		output xml stream
    */
    virtual bool saveAsXmlProperties(QXmlStreamWriter *xmlWriter);

  protected:
    WoolzObject * obj;			/*!< Woolz object being visualised  */
    QObject * m_parent;	      		/*!< parent viewer */
    bool m_maskVisibility;      	/*!< masking visibility. this is the
    					     view's original visibility,
					     however the object visibility
					     mask it in this class */
};
#endif // OBJECTVIEW_H
