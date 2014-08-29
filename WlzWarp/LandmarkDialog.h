#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _LandmarkDialog_h[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         LandmarkDialog.h
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
* \brief	Landmark dialog
* \ingroup	UI
*/

#ifndef LANDMARKDIALOG_H
#define LANDMARKDIALOG_H

#include <QDialog>
#include <ui_LandmarkDialog.h>

class LandmarkController;

/*!
 * \brief	Controler class for landmark dialog
 * \ingroup	UI
 */
class LandmarkDialog : public QDialog, public Ui::LandmarkDialog
{
  Q_OBJECT
  public:

    /*!
     * \ingroup	UI
     * \brief	Constructor
     * \param	controller			the landmark controller
     * \param	parent			parent widget
     */
    LandmarkDialog(LandmarkController* controller, QWidget *parent = 0);

    /*!
     * \ingroup	UI
     * \brief	Destructor
     */
    ~LandmarkDialog();

    private slots:
      /*!
       * \ingroup	UI
       * \brief	Processes the signal of delete button press
       */
      void removeLandmark();

    /*!
     * \ingroup	UI
     * \brief	Processes the signal of landmark selection change
     * \param	current			new selection
     * \param	previous		previous selection
     */
    void currentRowChanged ( const QModelIndex & current,
	const QModelIndex & /*previous */);

    /*!
     * \ingroup	UI
     * \brief	Processes the signal of feature point selection generated
     * 		by viewers
     */
    void setHighlight(const int index, const bool on);

  private:
    LandmarkController* m_controller;    /*!< the landmark controller*/
};

#endif // LANDMARKDIALOG_H
