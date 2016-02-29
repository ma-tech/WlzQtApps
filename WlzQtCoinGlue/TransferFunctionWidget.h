#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _TransferFunctionWidget_h[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         TransferFunctionWidget.h
* \author       Zsolt Husz, Bill Hill
* \date         July 2009
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
* \brief	Widget for manipulating transfer functions of volume objects
* \ingroup	UI
*
*/
#ifndef TRANSFERFUNCTIONWIDGET_H
#define TRANSFERFUNCTIONWIDGET_H

#include <QDockWidget>
#include <Inventor/fields/SoMFFloat.h>
#include <ui_TransferFunctionWidget.h>

class ObjectListModelAbstract;
class WoolzObject;
class TransferFunction;
class QObject;
class QFunctionEditor;
class QUndoCommand;
class QDomElement;

#include <QtXml/QXmlStreamReader>

/*!
 * \brief	Widget class for manipulating transfer functions of volume
 * 		objects
 * \ingroup	UI
 */
class TransferFunctionWidget:
public QDockWidget, public Ui::
TransferFunctionWidget
{
  Q_OBJECT
  public:
   /*!
    * \ingroup	UI
    * \brief	Constructor
    * \param	parent			parent widget
    * \param    objectListModel 	to manage objects
    */
    TransferFunctionWidget(QWidget *parent,
    			   ObjectListModelAbstract *objectListModel);

   /*!
    * \ingroup	UI
    * \brief	Destructor
    */
    ~TransferFunctionWidget();

  private slots:
   /*!
    * \ingroup	UI
    * \brief	Processes low cut off change
    */
    void lowCutOffChanged();

   /*!
    * \ingroup	UI
    * \brief	Processes low cut off change
    */
    void highCutOffChanged();

    /*!
     * \ingroup	UI
     * \brief	Creates a gamma curve between the low and high tresholds
     */
     void setGamma();

    /*!
     * \ingroup	UI
     * \brief	Inverts curve between the low and high tresholds
     */
     void invert();

    /*!
     * \ingroup	UI
     * \brief	Histogram scale selection event handler
     */
     void scaleSelected(int);

   /*!
    * \ingroup	UI
    * \brief	Processes object removal.
    */
    void removedObjectSignal(WoolzObject* obj);

   /*!
    * \ingroup	UI
    * \brief	Processes object addition.
    */
    void addObjectSignal(WoolzObject* obj);

   /*!
    * \ingroup	UI
    * \brief	Processes external object selection.
    * \param	object			new selected object
    */
    void objectSelected(WoolzObject* object);

   /*!
    * \ingroup	UI
    * \brief	Processes external object property change.
    */
    void objectPropertyChanged();

   /*!
    * \ingroup	UI
    * \brief	Processes red, gren, blue and alpha channel selection change
    */
    void selectChannels(bool);

   /*!
    * \ingroup	UI
    * \brief	Processes luminance group selection
    * \param	enabled			true if selected
    */
    void selectLuminance(bool enabled);

   /*!
    * \ingroup	UI
    * \brief	Processes luminance group selection
    * \param	enabled			true if selected
    */
    void selectIntensity(bool enabled);

    /*!
    * \ingroup	UI
    * \brief	Processes alpha group selection
    * \param	enabled			true if selected
    */
    void selectAlpha(bool enabled);

    /*!
    * \ingroup	UI
    * \brief	Processes load button press
    */
    void load(bool);

   /*!
    * \ingroup	UI
    * \brief	Processes save button press
    */
    void save(bool);

   /*!
    * \ingroup	UI
    * \brief	Processes function editor widget change
    */
    void tableChanged();

  private:
   /*!
    * \ingroup	UI
    * \brief	Fills widget with the values the transfer function of the
    * 		object.
    * \param	object			object to be loaded
    */
    void loadProperties(WoolzObject* object);

   /*!
    * \ingroup	UI
    * \brief	Updates group selection to match channel selection
    */
    void updateGroups();

   /*!
    * \ingroup	UI
    * \brief	Recomputes the histogram of the object.
    *           Note: The histogram currently used is a log histogram.
    */
    void recomputeHistogram();

   /*!
    * \ingroup	UI
    * \brief	Loads the transfer function from the XML streams and generates
    * 		commands for loading.
    * \param    xmlReader 		XML stream to read
    */
    bool loadFromDOM(const QDomElement &element);

  protected:
    ObjectListModelAbstract* m_objectListModel; /*!< the object list model*/
    WoolzObject * m_object;       	/*!< current object*/
    QFunctionEditor *m_functionEditor;  /*!< widget for function editing*/
    SoMFFloat m_colorMap;               /*!< color map edited by the widget*/
    float *m_pHist;                     /*!< histogram of the object*/
    bool inUpdate;                      /*!< in apply process, do not reload
                                             object properties*/
};

#endif  //TRANSFERFUNCTIONWIDGET_H
