//TODO: currently not used

#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _LoadViewThread_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         LoadViewThread.h
* \author       Zsolt Husz
* \date         October 2008
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
* \brief        Manages object loading in different thread
* \ingroup      UI
* \todo         not yet implemented
*
*/
#ifndef LOADVIEWTHREAD_H
#define LOADVIEWTHREAD_H

#include <QThread>

/*!
* \brief	Thread class managing background object loading
* \ingroup      UI
*/
class LoadViewThread : public QThread
{
Q_OBJECT
public:
  /*!
  * \ingroup      UI
  * \brief        Constructor
  * \param        parent parent object
  *
  * \return       void
  * \par      Source:
  *                LoadViewThread.cpp
  */
   LoadViewThread(QObject *parent = 0);

  /*!
  * \ingroup      UI
  * \brief        Destructor
  * \param        parent parent object
  *
  * \return       void
  * \par      Source:
  *                LoadViewThread.cpp
  */
   ~LoadViewThread();
protected:
  /*!
  * \ingroup      UI
  * \brief        Starts the thread
  *
  * \return       void
  * \par      Source:
  *                LoadViewThread.cpp
  */
   void run();
private:
    volatile bool done; /*!< if true thread finished loading */
};

#endif
