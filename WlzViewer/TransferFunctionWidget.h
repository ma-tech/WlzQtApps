#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _TransferFunctionWidget_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         TransferFunctionWidget.h
* \author       Zsolt Husz
* \date         July 2009
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
* \brief        Widget for manipulating transfer functions of volume objects
* \ingroup      UI
*
*/
#ifndef TRANSFERFUNCTIONWIDGET_H
#define TRANSFERFUNCTIONWIDGET_H

#include <QDockWidget>
#include <ui_TransferFunctionWidget.h>
#include <Inventor/fields/SoMFFloat.h>

class ObjectListModelAbstract;
class WoolzObject;
class TransferFunction;
class QObject;
class QFunctionEditor;

#include <QtXml/QXmlStreamReader>

/*!
 * \brief	Widget class for manipulating transfer functions of volume objects
 * \ingroup      UI
 */
class TransferFunctionWidget : public QDockWidget, public Ui::TransferFunctionWidget
{
Q_OBJECT
public:
 /*!
  * \ingroup      UI
  * \brief        Constructor
  * \param        parent parent widget
  * \param        objectListModel to manage objects
  * \par      Source:
  *                TransferFunctionWidget.cpp
  */
  TransferFunctionWidget(QWidget *parent, ObjectListModelAbstract *objectListModel);

 /*!
  * \ingroup      UI
  * \brief        Destructor
  * \par      Source:
  *                TransferFunctionWidget.cpp
  */
  ~TransferFunctionWidget();

private slots:
 /*!
  * \ingroup      UI
  * \brief        Processes low cut off change
  * \par      Source:
  *                TransferFunctionWidget.cpp
  */
  void lowCutOffChanged() ;

 /*!
  * \ingroup      UI
  * \brief        Processes low cut off change
  * \par      Source:
  *                TransferFunctionWidget.cpp
  */
  void highCutOffChanged() ;

 /*!
  * \ingroup      UI
  * \brief        Forces update of the object views
  * \par      Source:
  *                TransferFunctionWidget.cpp
  */
  void apply();

  /*!
   * \ingroup      UI
   * \brief        Creates a gama curve between the low and high tresholds
   * \par      Source:
   *                TransferFunctionWidget.cpp
   */
   void setGamma();

 /*!
  * \ingroup      UI
  * \brief        Processes object removal.
  * \return       void
  * \par      Source:
  *                TransferFunctionWidget.cpp
  */
  void removedObjectSignal(WoolzObject* obj);

 /*!
  * \ingroup      UI
  * \brief        Processes object addition.
  * \return       void
  * \par      Source:
  *                TransferFunctionWidget.cpp
  */
  void addObjectSignal(WoolzObject* obj);

 /*!
  * \ingroup      UI
  * \brief        Processes external object selection.
  * \param        object new selected object
  * \return       void
  * \par      Source:
  *                TransferFunctionWidget.cpp
  */
  void objectSelected(WoolzObject* object);

 /*!
  * \ingroup      UI
  * \brief        Processes external object property change.
  * \return       void
  * \par      Source:
  *                TransferFunctionWidget.cpp
  */
  void objectPropertyChanged();

 /*!
  * \ingroup      UI
  * \brief        Processes red, gren, blue and alpha channel selection change
  * \return       void
  * \par      Source:
  *                TransferFunctionWidget.cpp
  */
  void selectChannels(bool);

 /*!
  * \ingroup      UI
  * \brief        Processes luminance group selection
  * \param        enabled true if selected
  * \return       void
  * \par      Source:
  *                TransferFunctionWidget.cpp
  */
  void selectLuminance(bool enabled);

 /*!
  * \ingroup      UI
  * \brief        Processes luminance group selection
  * \param        enabled true if selected
  * \return       void
  * \par      Source:
  *                TransferFunctionWidget.cpp
  */
  void selectIntensity(bool enabled);

  /*!
  * \ingroup      UI
  * \brief        Processes alpha group selection
  * \param        enabled true if selected
  * \return       void
  * \par      Source:
  *                TransferFunctionWidget.cpp
  */
  void selectAlpha(bool enabled);

  /*!
  * \ingroup      UI
  * \brief        Processes load button press
  * \return       void
  * \par      Source:
  *                TransferFunctionWidget.cpp
  */
  void load(bool);

 /*!
  * \ingroup      UI
  * \brief        Processes save button press
  * \return       void
  * \par      Source:
  *                TransferFunctionWidget.cpp
  */
  void save(bool);

 /*!
  * \ingroup      UI
  * \brief        Processes function editor widget change
  * \return       void
  * \par      Source:
  *                TransferFunctionWidget.cpp
  */
  void tableChanged();

private:
 /*!
  * \ingroup      UI
  * \brief        Fills widget with the values the transfer function of the object.
  * \param        object object to be loaded
  * \return       void
  * \par      Source:
  *                TransferFunctionWidget.cpp
  */
  void loadProperties(WoolzObject* object);

 /*!
  * \ingroup      UI
  * \brief        Updates group selection to match channel selection
  * \return       void
  * \par      Source:
  *                TransferFunctionWidget.cpp
  */
  void updateGroups();

 /*!
  * \ingroup      UI
  * \brief        Recomputes the histogram of the object.
  * \par          Note: The histogram currently used is a log histogram.
  * \return       void
  * \par      Source:
  *                TransferFunctionWidget.cpp
  */
  void recomputeHistogram();

 /*!
  * \ingroup      UI
  * \brief        Loads the transfer function from the XML streams and generates commands for loading.
  * \param        xmlReader XML stream to read
  * \param        command   parent command to which new command should be generated
  * \return       void
  * \par      Source:
  *                TransferFunctionWidget.cpp
  */
  void loadFromXml(QXmlStreamReader &xmlReader);

 /*!
  * \ingroup      UI
  * \brief        Skips unknow xml tags
  * \param        xmlReader stream to read
  * \return       void
  * \par      Source:
  *                TransferFunctionWidget.cpp
  */
  void xmlSkipElement(QXmlStreamReader &xmlReader);

protected:
  ObjectListModelAbstract* m_objectListModel;      /*!< the object list model*/
  WoolzObject * m_object;                          /*!< current object*/
  QFunctionEditor *m_functionEditor;               /*!< widget for function editing*/
  SoMFFloat m_colorMap;                            /*!< color map edited by the widget*/
  float *m_pHist;                                  /*!< histogram of the object*/
  bool inUpdate;                                   /*!< in apply process, do not reload object properties*/
};

#endif  //TRANSFERFUNCTIONWIDGET_H
