#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _View_h[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         View.h
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
* \brief	General interface for view based on Open Inventor.
* \ingroup	Views
*/

#ifndef VIEW_H
#define VIEW_H

#include <QString>
#include <QObject>

class SoSeparator;
class SoSwitch;

class ObjectViewer;
class QStringList;
class SoNode;

#include <QtXml/QXmlStreamWriter>
#include <QtXml/QDomElement>


class QXmlStreamWriter;
class QDomElement;

/*!
* \brief	Interface class for views based on Open Inventor.
* \ingroup	Views
*/
class View: public QObject
{
  Q_OBJECT

  public:
   /*!
    * \ingroup	Views
    * \brief	Constructor
    */
    View(QObject *parent);

   /*!
    * \ingroup	Views
    * \brief	Destructor
    * \return	void
    */
    virtual ~View();

   /*!
    * \return	scene graph root
    * \ingroup	Visualisation
    * \brief	Builds the scene graph of the view.
    *
    *		Returns empty scene graph
    * \param	bForce			if true force update of the scene
    * 					graph elements
    */
    virtual SoNode* getSceneGraph(bool bForce = false) ;

   /*!
    * \return	the number of available visualisations
    * \ingroup	Visualisation
    * \brief	Returns the number of available visualisations.
    */
    virtual int visualisationType() = 0;

   /*!
    * \ingroup	Visualisation
    * \brief	Sets the transparency value of the view.
    * \param	transparency		new transparency value between 0
    * 					(non transparent) and 100 (transparent)
    */
    virtual void setTransparency(int transparency)
    {
      m_transparency = transparency;
      emit viewPropertyChanged();
    }

   /*!
    * \ingroup	Visualisation
    * \brief	Returns the transparency value of the view.
    * \return	transparency value
    */
    virtual int getTransparency() {return m_transparency;}

   /*!
    * \return	The number of available visualisations.
    * \ingroup	Visualisation
    * \brief	In derived classes returns the number of available
    * 		visualisations.
    */
    virtual int getVisualisationTypesNo()  = 0;

   /*!
    * \return	list of visualistion type names.
    * \ingroup	Visualisation
    * \brief	In derived classes returns the list of available
    * 		visualisations.
    */
    virtual QStringList getVisualisationTypes () = 0;

   /*!
    * \return	list of visualistion type names.
    * \ingroup	Visualisation
    * \brief	In derived classes changes the current visualisation.
    *
    *		In View has empty implementation.
    */
    virtual void setVisualisationType(int /*type*/) {}

   /*!
    * \return	true of view is visible, false if not.
    * \ingroup	Visualisation
    * \brief	Returns the visibility of the view.
    */
    virtual bool getVisibility() {return m_visible;}

   /*!
    * \ingroup	Visualisation
    * \brief	Sets visibility of the view.
    * \param	visibility		new visibility value
    */
    virtual void setVisibility(bool visibility);

   /*!
    * \return	"No properties".
    * \ingroup	Visualisation
    * \brief	Returns the properties of the view. Should be overiden in
    *     	derived classes.
    */
    virtual QString getProperties() {return tr("No properties");}

   /*!
    * \return	true if succeded, false if not
    * \ingroup	Control
    * \brief	Saves model in xml format.
    * \param	xmlWriter		output xml stream
    */
    virtual bool saveAsXml(QXmlStreamWriter *xmlWriter);

   /*!
    * \return	true if succeded, false if not
    * \ingroup	Control
    * \brief	Reads view in from DOM tree
    * \param	element			current element of the DOM tree
    */
    virtual bool parseDOM(const QDomElement& element);

  protected:
   /*!
    * \ingroup	Visualisation
    * \brief	Views add here their specific children
    * \param	bForce			force object regeneration
    */
    virtual void generateSceneGraph(bool bForce = false) = 0;

  signals:
   /*!
    * \ingroup	Visualisation
    * \brief	Signals property change of the view.
    * \return	void
    */
    void viewPropertyChanged();

  protected:
   /*!
    * \return	true if succeded, false if not
    * \ingroup	Control
    * \brief	Saves view details in xml format.
    * \param	xmlWriter		output xml stream
    */
    virtual bool saveAsXmlProperties(QXmlStreamWriter *xmlWriter) ;

   /*!
    * \return	true if succeded, false if not
    * \ingroup	Control
    * \brief	Reads current token if known
    * \param	element			current element of the DOM tree
    */
    virtual bool parseDOMLine(const QDomElement &element);

  protected:
    // protected attributes
    SoSeparator *root;		  	/*!< root of the scene graph where 
					       the ellemets are added */
    SoSwitch *switchroot;	  	/*!< real root of the view, allowing
					       on/off switch*/

    int m_transparency;		  	/*!< transparency of the view. 0 if
					       non transparent, 100 if
					       transparent */
    bool m_visible;		  	/*!< visibility of the view */

  public:
    static const char * xmlTag;	  	/*!< xml section tag string */
};

#endif // VIEW_H
