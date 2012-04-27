#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _TransferFunction_cpp[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         TransferFunction.cpp
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
* \brief        Extends the SoTransferFunction with low and high cut off values
* \ingroup      UI
*
*/

#include "TransferFunction.h"
#include <QtXml/QDomElement>

const char* TransferFunction::xmlTag = "TransferFunction";

TransferFunction::TransferFunction(QObject *parent):  QObject(parent), SoTransferFunction() {
    m_lowCutOff  = 50;
    m_highCutOff = 255;
    predefColorMap = SoTransferFunction::NONE;
    colorMapType = SoTransferFunction::RGBA;
    for (int ir=0;ir<256;ir++) {
        m_colorMap.set1Value(ir*4, ir/255.0f);
        m_colorMap.set1Value(ir*4+1, ir/255.0f);
        m_colorMap.set1Value(ir*4+2, ir/255.0f);
        if (ir<m_lowCutOff  )
           m_colorMap.set1Value(ir*4+3, 0.00f);
        else if (ir>m_highCutOff)
           m_colorMap.set1Value(ir*4+3, 1.00f);
        else
           m_colorMap.set1Value(ir*4+3, 0.0f+(ir-m_lowCutOff)/((float)(m_highCutOff-m_lowCutOff)));
    }
    colorMap.copyFrom(m_colorMap);
}

void TransferFunction::copyTF(TransferFunction *tf){
    if (tf) {
        m_lowCutOff = tf->m_lowCutOff;
        m_highCutOff = tf->m_highCutOff;
        setColorMap(tf->m_colorMap);
    }
}

void TransferFunction::update() {
   colorMap.copyFrom(m_colorMap);
   int ir=0;
   for (; ir< m_lowCutOff;ir++)
      colorMap.set1Value(ir*4+3, 0.0f);
   for (ir = m_highCutOff+1; ir< 256;ir++)
      colorMap.set1Value(ir*4+3, 0.0f);
   emit updated();
}

void TransferFunction::setColorMap(SoMFFloat &colorMap) {
   m_colorMap.copyFrom(colorMap);
   update();
}


bool TransferFunction::saveAsXml(QXmlStreamWriter *xmlWriter) {
  Q_ASSERT(xmlWriter);
  xmlWriter->writeStartElement(getXmlTag());
  xmlWriter->writeTextElement("lowCutOff", QString("%1").arg(m_lowCutOff));
  xmlWriter->writeTextElement("highCutOff", QString("%1").arg(m_highCutOff));
  for (int i=0; i<256 ; i++) {
      xmlWriter->writeStartElement("value");
      xmlWriter->writeTextElement("index", QString("%1").arg(i));
      xmlWriter->writeTextElement("red", QString("%1").arg(m_colorMap[i*4]));
      xmlWriter->writeTextElement("green", QString("%1").arg(m_colorMap[i*4+1]));
      xmlWriter->writeTextElement("blue", QString("%1").arg(m_colorMap[i*4+2]));
      xmlWriter->writeTextElement("alpha", QString("%1").arg(m_colorMap[i*4+3]));
      xmlWriter->writeEndElement();
  }
  xmlWriter->writeEndElement();
  return true;
}

bool TransferFunction::parseDOM(const QDomElement &element) {
  if (element.tagName() != getXmlTag())
        return false;
  QDomNode child = element.firstChild();
  m_colorMap.setNum(256*4);
  float *f=m_colorMap.startEditing();
  memset(f, sizeof(*f)*4*256, 0);
  colorMap.finishEditing();
  while (!child.isNull()) {
      QDomElement element = child.toElement();
      if (element.tagName() == "lowCutOff") {
          m_lowCutOff =  element.text().toInt();
      } else if (element.tagName() == "highCutOff") {
          m_highCutOff =  element.text().toInt();
      } else if (element.tagName() == "value") {
        int index=-1;
        float R=0;
        float G=0;
        float B=0;
        float A=0;
        QDomNode child = element.firstChild();
        while (!child.isNull()) {
           QDomElement element = child.toElement();
           if (element.tagName() == "index") {
              index =  element.text().toInt();
           } else if (element.tagName() == "red") {
              R =  element.text().toFloat();
           } else if (element.tagName() == "green") {
              G =  element.text().toFloat();
           } else if (element.tagName() == "blue") {
              B =  element.text().toFloat();
           } else if (element.tagName() == "alpha") {
              A =  element.text().toFloat();
           }
          child = child.nextSibling();
        }
        if (index>=0 && index<256) {
           m_colorMap.set1Value(index*4  , R);
           m_colorMap.set1Value(index*4+1, G);
           m_colorMap.set1Value(index*4+2, B);
           m_colorMap.set1Value(index*4+3, A);
        }
      }
      child = child.nextSibling();
  }
  update();
  return true;
}
