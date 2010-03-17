#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _WoolzDynThresholdedObj_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         WoolzDynThresholdedObj.h
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
#ifndef WOOLZDYNTRESHOLDEDOBJECT_H
#define WOOLZDYNTRESHOLDEDOBJECT_H

#include "WoolzDynObject.h"

class ObjectListModel;
/*!
 * \brief	Class storing and managing and generated mesh woolz object
 * \ingroup      Control
 */
class WoolzDynThresholdedObj : public WoolzDynObject
{
  Q_OBJECT

public:
  enum channelTypes { Red, Green, Blue, Grey};   /*!< channel used to threshold */

 /*!
  * \ingroup      Control
  * \brief        Constructor
  * \return       void
  * \par      Source:
  *                WoolzDynThresholdedObj.cpp
  */
  WoolzDynThresholdedObj(ObjectListModel * objectListModel);

 /*!
  * \ingroup      Control
  * \brief        Destructor
  * \return       void
  * \par      Source:
  *                WoolzDynThresholdedObj.cpp
  */
  virtual ~WoolzDynThresholdedObj();

 /*!
  * \ingroup      Control
  * \brief        Returns notes of the object.
  * \return       Return the "generated" string
  * \par      Source:
  *                WoolzDynThresholdedObj.cpp
  */
  virtual QString notes ( )   {
    return "threshold";
  }

 /*!
  * \ingroup      Control
  * \brief        Sets low threshold
  * \param        lowTh new low threshold
  * \return       void
  * \par      Source:
  *                WoolzDynThresholdedObj.cpp
  */
  void setLowThreshold ( unsigned char lowTh) {
    if (m_lowTh != lowTh) {
      m_lowTh = lowTh;
      emit updated(false);
      emit objectPropertyChanged();
    }
  }

 /*!
  * \ingroup      Control
  * \brief        Sets high threshold
  * \param        highTh new high threshold
  * \return       void
  * \par      Source:
  *                WoolzDynThresholdedObj.cpp
  */
  void setHighThreshold ( unsigned char highTh) {
    if (m_highTh != highTh) {
       m_highTh = highTh;
       emit updated(false);
       emit objectPropertyChanged();
   }
  }

 /*!
  * \ingroup      Control
  * \brief        Sets new channel to threshold
  * \param        channel type of the channel used to threshold
  * \return       void
  * \par      Source:
  *                WoolzDynThresholdedObj.cpp
  */
  void setChannel ( enum channelTypes channel) {
    m_channel = channel;
    emit updated(false);
  }

 /*!
  * \ingroup      Control
  * \brief        Returns the low threshold value
  * \return       low threshold value
  * \par      Source:
  *                WoolzDynThresholdedObj.cpp
  */
  unsigned char lowTh() { return  m_lowTh;}

  /*!
  * \ingroup      Control
  * \brief        Returns the high threshold value
  * \return       high threshold value
  * \par      Source:
  *                WoolzDynThresholdedObj.cpp
  */
  unsigned char highTh() { return  m_highTh;}

 /*!
  * \ingroup      Control
  * \brief        Returns the thresholded channel type
  * \return       thresholded channel type
  * \par      Source:
  *                WoolzDynThresholdedObj.cpp
  */
  enum channelTypes channel() { return  m_channel;}

 /*!
  * \ingroup      Control
  * \brief        Saves object details in xml format.
  * \param        xmlWriter output xml stream
  * \return       true if succeded, false if not
  * \par      Source:
  *                WoolzDynThresholdedObj.cpp
  */
  virtual bool saveAsXml(QXmlStreamWriter *xmlWriter);

 /*!
  * \ingroup      Control
  * \brief        Checkes if the object is a value object
  * \return       true if is a value object
  * \par      Source:
  *                WoolzDynThresholdedObj.cpp
  */
  virtual bool isValue ( ) ;

 /*!
  * \ingroup      Control
  * \brief        Sets up connections to target
  * \param        target connection target
  * \return       void
  * \par      Source:
  *                WoolzDynThresholdedObj.cpp
  */
  virtual void setupConnections(QObject *target);

  /*!
  * \ingroup      Control
  * \brief        Checks without reading the whole file if the object is colour
  * \return       true if is a colour value object
  * \par      Source:
  *                WoolzDynThresholdedObj.cpp
  */
  virtual bool isColourPreRead () { return (!m_obj && (sourceObj()) && sourceObj()->isColourPreRead()) || isColour();}

 /*!
  * \ingroup      Control
  * \brief        Checks without reading the whole file if the object is a Value object
  * \return       true if is a 2D or 3D value object
  * \par      Source:
  *                WoolzDynThresholdedObj.cpp
  */
  virtual bool isValuePreRead () { return (!m_obj && (sourceObj()) && sourceObj()->isValuePreRead()) || isValue();}

  /*!
   * \ingroup      Control
   * \brief        Checks if object parameters changes requiring update
   * \return       true if object needs update
   * \par      Source:
   *                WoolzDynThresholdedObj.cpp
   */
   virtual bool needsUpdate();

protected:
 /*!
  * \ingroup      Control
  * \brief        Re-thresholds the object
  * \return       void
  * \par      Source:
  *                WoolzDynThresholdedObj.cpp
  */
  void doUpdate ();

/*!
  * \ingroup      Control
  * \brief        Saves object details in xml format.
  * \param        xmlWriter output xml stream
  * \return       true if succeded, false if not
  * \par      Source:
  *                WoolzDynThresholdedObj.cpp
  */
  virtual bool saveAsXmlProperties(QXmlStreamWriter *xmlWriter) ;

 /*!
  * \ingroup      Control
  * \brief        Reads current token if known
  * \param        element current element of the DOM tree
  * \return       true if succeded, false if not
  * \par      Source:
  *                WoolzDynThresholdedObj.cpp
  */
  virtual bool parseDOMLine(const QDomElement &element);

 /*!
  * \ingroup      Control
  * \brief        Save parameters used for dynamic object
  * \return       void
  * \par      Source:
  *               WoolzDynThresholdedObj.cpp
  */
  virtual void saveUsedParameters();

protected:
  unsigned char m_lowTh;                 /*!< low threshold */
  unsigned char m_highTh;                /*!< high threshold value */
  enum channelTypes m_channel;           /*!< threshold chanel type */
  unsigned char m_lowThUsed;             /*!< low threshold value used in dynamic object*/
  unsigned char m_highThUsed;            /*!< high threshold value used in dynamic object*/
  enum channelTypes m_channelUsed;       /*!< threshold chanel type used in dynamic object*/

public:
  static const char * xmlTag;           /*!< xml section tag string */
};
#endif //WOOLZDYNTRESHOLDEDOBJECT_H
