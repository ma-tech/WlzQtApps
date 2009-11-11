#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _PreferencesDialog_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         PreferencesDialog.h
* \author       Zsolt Husz
* \date         September 2009
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
* \brief        Dialog to set UI preferences
* \ingroup      UI
*
*/
#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>
#include <ui_PreferencesDialog.h>

class QMdiArea;
class ObjectListModel;
class WoolzObject;
class LandmarkModel;
class QObject;

/*!
 * \brief	Dialog for UI properties
 * \ingroup      UI
 */
class PreferencesDialog : public QDialog, public Ui::PreferencesDialog
{
Q_OBJECT
public:
 /*!
  * \ingroup      UI
  * \brief        Constructor
  * \param        parent parent widget
  * \param        objectListModel
  * \par      Source:
  *               PreferencesDialog.cpp
  */
  PreferencesDialog(QWidget *parent);

private slots:
 /*!
  * \ingroup      UI
  * \brief        Processes dialog close with accept
  * \return       void
  * \par      Source:
  *               PreferencesDialog.cpp
  */
  virtual void accept();

 /*!
  * \ingroup      UI
  * \brief        Processes dialog close with cancel
  * \return       void
  * \par      Source:
  *               PreferencesDialog.cpp
  */
  virtual void reject();

 /*!
  * \ingroup      UI
  * \brief        Processes apply button press
  * \return       void
  * \par      Source:
  *               PreferencesDialog.cpp
  */
  virtual void apply();

private slots:
  /*!
  * \ingroup      UI
  * \brief        Processes basis function change
  * \return       void
  * \par      Source:
  *                TransformWidget.cpp
  */
  void basisFnChanged();

signals:
 /*!
  * \ingroup      UI
  * \brief        Signals configuration change that has be processed by those functionalities using this configuration
  * \return       void
  * \par      Source:
  *               PreferencesDialog.cpp
  */
  void configurationChanged();
protected:
  QColor m_origSourceViewerColour;          /*!< source viewer background colour */
  QColor m_origTargetViewerColour;          /*!< target viewer background colour */
  QColor m_origResultViewerColour;          /*!< result viewer background colour */
  float  m_orig2DMarkerSize;                /*!< 2D marker size */
  float  m_orig3DMarkerSize;                /*!< 3D marker size */
  bool   m_origIsIMQ;                       /*!< if is IMQ warping*/
  float  m_origDeltaIMQ;                    /*!< delta value for IMQ*/
  float  m_origDeltaMQ;                     /*!< delta value for MQ*/
  bool   m_useIMQ;                          /*!< current warping type*/
  float  m_deltaIMQ;                        /*!< current delta value for IMQ*/
  float  m_deltaMQ;                         /*!< current delta value for MQ*/

};

#endif  //PREFERENCESDIALOG_H
