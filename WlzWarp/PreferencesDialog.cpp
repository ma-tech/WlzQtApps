#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _PreferencesDialog_cpp[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         PreferencesDialog.cpp
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

#include <QtColorPicker>
#include <QSettings>

#include "PreferencesDialog.h"

PreferencesDialog::PreferencesDialog(QWidget *parent): QDialog(parent) {
  setupUi( this );

  connect(pushButtonApply, SIGNAL(clicked()), this, SLOT(apply()));
  QSettings settings;
  colorPickerSource->setStandardColors();
  colorPickerTarget->setStandardColors();
  colorpickerResult->setStandardColors();

  m_origSourceViewerColour = settings.value("viewer/source/color", QColor(0, 77, 51)).value<QColor>();
  m_origTargetViewerColour = settings.value("viewer/target/color", QColor(0, 51, 77)).value<QColor>();
  m_origResultViewerColour = settings.value("viewer/result/color", QColor(77, 0, 0)).value<QColor>();
  m_orig2DMarkerSize       = settings.value("markers/2D/size", 1.00f).value<float>();
  m_orig3DMarkerSize       = settings.value("markers/3D/size", 1.00f).value<float>();

  m_origIsIMQ = settings.value("warping/type", QString("IMQ")).value<QString>() == "IMQ";
  m_origDeltaIMQ = settings.value("warping/IMQ/delta", 0.1f).value<float>();
  m_origDeltaMQ  = settings.value("warping/MQ/delta", 0.1f).value<float>();
  m_deltaIMQ = m_origDeltaIMQ;
  m_deltaMQ = m_origDeltaMQ;
  if (m_origIsIMQ) {
     doubleSpinBoxDelta->setValue(m_origDeltaIMQ);
     radioButtonIMQ->setChecked(true);
  } else {
     doubleSpinBoxDelta->setValue(m_origDeltaMQ);
     radioButtonMQ->setChecked(true);
  }
  m_useIMQ = m_origIsIMQ;

  colorPickerSource->setCurrentColor(m_origSourceViewerColour);
  colorPickerTarget->setCurrentColor(m_origTargetViewerColour);
  colorpickerResult->setCurrentColor(m_origResultViewerColour);

  doubleSpinBox2DMarker->setValue(m_orig2DMarkerSize);
  doubleSpinBox3DMarker->setValue(m_orig3DMarkerSize);

  connect( radioButtonIMQ, SIGNAL(clicked(bool)),
           this, SLOT(basisFnChanged()));
  connect( radioButtonMQ, SIGNAL(clicked(bool)),
           this, SLOT(basisFnChanged()));

  tabWidget->removeTab(2);
  //tabWarping->setVisible(false); // do not use this tab
  //could be used again to provide default parameters for warping
}

void PreferencesDialog::accept() {
    apply();
    QDialog::accept();
}

void PreferencesDialog::apply() {
    QSettings settings;
    settings.setValue("viewer/source/color", colorPickerSource->currentColor());
    settings.setValue("viewer/target/color", colorPickerTarget->currentColor());
    settings.setValue("viewer/result/color", colorpickerResult->currentColor());
    settings.setValue("markers/2D/size", doubleSpinBox2DMarker->value());
    settings.setValue("markers/3D/size", doubleSpinBox3DMarker->value());
    settings.setValue("warping/type", m_useIMQ ? QString("IMQ") : QString("MQ"));
    settings.setValue("warping/IMQ/delta", m_deltaIMQ);
    settings.setValue("warping/MQ/delta", m_deltaMQ);
    emit configurationChanged();
}

void PreferencesDialog::reject() {
    QSettings settings;
    settings.setValue("viewer/source/color", m_origSourceViewerColour);
    settings.setValue("viewer/target/color", m_origTargetViewerColour);
    settings.setValue("viewer/result/color", m_origResultViewerColour);
    settings.setValue("markers/2D/size", m_orig2DMarkerSize);
    settings.setValue("markers/3D/size", m_orig3DMarkerSize);

    settings.setValue("warping/type", m_origIsIMQ ? QString("IMQ") : QString("MQ"));
    settings.setValue("warping/IMQ/delta", m_origDeltaIMQ);
    settings.setValue("warping/MQ/delta", m_origDeltaMQ);

    emit configurationChanged();
    QDialog::reject();
}

void PreferencesDialog::basisFnChanged() {
  bool useIMQ = radioButtonIMQ->isChecked();

  //any change?
  if (useIMQ == m_useIMQ)
      return; //no: return

  if (useIMQ) {
     m_deltaMQ = doubleSpinBoxDelta->value();
     doubleSpinBoxDelta->setValue(m_deltaIMQ);
  } else {
     m_deltaIMQ = doubleSpinBoxDelta->value();
     doubleSpinBoxDelta->setValue(m_deltaMQ);
  }
  m_useIMQ = useIMQ;
}
