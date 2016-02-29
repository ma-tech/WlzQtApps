#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _TransferFunctionWidget_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         TransferFunctionWidget.cpp
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
* \brief        Widget for manipulating segmented Woolz Object properties
* 		(see WoolzDynCutOffedObj)
* \ingroup      UI
*
*/

//project includes
#include <TransferFunctionWidget.h>
#include "TransferFunction.h"
#include "ObjectListModelAbstract.h"
#include "WoolzObject.h"
#include <QFunctionEditor.h>

//qt includes
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QtXml/QXmlStreamWriter>
#include <QtXml/QDomDocument>
#include <QtXml/QDomNode>
#include <QtXml/QDomElement>
#include <QTextStream>

TransferFunctionWidget::
TransferFunctionWidget(
  QWidget *parent,
  ObjectListModelAbstract *objectListModel):
QDockWidget(parent),
m_objectListModel(objectListModel),
m_object(NULL),
inUpdate(false)
{

  setupUi(this);
  comboBoxScale->addItem(QString("Scale N"));
  comboBoxScale->addItem(QString("Scale N^2"));
  comboBoxScale->addItem(QString("Scale N^3"));
  comboBoxScale->setCurrentIndex(0);
  connect(horizontalHighSlider, SIGNAL(valueChanged(int)),
          this, SLOT(highCutOffChanged()));
  connect(horizontalLowSlider, SIGNAL(valueChanged(int)),
          this, SLOT(lowCutOffChanged()));
  connect(checkBoxAlpha, SIGNAL(clicked(bool)),
          this, SLOT(selectChannels(bool)));
  connect(checkBoxRed, SIGNAL(clicked(bool)),
          this, SLOT(selectChannels(bool)));
  connect(checkBoxGreen, SIGNAL(clicked(bool)),
          this, SLOT(selectChannels(bool)));
  connect(checkBoxBlue, SIGNAL(clicked(bool)),
          this, SLOT(selectChannels(bool)));
  connect(radioButtonLuminance, SIGNAL(clicked(bool)),
          this, SLOT(selectLuminance(bool)));
  connect(radioButtonIntensity, SIGNAL(clicked(bool)),
          this, SLOT(selectIntensity(bool)));
  connect(radioButtonAlpha, SIGNAL(clicked(bool)),
          this, SLOT(selectAlpha(bool)));
  connect(pushButtonLoad, SIGNAL(clicked(bool)),
          this, SLOT(load(bool)));
  connect(pushButtonSave, SIGNAL(clicked(bool)),
          this, SLOT(save(bool)));
  connect(pushButtonGamma, SIGNAL(clicked(bool)),
          this, SLOT(setGamma()));
  connect(pushButtonInvert, SIGNAL(clicked(bool)),
          this, SLOT(invert()));
  connect(comboBoxScale, SIGNAL(currentIndexChanged(int)),
          SLOT(scaleSelected(int)));
  connect(objectListModel, SIGNAL(removedObjectSignal(WoolzObject*)),
          this, SLOT(removedObjectSignal(WoolzObject*)));
  connect(objectListModel, SIGNAL(addObjectSignal(WoolzObject*)),
          this, SLOT(addObjectSignal(WoolzObject*)));
  connect(objectListModel, SIGNAL(objectSelected(WoolzObject*)),
          this, SLOT(objectSelected(WoolzObject*)));

  m_functionEditor =  new QFunctionEditor();
  connect(m_functionEditor , SIGNAL(tableChanged()),
          this, SLOT(tableChanged()));

  m_colorMap.setNum(256 * 4);
  m_pHist = new float[256];
  memset(m_pHist, 0, sizeof(float)*256);

  m_functionEditor->setMap(&m_colorMap);
  m_functionEditor->setMode(QFunctionEditor::EDIT_ALPHA_COMP );
  m_functionEditor->setObjectName(QString::fromUtf8("FunctionEditor"));
  m_functionEditor->setMinimumSize(QSize(300, 300));
  horizontalLayout->addWidget(m_functionEditor);

  horizontalHighSlider->setEnabled(false);
  spinBoxHighCutOff->setEnabled(false);
  labelHighCutOff->setEnabled(false);
  horizontalLowSlider->setEnabled(false);
  spinBoxLowCutOff->setEnabled(false);
  labelLowCutOff->setEnabled(false);
  pushButtonGamma->setEnabled(false);
  doubleSpinBoxGamma->setEnabled(false);
  groupBoxComponents->setEnabled(false);
  groupBoxGroups->setEnabled(false);
  m_functionEditor->setEnabled(false);
  pushButtonLoad->setEnabled(false);
  pushButtonSave->setEnabled(false);

  selectAlpha(true);                // select alpha channel editing
  setMinimumSize(150, 0);
  setMaximumSize(550, 550);
  resize(150, 400);
}

TransferFunctionWidget::
~TransferFunctionWidget()
{
  delete m_functionEditor;
  delete []m_pHist;
}

void TransferFunctionWidget::
recomputeHistogram()
{
  if(m_object && m_object->getObj() && m_pHist)
  {
    memset(m_pHist, 0, 256 * sizeof(float));
    float max= 0.0f;

    WlzObject     *histObj = NULL;
    WlzErrorNum   errNum = WLZ_ERR_NONE;
    if(m_object->getWoolzGreyType() == WLZ_GREY_UBYTE)
    {
      histObj = WlzAssignObject(
                WlzHistogramObj(m_object->getObj(),
				256, 0.0, 1.0, &errNum), NULL);
    }
    else
    {
      WlzPixelV     greyMinV,
                    greyMaxV;

      if(WlzGreyRange(m_object->getObj(), &greyMinV, &greyMaxV) == WLZ_ERR_NONE)
      {
        WlzValueConvertPixel(&greyMinV, greyMinV, WLZ_GREY_DOUBLE);
        WlzValueConvertPixel(&greyMaxV, greyMaxV, WLZ_GREY_DOUBLE);

        histObj = WlzAssignObject(
                  WlzHistogramObj(m_object->getObj(), 256, greyMinV.v.dbv,
                                  (greyMaxV.v.dbv - greyMinV.v.dbv + 1) / 256,
                                  &errNum), NULL);
      }
    }
    if((histObj != NULL) && (errNum == WLZ_ERR_NONE))
    {
      WlzHistogramDomain *histDom;
      histDom = histObj->domain.hist;
      Q_ASSERT(histDom->nBins == 256);
      for(int idx = 0; idx < 256; ++idx)
      {
	if ((float )*(histDom->binValues.inp + idx) > 0)
	{
	  m_pHist[idx] = log((float) *(histDom->binValues.inp + idx));
	}
	else
	{
	  m_pHist[idx] = 0;
	}
	if (max<m_pHist[idx])
	{
	  max=m_pHist[idx];
	}
      }
      m_functionEditor->setHistogram(m_pHist, max);
    }
    if(histObj)
    {
      WlzFreeObj(histObj);
    }
  }
}

void TransferFunctionWidget::
lowCutOffChanged()
{
  unsigned char cutOff=spinBoxLowCutOff->value();
  m_functionEditor->setLowCutoff(cutOff);
  if(!m_object)
  {
    return ;
  }
  m_object->transferFunction()->setLowCutOff(spinBoxLowCutOff->value());
}

void TransferFunctionWidget::
highCutOffChanged()
{
  unsigned char cutOff=spinBoxHighCutOff->value();
  m_functionEditor->setHighCutoff(cutOff);
  if(!m_object)
  {
    return ;
  }
  m_object->transferFunction()->setHighCutOff(spinBoxHighCutOff->value());
}

void TransferFunctionWidget::
loadProperties(
  WoolzObject *object)
{
  m_object = NULL;  // disable property update
  if(!object)
  {
    return;
  }
  TransferFunction *transferFunction =  object->transferFunction();
  if(!transferFunction)
  {
    return;
  }
  m_colorMap.copyFrom(transferFunction->getColorMap());
  horizontalLowSlider->setValue(transferFunction->lowCutOff());
  horizontalHighSlider->setValue(transferFunction->highCutOff());
  m_functionEditor->setLowCutoff(transferFunction->lowCutOff());
  m_functionEditor->setHighCutoff(transferFunction->highCutOff());
  m_functionEditor->update();
  m_object = object;
  recomputeHistogram();  //must be after m_object is assigned
}

void TransferFunctionWidget::
objectSelected(
  WoolzObject* object)
{
  if(m_object)
  {
    disconnect(m_object , 0, this, 0);
  }

  m_object = NULL;
  if(object != NULL)
  {
    //disable autoupdate while properties are populated
    connect(object, SIGNAL(objectPropertyChanged()),
	    this, SLOT(objectPropertyChanged()));
    loadProperties(object);
  }
  bool visible = (m_object != NULL) && (m_object->transferFunction() != NULL);
  horizontalHighSlider->setEnabled(visible);
  spinBoxHighCutOff->setEnabled(visible);
  labelHighCutOff->setEnabled(visible);
  horizontalLowSlider->setEnabled(visible);
  spinBoxLowCutOff->setEnabled(visible);
  labelLowCutOff->setEnabled(visible);
  pushButtonGamma->setEnabled(visible);
  doubleSpinBoxGamma->setEnabled(visible);
  groupBoxComponents->setEnabled(visible);
  groupBoxGroups->setEnabled(visible);
  m_functionEditor->setEnabled(visible);
  pushButtonLoad->setEnabled(visible);
  pushButtonSave->setEnabled(visible);
  resize(150, 0);
}

void TransferFunctionWidget::
removedObjectSignal(
  WoolzObject* obj)
{
  if(obj == m_object)
  {
    objectSelected(NULL);
  }
}


void TransferFunctionWidget::
addObjectSignal(
  WoolzObject* obj)
{
  connect(obj, SIGNAL(objectPropertyChanged()),
          this, SLOT(objectPropertyChanged()));
}

void TransferFunctionWidget::
objectPropertyChanged()
{
  if(inUpdate)
  {
    return;
  }
  WoolzObject* obj = qobject_cast<WoolzObject*>(sender());
  if(!obj || (obj != m_object))
  {
    return;
  }
  m_object= NULL;  //disable updates
  loadProperties(obj);
}

void TransferFunctionWidget::
updateGroups()
{
  QFunctionEditor::EditMode mode = QFunctionEditor::EDIT_NONE;
  if(checkBoxAlpha->isChecked())
  {
    mode = mode | QFunctionEditor::EDIT_ALPHA_COMP;
  }
  if(checkBoxRed->isChecked())
  {
    mode |= QFunctionEditor::EDIT_RED_COMP;
  }
  if(checkBoxGreen->isChecked())
  {
    mode |= QFunctionEditor::EDIT_GREEN_COMP;
  }
  if(checkBoxBlue->isChecked())
  {
    mode |= QFunctionEditor::EDIT_BLUE_COMP;
  }
  m_functionEditor->setMode(mode);
}

void TransferFunctionWidget::
selectChannels(bool)
{
  if(checkBoxRed->isChecked() &&
     checkBoxGreen->isChecked() &&
     checkBoxBlue->isChecked())
  {
    if(checkBoxAlpha->isChecked())
    {
      radioButtonIntensity->setChecked(true);
    }
    else
    {
      radioButtonLuminance->setChecked(true);
    }
  }
  else
  {
    if(!checkBoxRed->isChecked() &&
       !checkBoxGreen->isChecked() &&
       !checkBoxBlue->isChecked() &&
       checkBoxAlpha->isChecked())
    {
      radioButtonAlpha->setChecked(true);
    }
    else
    {
      radioButtonUser->setChecked(true);
    }
  }
  updateGroups();
}

void TransferFunctionWidget::tableChanged()
{
  if(m_object)
  {
    m_object->transferFunction()->setColorMap(m_colorMap);
  }
}

void TransferFunctionWidget::
selectLuminance(
  bool enabled)
{
  if(enabled)
  {
    checkBoxRed->setChecked(true);
    checkBoxGreen->setChecked(true);
    checkBoxBlue->setChecked(true);
    checkBoxAlpha->setChecked(false);
    updateGroups();
  }
}

void TransferFunctionWidget::
selectIntensity(
  bool enabled)
{
  if(enabled)
  {
    checkBoxRed->setChecked(true);
    checkBoxGreen->setChecked(true);
    checkBoxBlue->setChecked(true);
    checkBoxAlpha->setChecked(true);
    updateGroups();
  }
}

void TransferFunctionWidget::
selectAlpha(
  bool enabled)
{
  if(enabled)
  {
    checkBoxRed->setChecked(false);
    checkBoxGreen->setChecked(false);
    checkBoxBlue->setChecked(false);
    checkBoxAlpha->setChecked(true);
    updateGroups();
  }
}

bool TransferFunctionWidget::
loadFromDOM(
  const QDomElement &element)
{
  SoMFFloat colorMap;
  colorMap.setNum(256 * 4);
  TransferFunction *tf = new TransferFunction;
  Q_ASSERT(tf);
  if(!tf->parseDOM((element)))
  {
    return(false);
  }
  m_object->transferFunction()->setColorMap(colorMap);
  return(true);
}


void TransferFunctionWidget::
load(
  bool)
{
  QString filename = QFileDialog::getOpenFileName(NULL,
      tr("Load transfer function"), ".",
      tr("Transfer function (*.trf);;Icol (*.icol);;"));
  if(filename.isEmpty())
  {
     return;
  }
  QString ext = QFileInfo(filename).suffix().toUpper();
  if(ext.isEmpty())
  {
    filename += ".trf";
  }
  QFile file(filename);
  if(!file.open(QFile::ReadOnly | QFile::Text))
  {
    QMessageBox::warning(NULL,
        "Load transfer function", "Cannot open transfer function file.");
    return;
  }

  if(ext.toUpper() == "ICOL")
  {
    QTextStream in(&file);
    SoMFFloat colorMap;
    colorMap.setNum(256 * 4);
    float *f = colorMap.startEditing();
    memset(f, sizeof(*f) * 4 * 256, 0);
    int index;
    int r, g, b, a;
    for(index = 0; index < 256; ++index)
    {
      in >> r >> g >> b >> a;
      colorMap.set1Value(index * 4,     r / 255.0f);
      colorMap.set1Value(index * 4 + 1, g / 255.0f);
      colorMap.set1Value(index * 4 + 2, b / 255.0f);
      colorMap.set1Value(index * 4 + 3, a / 255.0f);
    }
    int low, high;
    in >> low >> high;
    m_object->transferFunction()->setHighCutOff((double )high * 255.0);
    m_object->transferFunction()->setLowCutOff((double )low * 255.0);
    m_object->transferFunction()->setColorMap(colorMap);
  }
  else
  {
    QDomDocument doc;
    QString errorStr;
    int errorLine, errorColumn;
    if(!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
    {
      QMessageBox::warning(NULL, "Load transfer function",
          QString("Parse error at line %1 column %2 :").
              arg(errorLine).arg(errorColumn) + errorStr);
      QApplication::restoreOverrideCursor();
      return;
    }

    QDomElement root = doc.documentElement();
    if(root.tagName() == "TransferFunction")
    {
      if(!loadFromDOM(root))
      {
        QMessageBox::warning(NULL, "Load transfer function",
            QString("Error reading transfer function."));
      }
    }
    else
    {
      QMessageBox::warning(NULL, "Load transfer function",
          QString("Not a landmark file."));
    }

    QApplication::restoreOverrideCursor();

    if(file.error())
    {
      QMessageBox::warning(NULL, "Load transfer function",
          "Cannot read file.");
      return;
    }
  }
}

void TransferFunctionWidget::
save(bool)
{
  if(!m_object || !m_object->transferFunction())
  {
    return;
  }
  QString filename = QFileDialog::getSaveFileName(NULL,
      tr("Save transfer function"), ".",
      tr("Transfer function (*.trf)"));
  if(filename.isEmpty())
  {
    return;
  }

  QString ext = QFileInfo(filename).suffix().toUpper();
  if(ext.isEmpty())
  {
    filename += ".trf";
  }
  QFile file(filename);
  if(!file.open(QFile::WriteOnly | QFile::Text))
  {
    QMessageBox::warning(NULL,
        tr("Save transfer function"),
        tr("Cannot create transfer function file."));
    return;
  }
  QXmlStreamWriter xmlWriter(&file);
  xmlWriter.setAutoFormatting(true);
  xmlWriter.writeStartDocument();
  m_object->transferFunction()->saveAsXml(&xmlWriter);
  if(file.error())
  {
    QMessageBox::warning(NULL,
        tr("Save transfer function"),
        tr("Error writing file."));
    file.remove();
    return;
  }
}

void TransferFunctionWidget::
setGamma()
{
  if(!m_object)
  {
    return ;
  }
  int index;
  bool r = checkBoxRed->isChecked();
  bool g = checkBoxGreen->isChecked();
  bool b = checkBoxBlue->isChecked();
  bool a = checkBoxAlpha->isChecked();
  double gamma = doubleSpinBoxGamma->value();
  int low = spinBoxLowCutOff->value();
  int high = spinBoxHighCutOff->value();
  double range = (high-low);
  if(range  > 0)
  {
    for(index = low; index <= high; ++index)
    {
      float value = pow((index - low)/range, gamma);
      if(r)
      {
        m_colorMap.set1Value(index * 4,     value);
      }
      if(g)
      {
        m_colorMap.set1Value(index * 4 + 1, value);
      }
      if(b)
      {
        m_colorMap.set1Value(index * 4 + 2, value);
      }
      if(a)
      {
        m_colorMap.set1Value(index * 4 + 3, value);
      }
    }
    m_functionEditor->update();
    m_object->transferFunction()->setColorMap(m_colorMap);
  }
}

void TransferFunctionWidget::
invert()
{
  if(!m_object)
  {
    return ;
  }
  int index;
  bool r = checkBoxRed->isChecked();
  bool g = checkBoxGreen->isChecked();
  bool b = checkBoxBlue->isChecked();
  bool a = checkBoxAlpha->isChecked();
  int low = spinBoxLowCutOff->value();
  int high = spinBoxHighCutOff->value();
  double range = (high-low);
  if(range  > 0)
  {
    for(index = low; index <= high; ++index)
    {
      int	i4;
      float	val;

      i4 = index * 4;
      if(r)
      {
	val = 1.0 - m_colorMap[i4 + 0];
        m_colorMap.set1Value(i4 + 0, val);
      }
      if(g)
      {
	val = 1.0 - (m_colorMap)[i4 + 1];
        m_colorMap.set1Value(i4 + 1, val);
      }
      if(b)
      {
	val = 1.0 - (m_colorMap)[i4 + 2];
        m_colorMap.set1Value(i4 + 2, val);
      }
      if(a)
      {
	val = 1.0 - (m_colorMap)[i4 + 3];
        m_colorMap.set1Value(i4 + 3, val);
      }
    }
    m_functionEditor->update();
    m_object->transferFunction()->setColorMap(m_colorMap);
  }
}

void TransferFunctionWidget::
scaleSelected(int index)
{
  switch(index)
  {
    case 0:
      m_functionEditor->setGraphMode(QFunctionEditor::GRAPH_IDENTITY);
      break;
    case 1:
      m_functionEditor->setGraphMode(QFunctionEditor::GRAPH_SQUARE);
      break;
    case 2:
      m_functionEditor->setGraphMode(QFunctionEditor::GRAPH_CUBE);
      break;
    default:
      break;
  }
  m_functionEditor->update();
}
