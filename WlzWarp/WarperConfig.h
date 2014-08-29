#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _WarperConfig_h[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         WarperConfig.h
* \author       Zsolt Husz
* \date         October 2008
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
* \brief	Class to store global data.
* \ingroup	UI
*/

#ifndef WARPERCONFIG_H
#define WARPERCONFIG_H

#include <QObject>

class ObjectListModel ;
/*! 
 * \brief	Combo box control for selecting a colour.
 * \ingroup	Control
 */
class WarperConfig : public QObject
{
  Q_OBJECT
  public:
    /**
     * \ingroup	Control
     * \brief	Constructor
     */
    WarperConfig();

    /**
     * \ingroup	Control
     * \brief	Returns the global auto update state
     * \return	update sate
     */
    bool globalAutoUpdate() {return m_globalAutoUpdate;}

    /**
     * \ingroup	Control
     * \brief	Sets the global auto update state.
     */
    void setGlobalAutoUpdate(bool newGlobalAutoUpdate);

    /**
     * \ingroup	Control
     * \brief	Returns the global warp update state.
     */
    bool globalWarpUpdate() {return m_globalWarpUpdate;}

    /**
     * \ingroup	Control
     * \brief	Sets the global warp update state.
     * \return	update sate
     */
    void setGlobalWarpUpdate(bool newWarpAutoUpdate);

    /**
     * \ingroup	Control
     * \brief	Returns the project directory
     */
    QString projectDir() {return m_strProjectDir;}

    /**
     * \ingroup	Control
     * \brief	Sets the project base
     * \param	dir			directory to set
     */
    void setProjectDir(QString dir);

    /**
     * \ingroup	Control
     * \brief	Sets the project base from the filename
     * \param	filename		file to get the directory from
     */
    void setProjectDirFromFile(QString filename);

signals:
    /**
     * \ingroup	Control
     * \brief	Signals need to update all dynamic objects
     */
    void updateAll();

    /**
     * \ingroup	Control
     * \brief	Signals need to update all warped objects
     */
    void updateAllWarped();
  private:
    bool m_globalAutoUpdate;   		/*!< if enables/disables warping
    					     updates */
    bool m_globalWarpUpdate;            /*!< if enables/disables updates for
    					     all dynamic objects */
    QString m_strProjectDir;            /*!< directory of the project */
};

extern WarperConfig config;

#endif //WARPERCONFIG
