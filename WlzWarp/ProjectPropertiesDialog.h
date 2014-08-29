#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _ProjectPropertiesDialog_h[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         ProjectPropertiesDialog.h
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
* \brief	Manages project properties
* \ingroup	UI
*/

#ifndef PROJECTPROPERTIES_H
#define PROJECTPROPERTIES_H

#include <QDialog>
#include <ui_ProjectPropertiesDialog.h>

class QMdiArea;
class QObject;

class ProjectProperties;

/*!
 * \brief	Controler class for setting mesh properties
 * \ingroup	UI
 */
class ProjectPropertiesDialog:
public QDialog, public Ui::
ProjectPropertiesDialog
{
  Q_OBJECT
  public:
    /*!
     * \ingroup	UI
     * \brief	Constructor
     * \param	parent			parent widget
     * \param   objectListModel
     */
    ProjectPropertiesDialog(ProjectProperties * projectProperties,
        QWidget *parent = NULL);


    private slots:
      /*!
       * \ingroup	UI
       * \brief	Processes dialog close with accept
       */
      virtual void accept();

  protected:
    ProjectProperties *m_projectProperties;  /*!< list of objects */
};

#endif  //PROJECTPROPERTIES_H
