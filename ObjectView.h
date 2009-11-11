#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _ObjectView_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         ObjectView.h
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
* \brief        View for generic Woolz object
* \ingroup      Views
*
*/

#ifndef OBJECTVIEW_H
#define OBJECTVIEW_H

#include "View.h"
#include <QColor>

class WoolzObject;
class QXmlStreamWriter;

/*!
* \brief	Generic interface for views of Woolz objects
* \ingroup      Views
*/
class ObjectView : public View
{
  Q_OBJECT

public:
 /*!
  * \ingroup      Views
  * \brief        Constructor
  * \param        parent parent viewer
  * \param        object Woolz object to be visualised
  * \return       void
  * \par      Source:
  *                ObjectView.cpp
  */
  ObjectView ( QObject * parent, WoolzObject *  obj);

 /*!
  * \ingroup      Views
  * \brief        Destructor
  * \return       void
  * \par      Source:
  *                ObjectView.cpp
  */
  virtual ~ObjectView ( );

 /*!
  * \ingroup      Views
  * \brief        Checks if view is using the given Woolz object
  * \param        testobj Woolz object 
  * \return       true if View uses testobj, false otherwise
  * \par      Source:
  *                ObjectView.cpp
  */
  bool isUsing ( WoolzObject * testobj ) {return obj==testobj;}

 /*!
  * \ingroup      Views
  * \brief        Checks if view is using the given Woolz object
  * \param        testobjID Woolz object ID
  * \return       true if View uses testobj, false otherwise
  * \par      Source:
  *                ObjectView.cpp
  */
  bool isUsing ( int testobjID );

 /*!
  * \ingroup      Views
  * \brief        Returns the colour of the view
  * \return       object colour
  * \par      Source:
  *                ObjectView.cpp
  */
  virtual QColor getQColour ( );

 /*!
  * \ingroup      Views
  * \brief        Returns the title (name) of the view
  * \return       object title
  * \par      Source:
  *                ObjectView.cpp
  */
  virtual QString getTitle ( );

 /*!
  * \ingroup      Views
  * \brief        Returns the properties of the view
  * \return       view properties
  * \par      Source:
  *                ObjectView.cpp
  */
  virtual QString getProperties ( );

 /*!
  * \ingroup      Views
  * \brief        Returns if the object has to be added to the head of the scene graph.
  * \return       true, if the view must be added in front of other views in the scene graph
  * \par      Source:
  *                ObjectView.cpp
  */
  virtual bool addAsFirst ( ) {return false;}

 /*!
  * \ingroup      Visualisation
  * \brief        Returns the parent viewer of the view.
  *
  * \return       Parent viewer.
  * \par      Source:
  *                ObjectView.cpp
  */
  QObject * getParent ( )   {
    return m_parent;
  }

 /*!
  * \ingroup      Visualisation
  * \brief        Returns the object viewed.
  *
  * \return       Parent viewer.
  * \par      Source:
  *                ObjectView.cpp
  */
  WoolzObject * object ( )   {
    return obj;
  }

 /*!
  * \ingroup      Visualisation
  * \brief        Sets visibility of the view.
  *
  * \par          Reimplemented from View, set the visibility using also the object visibility
  * \param        visibility new visibility value
  * \return       void
  * \par      Source:
  *                ObjectView.cpp
  */
  virtual void setVisibility ( bool visibility );

 /*!
  * \ingroup      Visualisation
  * \brief        Returns the visibility of the view.
  *
  * \par          Reimplemented from View, considers also the object visibility 
  * \return       true of view is visible, false if not.
  * \par      Source:
  *                View.cpp
  */
  virtual bool getVisibility ( )   { return m_maskVisibility; }

 /*!
  * \ingroup      Visualisation
  * \brief        Returns if the current object is compatible with the view
  *
  * \par          Will be reimplemented in each of the subclasses
  * \return       true of object is compatible, false if not.
  * \par      Source:
  *                View.cpp
  */
  virtual bool compatible( ) = 0;

public slots:

 /*!
  * \ingroup      Views
  * \brief        Called after Woolz object was changed, but no visual view update is necessary.
  * \return       void
  * \par      Source:
  *                ObjectView.cpp
  */
  void objectPropertyChanged( );

 /*!
  * \ingroup      Views
  * \brief        Called after Woolz object visibility was changed
  * \return       void
  * \par      Source:
  *                ObjectView.cpp
  */
  void objectVisibilityChanged ( );


 /*!
  * \ingroup      Views
  * \brief        Updates view after Woolz object changes.
  * \return       void
  * \par      Source:
  *                ObjectView.cpp
  */
  void objectChanged();

 /*!
  * \ingroup      Views
  * \brief        Updates view transparency 
  *
  *                to the new value (if object is generated or to its complement if is not
  *               generated.
  * \return       void
  * \par      Source:
  *                ObjectView.cpp
  */
  void updateTransparency(int transparency);

 /*!
  * \ingroup      Views
  * \brief        Selects the view
  *
  * \return       void
  * \par      Source:
  *                ObjectView.cpp
  */
  void selectView();

signals:
 /*!
  * \ingroup      Views
  * \brief        Signals view type change.
  * \return       void
  * \par      Source:
  *                ObjectView.cpp
  */
  void regerateView();

protected:
 /*!
  * \ingroup      Control
  * \brief        Saves view details in xml format.
  * \param        xmlWriter output xml stream
  * \return       true if succeded, false if not
  * \par      Source:
  *                ObjectView.cpp
  */
  virtual bool saveAsXmlProperties(QXmlStreamWriter *xmlWriter) ;

protected:
  WoolzObject *  obj;         /*!< Woolz object being visualised  */
  QObject * m_parent;         /*!< parent viewer */
  bool m_maskVisibility;      /*!< masking visibility. this is the view's original visibility,
                                    however the object visibility mask it in this class  */
};
#endif // OBJECTVIEW_H
