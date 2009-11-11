#if defined(__GNUC__)
#ident "MRC HGU $Id$"
#else
#if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#pragma ident "MRC HGU $Id$"
#else
static char _QFunctionEditor_h[] = "MRC HGU $Id$";
#endif
#endif
/*!
* \file         QFunctionEditor.h
* \author       Zsolt Husz
* \author       Christof Rezk-Salama
* \date         October 2008
* \version      $Id$
* \par
* Address:
*               MRC Human Genetics Unit,
*               Western General Hospital,
*               Edinburgh, EH4 2XU, UK.
* \par
* Copyright (C) 2008 Medical research Council, UK.
* Copyright (C) 2000-2002 Christof Rezk-Salama
*
* This code was adapted to Qt4 from Qt3 from Christof
* Rezk-Salama's OpenQVis software available with GNU General
* Public License
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
* \brief        Transfer function editing widget
* \par          This file is based on OpenQVis QFunctionEditor.qt.H (see notes bellow)
*               and it was migrated from Qt3 to Qt4 by Zsolt Husz together with
*               extension to low/high cut off modes
* \ingroup      UI
*
*/

#ifndef _QT_FUNCTION_EDITOR_H_
#define _QT_FUNCTION_EDITOR_H_

#include <QWidget>
#include <QPen>

class QMouseEvent;
class QResizeEvent;
class QPaintEvent;
class QToolButton;
class QSpinBox;
class QPainter;
class QString;

#include <Inventor/fields/SoMFFloat.h>


//! A widget class that provides a transfer function editor.
/*! \ingroup Widgets
The class QFunctionEditor provides a transfer function editor, that supports both "color tables", 
that store RGBA values in sequential order, and "component maps" that store the color components in separate arrays.
*/
class QFunctionEditor : public QWidget
{
	Q_OBJECT
public:
       /*! An enumeration of different edit modes for the function editor.
        *  Multiple selections are possibly by ORing the selected values.
	*/
        enum _EditMode {    EDIT_NONE = 0x00,         //!< No changes
                           EDIT_RED_COMP   = 0x01,   //!< Changes affect the red component
                           EDIT_GREEN_COMP = 0x02,   //!< Changes affect the green component
                           EDIT_BLUE_COMP  = 0x04,   //!< Changes affect the blue component
                           EDIT_ALPHA_COMP = 0x08    //!< Changes affect the alpha component
	};
        typedef QFlags<_EditMode> EditMode;



        /*! \brief constructor
         *  \param parent If parent is 0, the new widget becomes a top-level window. If parent is another widget, this widget becomes a child window inside parent. The new widget is deleted when parent is.
         *  \param name Name of the widget. The name is sent to the QObject constructor.
         */
        QFunctionEditor(QWidget *parent=0);
	
	//! destructor
	virtual ~QFunctionEditor();
	
	/*! \brief get the active edit mode
         *  \return the edit mode
         */
	inline EditMode getMode() { return m_nMode; }
	
public slots:
        /*! \brief slot function for setting the active edit mode
         *  \param nMode the edit mode to be used
         */
	void setMode(EditMode nMode) { m_nMode = nMode;}
	
	/*! \brief slot function for setting the color table to be edited
         *  \param pTable pointer an array of 2*256 char values, that contains the RGBA values.
         */
	void setTable(unsigned char *pTable);
	
	/*! \brief slot function for setting the component maps to be edited
         *  \param pR pointer an array of 256 float values, that contains the red values.
         *  \param pG pointer an array of 256 float values, that contains the green values.
         *  \param pB pointer an array of 256 float values, that contains the blue values.
         *  \param pA pointer an array of 256 float values, that contains the alpha values.
         */
        void setMap(SoMFFloat *colorMap);

	/*! \brief slot function for setting the histogram
         *  \param pHistogram pointer an array of 256 float values, that represents the histogram
         *  \param fMaxVal the maximum float value contained in the histogram.
         */
	void setHistogram(float *pHistogram, float fMaxVal);
	
	/*! \brief slot function for scaling the histogram
         *  \param fScale scale factor.
         */
        void setHistScale(float fScale) { m_fHistScale = fScale; /*repaint(FALSE);*/ repaint();}

        /*! \brief slot function for setting histogram low cut off value
         *  \param lowCutoff low cut off value
         */
        void setLowCutoff(int lowCutoff) { m_lowCutoff = lowCutoff; repaint();}

        /*! \brief slot function for setting histogram high cut off value
         *  \param highCutoff high cut off value
         */
        void setHighCutoff(int highCutoff) { m_highCutoff = highCutoff; repaint();}

	/*! \brief slot function for resetting the color table
         *  \param bSelected If set to true, only the selected color components are resetted, otherwise all components.
         */
	void resetTable(bool bSelected);
	
signals:
	//! signal, that is emitted whenever the color table changes
	void tableChanged();
	
protected:
	
       /** @name Draw Functions
        * helper function sfor drawing different parts of the widget
	*/
    //@{
	/*! \brief draws the frame of the widget
         *  \param pPainter pointer to a QPainter class, that represents the drawing area.
         */
	void drawFrame(QPainter *pPainter); 
	
	/*! \brief draws the ticks around the frame
         *  \param pPainter pointer to a QPainter class, that represents the drawing area.
         */
	void drawTicks(QPainter *pPainter);

        /*! \brief draws the limits of the transfer function
         *  \param pPainter pointer to a QPainter class, that represents the drawing area.
         */
        void drawLimits(QPainter *pPainter);

	/*! \brief draws the histogram
         *  \param pPainter pointer to a QPainter class, that represents the drawing area.
         *  \param start start x-coordinate for drawing
         *  \param end end x-coordinate for drawing
         */
	void drawHistogram(QPainter *pPainter, int start, int end);

	/*! \brief draws the color table
         *  \param pPainter pointer to a QPainter class, that represents the drawing area.
         *  \param start start x-coordinate for drawing
         *  \param end end x-coordinate for drawing
         */
	void drawColorTable(QPainter *pPainter, int start, int end);

	/*! \brief draws the component maps
         *  \param pPainter pointer to a QPainter class, that represents the drawing area.
         *  \param start start x-coordinate for drawing
         *  \param end end x-coordinate for drawing
         */
	void drawColorMap(QPainter *pPainter, int start, int end);
	//@}

	
	/*! \brief helper functions that clips a line against the valid area in the editor
         *  \param start_x x-coordinate of the starting point of the line
         *  \param start_y y-coordinate of the starting point of the line
         *  \param end_x x-coordinate of the end point of the line
         *  \param end_y y-coordinate of the end point of the line
         */
	bool ClipLine(int &start_x, int &start_y, int &end_x, int &end_y);
	
	/*! \brief helper functions that writes a line into the color table
         *  \param start_x x-coordinate of the starting point of the line
         *  \param start_y y-coordinate of the starting point of the line
         *  \param end_x x-coordinate of the end point of the line
         *  \param end_y y-coordinate of the end point of the line
         */
	void DoLine(int &start_x, int &start_y, int &end_x, int &end_y);
	
	/** @name Event handlers
         * functions that handle Qt events
         */
	//@{
	/*! \brief event handler for mouse move events
         *  \param m pointer to the mouse event.
         */
	void mouseMoveEvent(QMouseEvent *m);
	
	/*! \brief event handler for mouse release events
         *  \param m pointer to the mouse event.
         */
	void mouseReleaseEvent(QMouseEvent *m);
	
	/*! \brief event handler for mouse press events
         *  \param m pointer to the mouse event.
         */
	void mousePressEvent(QMouseEvent *m);
	
	/*! \brief event handler for resize events
         *  \param e pointer to the resize event.
         */
	void resizeEvent(QResizeEvent *e );
	
	/*! \brief event handler for paint events
         *  \param e pointer to the paint event.
         */
	void paintEvent(QPaintEvent *e );
	//@}
	
        //! enumeration of the internal modes
	enum InternalMode {
			INTERNAL_MODE_UNSPEC,      //! The mode is unspecified.
			INTERNAL_MODE_COLOR_TABLE, //! The widget displays a RGBA color table
			INTERNAL_MODE_COLOR_MAP    //! The widget displays separate component maps for R,G,B and A.
	};
	
	/** @name Pens and Brushes
         * Predefined pens and color brushes for drawing the different parts of the widget
         */
	//@{
	QPen    m_penRed;
	QPen    m_penGreen;
	QPen    m_penBlue;
	QPen    m_penAlpha;
	QPen    m_penHistogram;
	QPen    m_penTicks;
        QPen    m_penLimits;
        QBrush  m_brushBG;
	//@}

	/** @name The Internal State 
         * member variables that represent the internal state of the widget
         */
	//@{
	//! the number of ticks to be drawn
	int m_nTicks;
	//! The width of the border
	int m_nBorder_x;
	//! The height of the border
	int m_nBorder_y;
	//! The x-position from which the user started to drag
	int m_nStart_x;
	//! The y-position from which the user started to drag
	int m_nStart_y;
	//! set to true if the drag is valid
	bool m_bIsDragValid;

	// the current edit mode
	EditMode       m_nMode;
	// the internal mode of the widget
	InternalMode   m_nInternalMode;
	//@}
	
	
	/** @name Data
         *        member variables related to external data.
         */
	//@{

	/*! \brief A pointer to the current color table (array of 256 unsigned chars).
         *         This pointer is only valid, if m_nInternalMode is equal to INTERNAL_MODE_COLOR_TABLE.
         */
	unsigned char *m_pColorTableBuffer;

        float         *m_pHistogram;       /*!< the current histogram as an array of 256 floats */
        float          m_fHistogramMax;    /*!< maximum value in the histogram */
        float          m_fHistScale;       /*!< current scaling factor for the histogram */

	/*!
         * \brief pointer to the current colour map
         *        These pointers are only valid, if m_nInternalMode is equal to INTERNAL_MODE_COLOR_MAP.
         */
        SoMFFloat         *m_pColorMapBuffer;
        int m_lowCutoff;           /*!< low cut off value */
        int m_highCutoff;          /*!< high cut off value */
        //@}
};
#endif
