#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _ObjectDelegate_h[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         ObjectDelegate.h
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
* \brief	Delegate for object tool tree view
* \ingroup	UI
*/

#ifndef OBJECTDELEGATE_H
#define OBJECTDELEGATE_H

#include <QStyledItemDelegate>
#include <QColor>
#include <QModelIndex>

class QPainter;
class QAbstractItemView;
/*!
 * \brief	Provides display and editing facilities in object tool tree
 * 		view
 * \ingroup	UI
 */
class ObjectDelegate : public QStyledItemDelegate
{
  Q_OBJECT

  public:
    /*!
     * \ingroup	UI
     * \brief	Constructor
     * \param	view			view that holds this widget needed to
     * 					make the color editor widget persistent
     * 					(editable all the time)
     * \param	parent			parent object
     */
    ObjectDelegate(QAbstractItemView *view, QObject *parent = 0);

    /*!
     * \ingroup	UI
     * \brief	Renders the delegate using the given painter and style option
     * 		for the item specified by index.
     *
     *          Displays the colour of the object as a coloured rectangle.
     * \param	painter			painter object
     * \param	option			options
     * \param	index			element to edit
     */
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
	const QModelIndex &index) const;
    /*!
     * \ingroup	UI
     * \brief	Creates specialised editors for non-standard items
     * \param	parent			parent widget
     * \param	option			options
     * \param	index			element to edit
     * \return	editor widget
     */
    QWidget *createEditor(QWidget *parent,
	const QStyleOptionViewItem &option,
	const QModelIndex &index) const;
    /*!
     * \ingroup	UI
     * \brief	Sets editor data from the model
     * \param	editor			editor widget
     * \param	index			index of the item to set
     */
    void setEditorData(QWidget * editor, const QModelIndex & index) const;

    /*!
     * \ingroup	UI
     * \brief	Sets model data from the editor
     * \param	editor			editor widget
     * \param	model			model to change
     * \param	index			index of the item in the model to set
     */
    void setModelData(QWidget * editor, QAbstractItemModel * model,
        const QModelIndex & index) const;

  private slots:
    /*!
     * \ingroup	UI
     * \brief	Slot to process end of editing
     */
    void emitCommitData();

  private:
    QAbstractItemView *m_view;      	/*!< view contating the element
    				             managed by the delegate
					     visualising, required to set
					     persistent state */
};
#endif //OBJECTDELEGATE_H
