#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _QFunctionEditor_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         QFunctionEditor.cpp
* \author       Rezk-Salama, Zsolt Husz, Bill Hill
* \date         August 2014
* \version      $Id$
* \par
* Address:
*               MRC Human Genetics Unit,
*               MRC Institute of Genetics and Molecular Medicine,
*               University of Edinburgh,
*               Western General Hospital,
*               Edinburgh, EH4 2XU, UK.
* \par
* Copyright (C) 2000-2002 Christof Rezk-Salama
* This code was adapted to Qt4 from Qt3 from Christof
* Rezk-Salama's OpenQVis software available with GNU General
* Public License
*
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
* \brief        Transfer function editing widget
*               This file is based on OpenQVis QFunctionEditor.qt.H
* 	        (see notes bellow) and it was migrated from Qt3 to Qt4
* 	        by Zsolt Husz together with extension to low/high cut
* 	        off modes
* \ingroup      UI
*/
#include <iostream>
#include <stdlib.h>
#include <QPixmap>
#include <QPainter>

#include <QPaintEvent>
#include <QRect>

#include "QFunctionEditor.h"

#define CLAMP_TO_UBYTE(V) (((V)<(0))?(0):((V)>(255))?(255):(V))

//----------------------------------------
// Constructor transfer function editor
//----------------------------------------
//
QFunctionEditor::
QFunctionEditor(QWidget *parent):
QWidget(parent),
m_penRed(Qt::red,1),
m_penGreen(Qt::green,1),
m_penBlue(Qt::blue,1),
m_penAlpha(Qt::yellow,1),
m_penHistogram(QColor(65,65,65),1),
m_penTicks(QColor(102,102,204),1),
m_penLimits(QColor(255,120,0),1),
m_brushBG(QColor(127,127,127))
{
  // init internal state
  m_nTicks            = 64;
  m_fHistScale        = 1.0;
  m_nInternalMode     = INTERNAL_MODE_UNSPEC;
  m_pColorTableBuffer = NULL;
  m_pColorMapBuffer   = NULL;
  m_pHistogram        = NULL;
  m_fHistogramMax     = 1.0;
  m_bIsDragValid      = false;
  m_nMode             = EDIT_ALPHA_COMP;
  m_lowCutoff         = 0;
  m_highCutoff        = 255;
  m_graphmode         = GRAPH_IDENTITY;
  m_penLimits.setStyle(Qt::DashLine);

  // set minium size of rendering area
  setMinimumSize(290,290);
  setCursor( Qt::CrossCursor );
}

//----------------------------------------
// Destructor transfer function editor
//----------------------------------------
//
QFunctionEditor::
~QFunctionEditor()
{
}

//----------------------------------------
// callback function for resize events
//----------------------------------------
//
void QFunctionEditor::
resizeEvent(
  QResizeEvent *e)
{
  int nWidth  = width();
  int nHeight = height();
  QWidget::resizeEvent( e );
  m_nBorder_x  = (nWidth  - 256) / 2;
  m_nBorder_y  = (nHeight - 256) / 2;
  update();
}

//----------------------------------------
// callback function for redraw events
//----------------------------------------
//
void QFunctionEditor::
paintEvent(
  QPaintEvent *e)
{

  QWidget::paintEvent( e );
  QRect region = e->rect();
  QPainter painter(this);
  painter.fillRect(region, m_brushBG);
  drawFrame(&painter);
  drawTicks(&painter);
  drawHistogram(&painter, 0, 255);
  drawColorMap(&painter,region.x()-2, region.x() + region.width()+5);
  drawLimits(&painter);
}

//----------------------------------------
// helper function for drawing the ticks
//----------------------------------------
//
void QFunctionEditor::
drawTicks(
  QPainter *pPainter)
{
  int x;
  int nWidth  = width();
  int nHeight = height();

  if((nWidth < 256) || (nHeight < 256))
  {
    return;
  }

  pPainter->setPen(m_penTicks);
  QPoint *polygon = new QPoint[5];
  polygon[0].setX(m_nBorder_x - 1);
  polygon[0].setY(m_nBorder_y - 1);
  polygon[1].setX(m_nBorder_x + 256);
  polygon[1].setY(m_nBorder_y - 1);
  polygon[2].setX(m_nBorder_x + 256);
  polygon[2].setY(m_nBorder_y + 257);
  polygon[3].setX(m_nBorder_x-1);
  polygon[3].setY(m_nBorder_y + 257);
  polygon[4].setX(m_nBorder_x-1);
  polygon[4].setY(m_nBorder_y - 1);
  pPainter->drawPolygon(polygon, 5);

  // DRAW THE TICKS
  int nTickSizeLo = 6, nTickSizeHi = 10;
  int nTickCount = 0;
  int nLowCount = 4;
  for(x = 0; x <= 256; x += 256/m_nTicks)
  {
    if(nTickCount == 0)
    {
      pPainter->drawLine(m_nBorder_x - 1 + x,
                         m_nBorder_y + 257, 
	                 m_nBorder_x -1 + x,
			 m_nBorder_y + 257 + nTickSizeHi);
      pPainter->drawLine(m_nBorder_x - 1,
                         m_nBorder_y + x, 
	                 m_nBorder_x - 1 -nTickSizeHi,
			 m_nBorder_y + x);
    }
    else
    {
      pPainter->drawLine(m_nBorder_x - 1 + x,
                         m_nBorder_y + 257, 
	                 m_nBorder_x - 1 + x,
			 m_nBorder_y + 257 + nTickSizeLo);
      pPainter->drawLine(m_nBorder_x - 1,
                         m_nBorder_y + x,
	                 m_nBorder_x - 1 - nTickSizeLo,
			 m_nBorder_y + x);
    }
    nTickCount++;
    if(nTickCount == nLowCount)
    {
      nTickCount = 0;
    }
  }
  delete []polygon;
}


//----------------------------------------
// helper function for drawing the limit lines
//----------------------------------------
//
void QFunctionEditor::
drawLimits(
  QPainter *pPainter)
{
  int nWidth  = width();
  int nHeight = height();

  if((nWidth >= 256) && (nHeight >= 256))
  {
    pPainter->setPen(m_penLimits);
    if(m_lowCutoff > 0)
    {
      pPainter->drawLine(m_nBorder_x + m_lowCutoff - 1,
			 m_nBorder_y + 2,
			 m_nBorder_x + m_lowCutoff - 1,
			 m_nBorder_y + 254);
    }
    if(m_highCutoff < 255)
    {
      pPainter->drawLine(m_nBorder_x + m_highCutoff + 1,
                         m_nBorder_y + 2,
	                 m_nBorder_x + m_highCutoff + 1,
			 m_nBorder_y + 254);
    }
  }
}


//------------------------------------------
// helper function for drawing the histogram
//------------------------------------------
//
void QFunctionEditor::
drawHistogram(
  QPainter *pPainter,
  int start,
  int end)
{

  if(m_pHistogram != NULL)
  {
    pPainter->setPen(m_penHistogram);
    if(start > end)
    {
      int tmp = start;
      start = end;
      end = tmp;
    }
    if((start <= 255) && (end >= 0))
    {
      start = CLAMP_TO_UBYTE(start);
      end = CLAMP_TO_UBYTE(end);
      int nEnd_y;
      int nHeight  = height();
      int nMax_y   = nHeight - m_nBorder_y;
      int nMin_y   = m_nBorder_y;
      int nRange_y = 255;
      for(int x = start; x <= end; x++)
      {
	nEnd_y = nMax_y - (int )(nRange_y * m_fHistScale *
	                         m_pHistogram[x] / m_fHistogramMax);
	if(nEnd_y < nMin_y)
	{
	  nEnd_y = nMin_y;
	}
	pPainter->drawLine(m_nBorder_x + x, nMax_y, m_nBorder_x + x, nEnd_y);
      }
    }
  }
}

//----------------------------------------------
// helper function for drawing the color map
//----------------------------------------------
//
void QFunctionEditor::
drawColorMap(
  QPainter *pPainter,
  int start,
  int end)
{
  if(m_pColorMapBuffer != NULL)
  {
    int nHeight = height();
    int chan, x, start_x, start_y, end_x, end_y;

    start -= m_nBorder_x + 5;
    end   -= m_nBorder_x + 5;
    if(start > end)
    {
      int tmp = start;
      start = end;
      end = tmp;
    }
    start = CLAMP_TO_UBYTE(start);
    end = CLAMP_TO_UBYTE(end);
    for(chan = 0; chan < 4; ++chan)
    {
      QPen pen;
      EditMode comp;
      switch(chan)
      {
        case 0:
	  comp = EDIT_ALPHA_COMP;
	  pen = m_penAlpha;
	  break;
	case 1:
	  comp = EDIT_BLUE_COMP;
	  pen = m_penBlue;
	  break;
	case 2:
	  comp = EDIT_GREEN_COMP;
	  pen = m_penGreen;
	  break;
	case 3:
	  comp = EDIT_RED_COMP;
	  pen = m_penRed;
	  break;
	default:
	  break;
      }
      pPainter->setPen(pen);
      start_x = m_nBorder_x + start;
      start_y = nHeight - m_nBorder_y - 1 - getTableEntry(comp, start);
      for(x = start + 1; x <= end; x++)
      {
	end_x = m_nBorder_x + x;
	end_y = nHeight - m_nBorder_y - 1 - getTableEntry(comp,  x);
	pPainter->drawLine(start_x, start_y, end_x, end_y);
	start_x = end_x;
	start_y = end_y;
      }
    }
  }
}

//----------------------------------------------
// helper function for drawing the frame
//----------------------------------------------
//
void QFunctionEditor::
drawFrame(QPainter *pPainter)
{
  qDrawShadePanel(pPainter,0,0,width(),height(),palette(),true,2, NULL);
}

//----------------------------------------------
// callback function for mouse press events
//----------------------------------------------
//
void QFunctionEditor::
mousePressEvent(
  QMouseEvent *m)
{
  int point_x = m->x();
  int point_y = m->y();
  point_x -= m_nBorder_x;
  point_y -= m_nBorder_y;
  if((m->buttons()  & Qt::LeftButton) != 0)
  {
    // LEFT MOUSE BUTTON PRESSED
    if((point_x >= 0) && (point_x <= 255))
    {
      m_bIsDragValid = true;
      m_nStart_x = point_x;
      m_nStart_y = point_y;
    }
  }
  else if (((m->buttons() & Qt::MidButton) != 0) ||
	   ((m->buttons() & Qt::RightButton) != 0))
  {
    // MIDDLE MOUSE BUTTON PRESSED
    if((m_bIsDragValid))
    {
      DoLine(m_nStart_x, m_nStart_y, point_x, point_y);
      m_nStart_x = point_x;
      m_nStart_y = point_y;
      repaint();
    }
  }
}

//----------------------------------------------
// callback function for mouse release events
//----------------------------------------------
//
void QFunctionEditor::
mouseReleaseEvent(
  QMouseEvent *m)
{
  if((m->buttons() & Qt::LeftButton) == 0)
  {
    int point_x = m->x();
    int point_y = m->y();
    point_x -= m_nBorder_x;
    point_y -= m_nBorder_y;
    if((point_x >= 0) && (point_x <= 255))
    {
      m_nStart_x = point_x;
      m_nStart_y = point_y;
      m_bIsDragValid = true;
    }
  }
}

//----------------------------------------------
// reset the color table to linear ramp
//----------------------------------------------
//
void QFunctionEditor::
resetTable(
  bool bSelected)
{
  int start_X = 0, start_Y = 255, end_X = 255, end_Y = 0;
  if(bSelected)
  {
    DoLine(start_X, start_Y, end_X, end_Y);
  }
  else
  {
    QFunctionEditor::EditMode nMode = m_nMode;
    m_nMode = (QFunctionEditor::EditMode) 0xFF;
    DoLine(start_X, start_Y, end_X, end_Y);
    m_nMode = nMode;
  }
  update();
}

//----------------------------------------------------
// edits the color table with the given stroke line
//----------------------------------------------------
//
void QFunctionEditor::
DoLine(
  int &start_x,
  int &start_y,
  int &end_x,
  int &end_y)
{
  int value;
  bool bChanged = false;
  ClipLine(end_x, end_y, start_x, start_y);
  int x1 = start_x;
  int x2 = end_x;
  x1 = CLAMP_TO_UBYTE(x1);
  x2 = CLAMP_TO_UBYTE(x2);
  float y1 = (float )start_y;
  float y2 = (float )end_y;
  int xstep = (x1 > x2)? -1: 1;
  float ystep = (y2 - y1) / (float )(xstep * (x2 - x1));
  for(int x = x1; x != x2 + xstep; x += xstep)
  {
    value = 255 - (int )(y1 + 0.5);
    value = CLAMP_TO_UBYTE(value);
    setTableEntry(m_nMode, x, value);
    y1 += ystep;
    bChanged = true;
  }
  if(bChanged)
  {
    emit(tableChanged());
  }
}

//----------------------------------------------------
// callback function for mouse move events
//----------------------------------------------------
//
void QFunctionEditor::
mouseMoveEvent(QMouseEvent *m)
{
  if((m_nBorder_x >= 0) && (m_nBorder_y >= 0))
  {
    int x,w;
    int point_x = m->x();
    int point_y = m->y();
    point_x -= m_nBorder_x;
    point_y -= m_nBorder_y;
    if((point_x >= 0) && (point_x <= 255))
    {
      point_y = CLAMP_TO_UBYTE(point_y);
      if((m_bIsDragValid) && (m->buttons() & Qt::LeftButton))
      {
	DoLine(m_nStart_x, m_nStart_y, point_x, point_y);
	w = point_x - m_nStart_x;
	if(w > 0)
	{
	  x = m_nStart_x;
	}
	else
	{
	  w = -w;
	  x = point_x;
	}
	m_nStart_x = point_x;
	m_nStart_y = point_y;
	repaint(m_nBorder_x + x - 15, 0, w + 30, height());
      }
    }
  }
}

//----------------------------------------------------
// clips a line against the valid region
//----------------------------------------------------
//
void QFunctionEditor::
ClipLine(
  int &p1_x,
  int &p1_y,
  int &p2_x,
  int &p2_y)
{
  float ratio;
  if(p1_y > 255)
  {
    p1_y = 255;
    if(p1_x > 255)
    {
      p1_x = 255; 
      return;
    } 
    if(p1_x < 0)
    {
      p1_x = 0; 
      return;
    } 
  }
  if(p1_y < 0)
  {
    p1_y = 0;
    if(p1_x > 255)
    {
      p1_x = 255; 
      return;
    } 
    if(p1_x < 0)
    {
      p1_x = 0; 
      return;
    } 
  }
  if(p1_x < 0)
  {
    ratio = (float )p2_x / (float )(p2_x - p1_x);
    p1_y = p2_y + (int )(ratio * ((float )p1_y - (float )p2_y) + 0.5);
    p1_x = 0;
  }
  else if (p1_x > 255)
  {
    ratio = (float )(255 - p2_x) / (float )(p1_x - p2_x);
    p1_y = p2_y + (int )(ratio * ((float )p1_y - (float )p2_y) + 0.5);
    p1_x = 255;
  } 
}

//----------------------------------------------------
// set the histogram and update
//----------------------------------------------------
//
void QFunctionEditor::
setHistogram(
  float *pHistogram,
  float fMaxVal)
{
  m_pHistogram    = pHistogram;
  m_fHistogramMax = fMaxVal;
  repaint();
}

//----------------------------------------------------
// set the color table (interleaved) and update
//----------------------------------------------------
//
void 
QFunctionEditor::
setTable(
  unsigned char *pTable)
{
  m_pColorMapBuffer = NULL;
  m_nInternalMode = INTERNAL_MODE_COLOR_TABLE;
  if(m_pColorTableBuffer != pTable)
  { 
    m_pColorTableBuffer = pTable;	

    m_bIsDragValid = false;
    repaint();
  }
  if(m_pColorTableBuffer == NULL)
  {
    m_nInternalMode = INTERNAL_MODE_UNSPEC;
  }
}

//----------------------------------------------------
// set the color map (non-interleaved) and update
//----------------------------------------------------
//
void QFunctionEditor::
setMap(SoMFFloat *pColorMap)
{
  m_pColorTableBuffer = NULL;
  m_nInternalMode = INTERNAL_MODE_COLOR_MAP;
  if(m_pColorMapBuffer != pColorMap)
  {
    m_pColorMapBuffer = pColorMap;
    m_bIsDragValid = false;
    repaint();
  }
  if(m_pColorMapBuffer == NULL)
  {
    m_nInternalMode = INTERNAL_MODE_UNSPEC;	
  }
}

int
QFunctionEditor::
getTableEntry(EditMode chan, int idx)
{
  int	  val;
  float   fval;
  int     col;
  const char maskLSB[16] =
  {
    /*0x0*/ 0, /*0x1*/ 0, /*0x2*/ 1, /*0x3*/ 0,
    /*0x4*/ 2, /*0x5*/ 0, /*0x6*/ 1, /*0x7*/ 0,
    /*0x8*/ 3, /*0x9*/ 0, /*0xa*/ 1, /*0xb*/ 0,
    /*0xc*/ 2, /*0xd*/ 0, /*0xe*/ 1, /*0xf*/ 0
  };

  col = maskLSB[chan & 0xf];
  idx = idx * 4;
  if(m_nInternalMode == INTERNAL_MODE_COLOR_TABLE)
  {
    fval = m_pColorTableBuffer[idx + col] / 255.0;
  }
  else
  {
    fval = (*m_pColorMapBuffer)[idx + col];
  }
  switch(m_graphmode)
  {
    case GRAPH_CUBE:
      fval = cbrtf(fval);
      break;
    case GRAPH_SQUARE:
      fval = sqrtf(fval);
      break;
    case GRAPH_IDENTITY: /* FALLTHROUGH */
    default:
      break;
  }
  fval = fval * 255.0;
  val = (int )floor((fval < 0.0)? (fval - 0.5): (fval + 0.5));
  return(val);
}

void
QFunctionEditor::
setTableEntry(EditMode chan, int idx, int val)
{
  int     col;
  float	  fval;

  fval = val / 255.0;
  switch(m_graphmode)
  {
    case GRAPH_CUBE:
      fval = fval * fval * fval;
      break;
    case GRAPH_SQUARE:
      fval = fval * fval;
      break;
    case GRAPH_IDENTITY: /* FALLTHROUGH */
    default:
      break;
  }
  if(m_nInternalMode == INTERNAL_MODE_COLOR_TABLE)
  {
    fval = fval * 255.0;
    val = (int )floor((fval < 0.0)? (fval - 0.5): (fval + 0.5));
  }
  idx = idx * 4;
  for(col = 0; col < 4; ++col)
  {
    if(chan & (1 << col))
    {
      if(m_nInternalMode == INTERNAL_MODE_COLOR_TABLE)
      {
	m_pColorTableBuffer[idx + col] = (unsigned char)val;
      }
      else
      {
	m_pColorMapBuffer->set1Value(idx + col, fval);
      }
    }
  }
}

