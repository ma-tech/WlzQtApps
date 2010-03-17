#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _WarperConfig_cpp[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         WarperConfig.cpp
* \author       Zsolt Husz
* \date         October 2008
* \version      $Id$
* \par
* Address:
*               MRC Human Genetics Unit,
*               Western General Hospital,
*               Edinburgh, EH4 2XU, UK.
* \par
* Copyright (C) 2009 Medical research Council, UK.
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
* \brief        Class to store global data.
* \ingroup      UI
*
*/

#include "WarperConfig.h"
#include <QApplication>
#include <QFileInfo>
#include "WoolzFileObject.h"

WarperConfig::WarperConfig() : QObject(), m_globalAutoUpdate (true), m_globalWarpUpdate (true) {}

void WarperConfig::setGlobalAutoUpdate(bool newGlobalAutoUpdate) {
    if (m_globalAutoUpdate!=newGlobalAutoUpdate) {
      m_globalAutoUpdate=newGlobalAutoUpdate;
      if (m_globalAutoUpdate) {
          QApplication::setOverrideCursor(Qt::WaitCursor);
          updateAll();
          QApplication::restoreOverrideCursor();
      }
  }
}

void WarperConfig::setGlobalWarpUpdate(bool newWarpAutoUpdate) {
    if (m_globalWarpUpdate!=newWarpAutoUpdate) {
      m_globalWarpUpdate=newWarpAutoUpdate;
      if (m_globalWarpUpdate) {
          QApplication::setOverrideCursor(Qt::WaitCursor);
          updateAllWarped();
          QApplication::restoreOverrideCursor();
      }
  }
}

void WarperConfig::setProjectDir(QString dir) {
    m_strProjectDir =  dir;
    WoolzFileObject::setBaseDir(m_strProjectDir);
}

void WarperConfig::setProjectDirFromFile(QString filename) {
    setProjectDir(QFileInfo(filename).canonicalPath());
}
