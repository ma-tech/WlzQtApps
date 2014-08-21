#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _TransferFunctionWidget_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         TransferFunctionWidget.cpp
* \author       Zsolt Husz
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
#include "TransferFunctionWidget.h"
#include "TransferFunction.h"
#include "ObjectListModelAbstract.h"
#include "WoolzObject.h"
#include <QFunctionEditor.h>

//qt includes
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QtXml/QXmlStreamWriter>
#include <QtXml/QXmlStreamReader>
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
  connect(pushButtonUpdate, SIGNAL(clicked(bool)),
          this, SLOT(apply()));
  connect(pushButtonGamma, SIGNAL(clicked(bool)),
          this, SLOT(setGamma()));
  connect(objectListModel, SIGNAL(removedObjectSignal(WoolzObject*)),
          this, SLOT(removedObjectSignal(WoolzObject*)));
  connect(objectListModel, SIGNAL(addObjectSignal(WoolzObject*)),
          this, SLOT(addObjectSignal(WoolzObject*)));
  connect(objectListModel, SIGNAL(objectSelected(WoolzObject*)),
          this, SLOT(objectSelected(WoolzObject*)));

  m_functionEditor =  new QFunctionEditor();
  connect(m_functionEditor , SIGNAL(tableChanged()),
      this, SLOT(tableChanged()));

  m_colorMap.setNum(256*4);
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
  pushButtonUpdate->setEnabled(false);
  pushButtonGamma->setEnabled(false);
  doubleSpinBoxGamma->setEnabled(false);
  checkBoxAutoUpdate->setEnabled(false);
  groupBoxComponents->setEnabled(false);
  groupBoxGroups->setEnabled(false);
  m_functionEditor->setEnabled(false);
  checkBoxAutoUpdate->setEnabled(false);
  pushButtonLoad->setEnabled(false);
  pushButtonSave->setEnabled(false);

  selectAlpha(true);                // select alpha channel editing
  setMinimumSize(150,0);
  setMaximumSize(550,550);
  resize(150,400);
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
    histObj = WlzAssignObject(WlzHistogramObj(m_object->getObj(),
                                              256, 0.0, 1.0, &errNum), NULL);
    if((histObj != NULL) && (errNum == WLZ_ERR_NONE))
    {
      WlzHistogramDomain *histDom;
      histDom = histObj->domain.hist;
      Q_ASSERT(histDom->nBins==256);
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
  if(!m_object)
  {
    return ;
  }
  unsigned char cutOff=spinBoxLowCutOff->value();
  m_functionEditor->setLowCutoff(cutOff);
  if(!checkBoxAutoUpdate->isChecked())
  {
    return ;
  }
  m_object->transferFunction()->setLowCutOff(spinBoxLowCutOff->value());
}

void TransferFunctionWidget::
highCutOffChanged()
{
  if(!m_object)
  {
    return ;
  }
  unsigned char cutOff=spinBoxHighCutOff->value();
  m_functionEditor->setHighCutoff(cutOff);
  if(!checkBoxAutoUpdate->isChecked())
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
  pushButtonUpdate->setEnabled(visible);
  pushButtonGamma->setEnabled(visible);
  doubleSpinBoxGamma->setEnabled(visible);
  checkBoxAutoUpdate->setEnabled(visible);
  groupBoxComponents->setEnabled(visible);
  groupBoxGroups->setEnabled(visible);
  m_functionEditor->setEnabled(visible);
  checkBoxAutoUpdate->setEnabled(visible);
  pushButtonLoad->setEnabled(visible);
  pushButtonSave->setEnabled(visible);
  resize(150,0);
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
apply()
{
  inUpdate = true; // disable control updates from object data
  m_object->transferFunction()->setHighCutOff(spinBoxHighCutOff->value());
  m_object->transferFunction()->setLowCutOff(spinBoxLowCutOff->value());
  m_object->transferFunction()->setColorMap(m_colorMap);
  inUpdate = false; // re-enable control updates from object data
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
  if(checkBoxRed->isChecked() && checkBoxGreen->isChecked() &&
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
    if(!checkBoxRed->isChecked() && !checkBoxGreen->isChecked() &&
       !checkBoxBlue->isChecked() && checkBoxAlpha->isChecked())
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

void TransferFunctionWidget::tableChanged() {
  if (m_object)
      if (checkBoxAutoUpdate->isChecked())
           m_object->transferFunction()->setColorMap(m_colorMap);

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

void TransferFunctionWidget::
xmlSkipElement(
  QXmlStreamReader &xmlReader)
{
  xmlReader.readNext();
  while(!xmlReader.atEnd())
  {
    if(xmlReader.isEndElement())
    {
      xmlReader.readNext();
      break;
    }
    if(xmlReader.isStartElement())
    {
      xmlSkipElement(xmlReader);
    }
    else
    {
      xmlReader.readNext();
    }
  }
}

void TransferFunctionWidget::
loadFromXml(
  QXmlStreamReader &xmlReader)
{
  SoMFFloat colorMap;
  colorMap.setNum(256*4);
  float *f=colorMap.startEditing();
  memset(f, sizeof(*f)*4*256, 0);
  colorMap.finishEditing();
  xmlReader.readNext();
  while(!xmlReader.atEnd())
  {
    if(xmlReader.isEndElement())
    {
      xmlReader.readNext();
      break;
    }
    if(xmlReader.isStartElement() && (xmlReader.name() == "lowCutOff"))
    {
      m_object->transferFunction()->setLowCutOff(
          xmlReader.readElementText().toDouble());
      if(xmlReader.isEndElement())
      {
	xmlReader.readNext();
      }
    }
    else if(xmlReader.isStartElement() && (xmlReader.name() == "highCutOff"))
    {
      m_object->transferFunction()->setHighCutOff(
          xmlReader.readElementText().toDouble());
      if(xmlReader.isEndElement())
      {
	xmlReader.readNext();
      }
    }
    else if(xmlReader.isStartElement() && (xmlReader.name() == "value"))
    {
      xmlReader.readNext();
      int index = -1;
      float R = 0;
      float G = 0;
      float B = 0;
      float A = 0;
      while(!xmlReader.atEnd())
      {
	if(xmlReader.isEndElement())
	{
	  xmlReader.readNext();
	  break;
	}
	if(xmlReader.isStartElement() && (xmlReader.name() == "index"))
	{
	  index = xmlReader.readElementText().toInt();
	  if(xmlReader.isEndElement())
	  {
	    xmlReader.readNext();
	  }
	}
	else if(xmlReader.isStartElement() && (xmlReader.name() == "red"))
	{
	  R = xmlReader.readElementText().toFloat();
	  if(xmlReader.isEndElement())
	  {
	    xmlReader.readNext();
	  }
	}
	else if(xmlReader.isStartElement() && (xmlReader.name() == "green"))
	{
	  G = xmlReader.readElementText().toFloat();
	  if(xmlReader.isEndElement())
	  {
	    xmlReader.readNext();
	  }
	}
	else if(xmlReader.isStartElement() && (xmlReader.name() == "blue"))
	{
	  B = xmlReader.readElementText().toFloat();
	  if(xmlReader.isEndElement())
	  {
	    xmlReader.readNext();
	  }
	}
	else if(xmlReader.isStartElement() && (xmlReader.name() == "alpha"))
	{
	  A = xmlReader.readElementText().toFloat();
	  if(xmlReader.isEndElement())
	  {
	    xmlReader.readNext();
	  }
	}
	else if(xmlReader.isStartElement())
	{
	  xmlSkipElement(xmlReader);
	}
	else
	{
	  xmlReader.readNext();
	}
      }
      if(index>=0 && index<256)
      {
	colorMap.set1Value(index * 4,     R);
	colorMap.set1Value(index * 4 + 1, G);
	colorMap.set1Value(index * 4 + 2, B);
	colorMap.set1Value(index * 4 + 3, A);
      }
    }
    else if(xmlReader.isStartElement())
    {
      xmlSkipElement(xmlReader);
    }
    else
    {
      xmlReader.readNext();
    }
  }
  m_object->transferFunction()->setColorMap(colorMap);
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

  if(ext.toUpper()=="ICOL")
  {
    QTextStream in(&file);
    SoMFFloat colorMap;
    colorMap.setNum(256*4);
    float *f=colorMap.startEditing();
    memset(f, sizeof(*f)*4*256, 0);
    int index;
    int r,g,b,a;
    for(index = 0; index < 256; ++index)
    {
      in >> r>>g>>b>>a;
      colorMap.set1Value(index*4  , r/255.0f);
      colorMap.set1Value(index*4+1, g/255.0f);
      colorMap.set1Value(index*4+2, b/255.0f);
      colorMap.set1Value(index*4+3, a/255.0f);
    }
    int low, high;
    in >>low>>high;
    m_object->transferFunction()->setHighCutOff((double )high * 255.0);
    m_object->transferFunction()->setLowCutOff((double )low * 255.0);
    m_object->transferFunction()->setColorMap(colorMap);
    QXmlStreamReader xmlReader(&file);
    xmlReader.readNext();
    while(!xmlReader.atEnd())
    {
      if(xmlReader.isStartElement())
      {
	if(xmlReader.name() == "TransferFunction")
	{
	  loadFromXml(xmlReader);
	}
	else
	{
	  xmlReader.readNext();
	}
      }
      else
      {
	xmlReader.readNext();
      }
    }
    file.close();

    if(xmlReader.hasError() || file.error())
    {
      QMessageBox::warning(NULL,
          "Load transfer function", "Cannot read file.");
      return;
    }
  }
}

void TransferFunctionWidget::
save(
  bool)
{
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
  if (!file.open(QFile::WriteOnly | QFile::Text))
  {
    QMessageBox::warning(NULL, tr("Save transfer function"), tr("Cannot create transfer function file."));
    return;
  }
  QXmlStreamWriter xmlWriter(&file);
  xmlWriter.setAutoFormatting(true);
  xmlWriter.writeStartDocument();
  xmlWriter.writeStartElement("TransferFunction");
  xmlWriter.writeTextElement("lowCutOff",
      QString("%1").arg(spinBoxLowCutOff->value()));
  xmlWriter.writeTextElement("highCutOff",
      QString("%1").arg(spinBoxHighCutOff->value()));
  for(int i = 0; i < 256 ; ++i)
  {
    xmlWriter.writeStartElement("value");
    xmlWriter.writeTextElement("index", QString("%1").arg(i));
    xmlWriter.writeTextElement("red", QString("%1").arg(m_colorMap[i*4]));
    xmlWriter.writeTextElement("green", QString("%1").arg(m_colorMap[i*4+1]));
    xmlWriter.writeTextElement("blue", QString("%1").arg(m_colorMap[i*4+2]));
    xmlWriter.writeTextElement("alpha", QString("%1").arg(m_colorMap[i*4+3]));
    xmlWriter.writeEndElement();
  }
  xmlWriter.writeEndElement();

  if(file.error())
  {
    QMessageBox::warning(NULL,
        tr("Save transfer function"), tr("Error writing file."));
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
    if(!checkBoxAutoUpdate->isChecked())
    {
      return ;
    }
    m_object->transferFunction()->setColorMap(m_colorMap);
  }
}
