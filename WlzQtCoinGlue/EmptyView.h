#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _EmptyView_h[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         EmptyView.h
* \author       Zsolt Husz
* \date         August 2009
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
* \brief	Empty visualisation for objects
* \ingroup      Views
*/

#ifndef EMPTYVIEW_H
#define EMPTYVIEW_H
#include "ObjectView.h"

#include <Wlz.h>

/*!
* \brief	View providing empty visualisation
* \ingroup	Views
*/
class EmptyView: public ObjectView
{
  Q_OBJECT
  public:
   /*!
    * \ingroup	Views
    * \brief	Constructor
    * \param	parent 			parent viewer
    * \param	object 			Woolz object to be visualised
    */
    EmptyView(QObject * parent, WoolzObject * object);

  public:
   /*!
    * \return	Return the number of available visualisations (1)
    * \ingroup	Visualisation
    * \brief	Returns the number of available visualisations that is 1
    * 		for EmptyView.
    *
    *	      	Implements View::getVisualisationTypesNo().
    */
    virtual int getVisualisationTypesNo() {return 1;}

   /*!
    * \return	List of visualistion type names.
    * \ingroup	Visualisation
    * \brief	Returns the list of available visualisations.
    *
    *	      	Implements View::getVisualisationTypes().
    */
    virtual QStringList getVisualisationTypes();

   /*!
    * \ingroup	Views
    * \brief	Current visualisation type.
    *
    *   	Currently only one visualisation is implemented, therefore
    *   	it returns 0.
    *
    *	      	Implements View::visualisationType().
    */
    virtual int visualisationType() {return 0;}

  protected:
   /*!
    * \ingroup	Visualisation
    * \brief	Builds the scene graph of the view.
    *
    *	      	Reimplements View::generateSceneGraph().
    * \param	bForce			if true force update of the scene
    * 					graph elements
    */
   virtual void generateSceneGraph(bool) {}

   /*!
    * \ingroup	Visualisation
    * \brief	Returns if the current object is compatible with the view
    * \return	true of object is compatible, false if not.
    */
    virtual bool compatible();
};

#endif // EMPTYVIEW_H
