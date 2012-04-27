#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _TransferFunction_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         TransferFunction.h
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

#ifndef TRANSFERFUNCTION_H
#define TRANSFERFUNCTION_H

#include <VolumeViz/nodes/SoTransferFunction.h>

//Qt includes
#include <QObject>
#include <QtXml/QXmlStreamWriter>

class QDomElement;

/*!
 * \brief	 Extends the SoTransferFunction with low and high cut off values
 *               and providing signalling when updated
 * \ingroup      UI
 */
class TransferFunction : public QObject, public SoTransferFunction
{
  Q_OBJECT
public:
 /*!
  * \ingroup      UI
  * \brief        Constructor
  * \param        parent parent object
  * \par      Source:
  *                TransferFunction.cpp
  */
  TransferFunction(QObject *parent = NULL);

public:
  static const char * xmlTag;             /*!< xml section tag string */

public:
 /*!
  * \ingroup      UI
  * \brief        Sets new colour map
  * \param        colorMap new colour map
  * \return       void
  * \par      Source:
  *                TransferFunction.cpp
  */
  virtual void setColorMap(SoMFFloat &colorMap);

 /*!
  * \ingroup      UI
  * \brief        Returns current colour map
  * \return       colour map refference
  * \par      Source:
  *                TransferFunction.cpp
  */
  virtual SoMFFloat& getColorMap() {return m_colorMap;}

 /*!
  * \ingroup      UI
  * \brief        Sets new low cut off value
  * \param        newCutOff cut off value
  * \return       void
  * \par      Source:
  *                TransferFunction.cpp
  */
  virtual void setLowCutOff(unsigned char newCutOff) {m_lowCutOff=newCutOff;update();}

 /*!
  * \ingroup      UI
  * \brief        Returns low cut off value
  * \return       low cut off value
  * \par      Source:
  *                TransferFunction.cpp
  */
  virtual unsigned char lowCutOff() {return m_lowCutOff;}

 /*!
  * \ingroup      UI
  * \brief        Sets new high cut off value
  * \param        newCutOff cut off value
  * \return       void
  * \par      Source:
  *                TransferFunction.cpp
  */
  virtual void setHighCutOff(unsigned char newCutOff) {m_highCutOff=newCutOff;update();}

 /*!
  * \ingroup      UI
  * \brief        Returns high cut off value
  * \return       high cut off value
  * \par      Source:
  *                TransferFunction.cpp
  */
  virtual unsigned char highCutOff() {return m_highCutOff;}

 /*!
  * \ingroup      Control
  * \brief        Saves transfer function in xml format.
  * \param        xmlWriter output xml stream
  * \return       true if succeded, false if not
  * \par      Source:
  *                TransferFunction.cpp
  */
  virtual bool saveAsXml(QXmlStreamWriter *xmlWriter);

 /*!
  * \ingroup      Control
  * \brief        Reads transfer functi from DOM tree
  * \param        element current element of the DOM tree
  * \return       true if succeded, false if not
  * \par      Source:
  *                TransferFunction.cpp
  */
  virtual bool parseDOM(const QDomElement &element);

  void copyTF(TransferFunction *tf);

private:
 /*!
  * \ingroup      UI
  * \brief        Sets the actual colour map using the current settings
  * \return       void
  * \par      Source:
  *                TransferFunction.cpp
  */
  virtual void update();

  /*!
   * \ingroup      UI
   * \brief        Returns the xml tag used by the object
   * \return       xml tag defined in static member xmlTag
   * \par      Source:
   *                TransferFunction.cpp
   */
   virtual const char * getXmlTag() { return xmlTag;}
signals:
 /*!
  * \ingroup      UI
  * \brief        Signals the update of the transfer function
  * \return       void
  * \par      Source:
  *                TransferFunction.cpp
  */
  void updated();


protected:
  unsigned char m_lowCutOff;               /*!< current low cut off value*/
  unsigned char m_highCutOff;              /*!< current high cut off value*/
  SoMFFloat m_colorMap;                    /*!< current colour map, including regions cut off*/

};

#endif // TRANSFERFUNCTION_H
