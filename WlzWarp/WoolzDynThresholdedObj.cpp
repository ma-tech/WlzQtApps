#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _WoolzDynThresholdedObj_cpp[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         WoolzDynThresholdedObj.cpp
* \author       Zsolt Husz
* \date         March 2009
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
* \brief        Dynamicaly generated segmented Woolz objects
* \ingroup      Control
*
*/
#include "WoolzDynThresholdedObj.h"
#include "ObjectListModel.h"
#include <QtXml/QDomElement>
#include <QtXml/QXmlStreamWriter>

const char* WoolzDynThresholdedObj::xmlTag = "ThresholdObject";

WoolzDynThresholdedObj::WoolzDynThresholdedObj(ObjectListModel * objectListModel) :
         WoolzDynObject(objectListModel) {
  m_channel = Grey;
  m_lowTh = 0;
  m_highTh = 255;

  m_channelUsed = Blue; //anything different from above
  m_lowThUsed = 255;
  m_highThUsed = 0;
}

WoolzDynThresholdedObj::~WoolzDynThresholdedObj() {}

void WoolzDynThresholdedObj::doUpdate () {
    WoolzObject *srcObj = sourceObj();

   if (!srcObj|| !(srcObj->getObj()) ||!(srcObj->isValue()))
      return ;

   statusChange("Tresholded " + m_name+ " started...", 0);

   WlzErrorNum	errNum = WLZ_ERR_NONE;

  if (m_obj) {
      WlzFreeObj(m_obj);
  }
  if (m_lowTh >  m_highTh) {
      m_obj = NULL;
  } else if (srcObj->isColour() && m_channel != Grey) {
      WlzPixelV     lowVal;
      WlzPixelV     highVal;
      WlzUInt       combineMode=0;
      lowVal.type = WLZ_GREY_RGBA;
      highVal.type = WLZ_GREY_RGBA;
      WLZ_RGBA_RED_SET(lowVal.v.rgbv,0);
      WLZ_RGBA_GREEN_SET(lowVal.v.rgbv,0);
      WLZ_RGBA_BLUE_SET(lowVal.v.rgbv,0);
      WLZ_RGBA_RED_SET(highVal.v.rgbv,255);
      WLZ_RGBA_GREEN_SET(highVal.v.rgbv,255);
      WLZ_RGBA_BLUE_SET(highVal.v.rgbv,255);
      WLZ_RGBA_RED_SET(combineMode, WLZ_BO_OR);
      WLZ_RGBA_GREEN_SET(combineMode, WLZ_BO_OR);
      WLZ_RGBA_BLUE_SET(combineMode, WLZ_BO_OR);
      switch (m_channel) {
        case Red:
          WLZ_RGBA_RED_SET(lowVal.v.rgbv,m_lowTh);
          WLZ_RGBA_RED_SET(combineMode, WLZ_BO_AND);
          WLZ_RGBA_RED_SET(highVal.v.rgbv,m_highTh);
          break;
        case Green:
          WLZ_RGBA_GREEN_SET(lowVal.v.rgbv,m_lowTh);
          WLZ_RGBA_GREEN_SET(combineMode, WLZ_BO_AND);
          WLZ_RGBA_GREEN_SET(highVal.v.rgbv,m_highTh);
          break;
        case Blue:
          WLZ_RGBA_BLUE_SET(lowVal.v.rgbv,m_lowTh);
          WLZ_RGBA_BLUE_SET(combineMode, WLZ_BO_AND);
          WLZ_RGBA_BLUE_SET(highVal.v.rgbv,m_highTh);
          break;
        default:
          Q_ASSERT(false);
      }

      m_obj = WlzRGBAMultiThreshold(
        srcObj->getObj(),
        lowVal,
        highVal,
        combineMode,
        &errNum);

      if (errNum == WLZ_ERR_NONE)
        m_obj = WlzAssignObject(m_obj, NULL);
      else
        m_obj = NULL;
  } else {
   WlzObject * tempObj;
   WlzPixelV greyV;
   if (m_lowTh>0) {
     greyV.type = WLZ_GREY_UBYTE;
     greyV.v.ubv = m_lowTh;
     //NOTE: Temporary object must be keept allive, since is used as thresholeded object
     tempObj = WlzAssignObject(WlzThreshold (srcObj->getObj(), greyV, WLZ_THRESH_HIGH, &errNum), NULL);
    } else
      tempObj = WlzAssignObject(srcObj->getObj(), NULL);
    if (m_highTh<255) {
       greyV.type = WLZ_GREY_UBYTE;
       greyV.v.ubv = m_highTh+1;
       m_obj = WlzAssignObject(WlzThreshold (tempObj, greyV, WLZ_THRESH_LOW, &errNum), NULL);
    } else
       m_obj = WlzAssignObject(tempObj, NULL);
    WlzFreeObj(tempObj);
    if (errNum!= WLZ_ERR_NONE) {
         if (m_obj)
            WlzFreeObj(m_obj);
         m_obj = NULL;
    }
  }
  m_type = srcObj->type();
  if (m_obj ) {
      statusChange("Tresholded " + m_name+ " finished.", 0);
  }  else {
      statusChange("Tresholded " + m_name+ " failed.", 0);
   }

  setupTransferFunction();
  saveUsedParameters();
  emit objectChanged();

  return;
}

bool WoolzDynThresholdedObj::saveAsXml(QXmlStreamWriter *xmlWriter) {
  xmlWriter->writeStartElement(xmlTag);
  saveAsXmlProperties(xmlWriter);
  xmlWriter->writeEndElement();
  return true;
}

bool WoolzDynThresholdedObj::saveAsXmlProperties(QXmlStreamWriter *xmlWriter) {
  WoolzDynObject::saveAsXmlProperties(xmlWriter);
  xmlWriter->writeTextElement("ThresholdLow", QString("%1").arg(m_lowTh));
  xmlWriter->writeTextElement("ThresholdHigh", QString("%1").arg(m_highTh));
  QString channel;
  switch (m_channel) {
      case Red:
          channel = "Red";break;
      case Green:
          channel = "Green";break;
      case Blue:
          channel = "Blue";break;
      case Grey:
          channel = "Grey";break;
  }
  xmlWriter->writeTextElement("Channel", channel);
  return true;
}

bool WoolzDynThresholdedObj::parseDOMLine(const QDomElement &element) {
    if (element.tagName() == "Channel") {
       QString str = element.text().toUpper();
       if (str == "RED")
           m_channel = Red;
       else if (str == "GREEN")
           m_channel = Green;
       else if (str == "BLUE")
           m_channel = Blue;
       else
           m_channel = Grey;
       return true;
    } else if (element.tagName() == "ThresholdLow") {
       m_lowTh = element.text().toInt();
       return true;
    } else if (element.tagName() == "ThresholdHigh") {
       m_highTh = element.text().toInt();
       return true;
    } else
       return WoolzDynObject::parseDOMLine(element);
    return false;
}

bool  WoolzDynThresholdedObj::needsUpdate() {
    return (m_lowThUsed != m_lowTh) || (m_highThUsed != m_highTh) || (m_channelUsed != m_channel) || WoolzDynObject::needsUpdate();
}

void WoolzDynThresholdedObj::saveUsedParameters() {
    m_lowThUsed    = m_lowTh;
    m_highThUsed   = m_highTh;
    m_channelUsed  = m_channel;
    WoolzDynObject::saveUsedParameters();
}

bool WoolzDynThresholdedObj::isValue ( ) {
  WoolzObject *srcObj = sourceObj();
  
  // if is really a value object, or the source is value and this object is read from while but not computed yet.
  return (WoolzObject::isValue()) || (!m_obj && srcObj && srcObj->isValue());
}

void WoolzDynThresholdedObj::setupConnections(QObject *target) {
    WoolzDynObject::setupConnections(target);
    connect( target, SIGNAL(loadAllSignal()), this, SLOT(update()));
}
