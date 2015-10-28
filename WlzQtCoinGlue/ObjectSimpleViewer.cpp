#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _ObjectSimpleViewer_cpp[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         ObjectSimpleViewer.cpp
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
* \brief        Woolz object viewer.
* \ingroup      UI
*/

// project includes
#include "ObjectSimpleViewer.h"
#include "WoolzObject.h"
#include "ObjectView.h"
#include "Viewer3D.h"
#include "Viewer2D.h"
#include "Viewer2D3D.h"
//#include "Camera2D.h"
#include "ClipPlaneButton.h"
#include "Mesh3DView.h"
#include "Contour2DView.h"
#include "Contour3DView.h"
#include "ConvHull2DView.h"
#include "ConvHull3DView.h"
#include "Points2DView.h"
#include "Points3DView.h"
#include "VolumeView.h"
#include "Mesh2DView.h"
#include "ImageView.h"
#include "EmptyView.h"

// Inventor includes
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoBlinker.h>
#include <Inventor/nodes/SoSwitch.h>
#include <Inventor/nodes/SoCamera.h>
#include <Inventor/nodes/SoPerspectiveCamera.h>
#include <Inventor/sensors/SoNodeSensor.h>
#include <Inventor/events/SoMouseButtonEvent.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/nodes/SoRotation.h>
#include <Inventor/manips/SoClipPlaneManip.h>
#include <VolumeViz/nodes/SoTransferFunction.h>
#include <Inventor/nodes/SoClipPlane.h>
#include <Inventor/draggers/SoJackDragger.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>

// Qt includes
#include <QAction>
#include <QMdiArea>
#include <QSizePolicy>
#include <QMdiSubWindow>
#include <QList>
#include <QCheckBox>
#include <QSlider>
#include <QtXml/QXmlStreamWriter>
#include <QtXml/QDomElement>
#include <QRect>
#include <QLayout>
#include <QLabel>

ObjectSimpleViewer::
ObjectSimpleViewer(
  bool is3D,
  bool isBlending):
QWidget(
  0,
  Qt::SubWindow | Qt::Window),
root(NULL),
views_root_s(NULL),
views_root_t(NULL),
blinker_root(NULL),
m_clipPlaneManip(NULL),
m_clipPlane(NULL),
m_clipManipulatorButtonBi(NULL),
m_obliqueSliceButton(NULL),
m_clipLandmarkButton(NULL),
m_blinkButton(NULL),
m_mixSlider(NULL),
m_viewAll(true)
{
  QLayout *mix = NULL;
  if(isBlending)
  {
    QGridLayout * l2 = new QGridLayout;

    m_mixSlider = new QSlider(Qt::Vertical, NULL);;
    l2->addWidget(m_mixSlider, 0, 0, Qt::AlignBottom | Qt::AlignHCenter);
    m_mixSlider->setRange(0, 100);
    m_mixSlider->setSingleStep(1);
    m_mixSlider->setPageStep(10);
    m_mixSlider->setValue(100);

    QLabel * label = new QLabel("Mix", NULL);
    label->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    label->setMargin(0);
    l2->addWidget(label, 1, 0, Qt::AlignTop | Qt::AlignHCenter);
    mix = l2;
    QWidget::connect(m_mixSlider, SIGNAL(valueChanged(int)),
	this, SLOT(transparencyChanged(int)));
  }

  QList <QWidget*> *buttons=new QList <QWidget*>;
  QIcon icon;
  //blink button on/off
  icon.addPixmap(QPixmap(QString::fromUtf8(":/icons/images/blink.png")),
      QIcon::Normal);
  m_blinkButton=new QPushButton(this);
  m_blinkButton->setCheckable(true);
  m_blinkButton->setChecked(false);
  m_blinkButton->setIcon(icon);
  m_blinkButton->setToolTip("Flash source on/off");
  m_blinkButton->setVisible(false);
  connect(m_blinkButton, SIGNAL(clicked(bool)),
          this, SLOT(setFlashSourceTarget(bool)));

  if(is3D)
  {
    m_clipManipulatorButtonBi=new ClipPlaneButtonBiDirection(this);
    m_clipManipulatorButtonBi->setCheckable(false);
    m_clipManipulatorButtonBi->setToolTip("Clip manipulator on/clip/off");
    buttons->append(m_clipManipulatorButtonBi);

    icon.addPixmap(QPixmap(
        QString::fromUtf8(":/icons/images/orthosliceon.png")),
	QIcon::Normal, QIcon::Off);
    icon.addPixmap(QPixmap(
        QString::fromUtf8(":/icons/images/orthosliceoff.png")),
	QIcon::Normal, QIcon::On);
    m_obliqueSliceButton=new QPushButton(this);
    m_obliqueSliceButton->setCheckable(true);
    m_obliqueSliceButton->setIcon(icon);
    m_obliqueSliceButton->setToolTip("Oblique slice on/off");

    buttons->append(m_obliqueSliceButton);

    //landmark clip on/off
    icon.addPixmap(QPixmap(
        QString::fromUtf8(":/icons/images/cuttinglandmarksoff.png")),
	QIcon::Normal, QIcon::Off);
    icon.addPixmap(
        QPixmap(QString::fromUtf8(":/icons/images/cuttinglandmarkson.png")),
	QIcon::Normal, QIcon::On);
    m_clipLandmarkButton=new QPushButton(this);
    m_clipLandmarkButton->setCheckable(true);
    m_clipLandmarkButton->setChecked(true);
    m_clipLandmarkButton->setIcon(icon);
    m_clipLandmarkButton->setToolTip("Clip landmarks on/off");
    m_clipLandmarkButton->setVisible(false);
    buttons->append(m_clipLandmarkButton);

    buttons->append(m_blinkButton);
    m_viewer = (Viewer2D3D*) new Viewer3D(this, mix, buttons);
    connect(m_clipManipulatorButtonBi,
            SIGNAL(stateChanged(ClipPlaneButton::statetype)),
	    this, SLOT(stateChanged(ClipPlaneButton::statetype)));
  }
  else
  {
    buttons->append(m_blinkButton);
    m_viewer = (Viewer2D3D *)new Viewer2D(this, mix, buttons);
  }
  Q_ASSERT(m_viewer);
  root = new SoSeparator;
  Q_ASSERT(root);
  root->ref();

  SoEventCallback * cb = new SoEventCallback;
  cb->addEventCallback(SoMouseButtonEvent::getClassTypeId(),
      mouseEventCB, this);
  root->insertChild(cb, 0);

  blinker_root = new SoSwitch;
  Q_ASSERT(blinker_root);
  blinker_root->ref();
  blinker_root->whichChild = 0; // select non blinking

  SoSeparator* noblinker = new SoSeparator;
  Q_ASSERT(noblinker);
  blinker_root->addChild(noblinker);

  SoBlinker* blinker = new SoBlinker;
  Q_ASSERT(blinker);
  blinker->on = true;

  blinker_root->addChild(blinker);

  views_root_s = new SoSeparator;
  Q_ASSERT(views_root_s);
  blinker->addChild(views_root_s);
  noblinker->addChild(views_root_s);

  views_root_t = new SoSeparator;
  Q_ASSERT(views_root_t);
  blinker->addChild(views_root_t);
  noblinker->addChild(views_root_t);

  root->addChild(blinker_root);

  m_viewer ->setSceneGraph(root);

  //this has to go in the Editor
  m_activateAction = new QAction (this);
  m_activateAction -> setCheckable(true);
  connect(m_activateAction, SIGNAL(triggered()), this, SLOT(show()));
  connect(m_activateAction, SIGNAL(triggered()), this, SLOT(raise()));
  connect(m_activateAction, SIGNAL(triggered()), this, SLOT(setFocus()));
  this->setSizePolicy (  QSizePolicy::Expanding, QSizePolicy::Expanding );
}

ObjectSimpleViewer::
~ObjectSimpleViewer()
{
  if(m_viewer)
  {
    delete m_viewer;
    m_viewer = NULL;
  }
  if(m_clipPlane)
  {
     m_clipPlane->unref();
     m_clipPlane = NULL;
  }
  if(m_clipPlaneManip)
  {
     m_clipPlaneManip->unref();
     m_clipPlaneManip = NULL;
  }
  for(int i = 0; i < views.size(); ++i)
  {
    delete views.at(i);
  }
  views.clear();
  root->unref();
  blinker_root->unref();

  removedViewerFinished(); //force to be called now
  this->disconnect();
}

void ObjectSimpleViewer::
addObject(
  WoolzObject * object,
  bool doViewAll,
  ObjectView *previousView)
{
  if(accepting(object))
  {
    ObjectView *view;
    view = Factory(this, object);

    //if object has no data or can not be visualised
    //then generate no view
    if(!view)
    {
        return;
    }
    views.append(view);
    if (object->isWarped())
    {
      views_root_s->insertChild(view->getSceneGraph(true),0);
    }
    else
    {
      views_root_t->insertChild(view->getSceneGraph(true),0);
    }

    if(previousView)
    {
        view->setVisibility(previousView->getVisibility());
        view->setTransparency(previousView->getTransparency());
    }
    else
    {
      int transparency = initialTransparency(object);
      if(transparency != 100)
      {
        view->setVisibility(true);
        view->setTransparency(transparency);
      }
      else
      {
        view->setVisibility(false);
        view->setTransparency(0);
      }
    }

    connect(view, SIGNAL(viewPropertyChanged()),
            this, SLOT(viewPropertyChanged()));
    connect(view, SIGNAL(regerateView()),
            this, SLOT(regerateView()));
    emit addedView(view);
    if(m_viewAll)
    {
      m_viewer->viewAll();
      if(doViewAll)
      {
	m_viewAll = false;
      }
    }
  }
}

void ObjectSimpleViewer::
removedObject(
  WoolzObject * object)
{
  if(object)
  {
    for(int i = 0; i < views.size(); ++i)
    {
      ObjectView* view = dynamic_cast<ObjectView*>(views.at(i));
      if(view && view->isUsing(object))
      {// remove
	removedViewStart(view); // must be before removeAt, otherwise row
	                        // number can't be found
	if (object->isWarped())
	{
	  views_root_s->removeChild(view->getSceneGraph(false));
	}
	else
	{
	  views_root_t->removeChild(view->getSceneGraph(false));
	}

	views.removeAt(i);
	removedViewFinish(view); // must be before removeAt, otherwise row
	                         // number can't be found
	delete view;
      }
    }
  }
}

void ObjectSimpleViewer::
setWindowTitle(
  QString title)
  {
  QWidget::setWindowTitle(title);
  m_activateAction->setText(title);
}

void ObjectSimpleViewer::
closeEvent(
  QCloseEvent * /*event*/)
{
 emit removedViewerStart(); //force to be called now
}

void ObjectSimpleViewer::
viewPropertyChanged()
{
  ObjectView * senderView = qobject_cast<ObjectView*>(sender());
  if(senderView)
  {
    emit changedObjectView(senderView);
  }
}

void ObjectSimpleViewer::
activate()
{
  m_activateAction->trigger();
  QMdiSubWindow * mdiSubWindow = qobject_cast<QMdiSubWindow *>(parent());
  mdiSubWindow->raise();
  mdiSubWindow->activateWindow();
}

void ObjectSimpleViewer::
mouseEventCB(
  void *data,
  SoEventCallback * event)
{
  Q_ASSERT(data);
  if(data)
  {
    ((ObjectSimpleViewer*)data)->notifyMouseEvent(event);
  }
}

void ObjectSimpleViewer::
notifyMouseEvent(
  SoEventCallback * /*event*/)
{
}

void ObjectSimpleViewer::
setEditingMode()
{
  if(m_viewer->isViewing())
  {
    m_viewer->setViewing(false);
  }
}

void ObjectSimpleViewer::
setViewingMode()
{
  if(!m_viewer->isViewing())
  {
    m_viewer->setViewing(true);
  }
}

void ObjectSimpleViewer::
switchModes()
{
  m_viewer->setViewing(!(m_viewer->isViewing()));
}

void ObjectSimpleViewer::
regerateView()
{
  ObjectView * senderView = qobject_cast<ObjectView*>(sender());
  if(!senderView)
  {
    return;
  }
  WoolzObject * object = senderView->object();

  if(object->isWarped())
  {
    views_root_s->removeChild(senderView->getSceneGraph(false));
  }
  else
  {
    views_root_t->removeChild(senderView->getSceneGraph(false));
  }

  emit removedViewStart(senderView);
  views.removeOne(senderView);
  emit removedViewFinish(senderView);

  addObject(object, false, senderView);
  delete senderView;
}

void ObjectSimpleViewer::
stateChanged(
  ClipPlaneButton::statetype state)
{
  switch(state)
  {
    case ClipPlaneButton::ClipOn:
      {
	if(!m_clipPlaneManip)
	{
	  //create at first use
	  m_clipPlaneManip = new SoClipPlaneManip;
	  m_clipPlaneManip->ref();
	  SoGetBoundingBoxAction ba(m_viewer->getViewportRegion());
	  ba.apply(blinker_root);
	  SbBox3f box = ba.getBoundingBox();
	  m_clipPlaneManip->setValue(box, SbVec3f(1.0f, 0.0f, 0.0f), 1.00f);
	}
	SbPlane plane = m_clipPlaneManip->plane.getValue();
	m_clipPlaneManip->plane.setValue(SbPlane(-plane.getNormal(),
	    -plane.getDistanceFromOrigin()));
	emit addedClipPlane(m_clipPlaneManip);  //remove ortho slices
      }
      break;
    case ClipPlaneButton::ClipOff:
      emit addedClipPlane(NULL);  //remove ortho slices
      break;
    case ClipPlaneButton::ClipOnly:
      if(!m_clipPlane)
      {
        //create at first use
	if(!m_clipPlaneManip)
	{
	  //create at first use
	  m_clipPlaneManip = new SoClipPlaneManip;
	  m_clipPlaneManip->ref();
	}
	m_clipPlane = new SoClipPlane;
	m_clipPlane->ref();
	m_clipPlane->plane.connectFrom(&(m_clipPlaneManip->plane));
      }
      emit addedClipPlane(m_clipPlane);  //remove ortho slices
      break;
  }
}

ObjectView* ObjectSimpleViewer::
Factory(
  QObject * parent,
  WoolzObject *object)
{
  ObjectView* view = NULL;

  if(object->isEmpty())
  {
    return new EmptyView(parent, object);
  }

  if(object->isMesh())
  {
    if(object->is3D())
    {
      Mesh3DView * mesh = new Mesh3DView(parent, object);
      if(m_clipManipulatorButtonBi->state() == ClipPlaneButton::ClipOnly)
      {
	Q_ASSERT(m_clipPlane);
	mesh->addedClipPlane(m_clipPlane);
      }
      else
      if(m_clipManipulatorButtonBi->state() == ClipPlaneButton::ClipOn)
      {
	Q_ASSERT(m_clipPlaneManip);
	mesh->addedClipPlane(m_clipPlaneManip);
      }
      view = mesh;
    }
    else
    {
      view = new Mesh2DView(parent, object);
    }
  }
  else
  if(object->isContour())
  {
    if(object->is3D())
    {
      Contour3DView * contour = new Contour3DView(parent, object);
      if(m_clipManipulatorButtonBi->state() == ClipPlaneButton::ClipOnly)
      {
	Q_ASSERT(m_clipPlane);
	contour->addedClipPlane(m_clipPlane);
      }
      else if (m_clipManipulatorButtonBi->state() == ClipPlaneButton::ClipOn)
      {
	Q_ASSERT(m_clipPlaneManip);
	contour->addedClipPlane(m_clipPlaneManip);
      }
      view = contour;
    }
    else
    {
      view = new Contour2DView(parent, object);
    }
  }
  else if (object->isConvHull())
  {
    if(object->is3D())
    {
      ConvHull3DView * convhull = new ConvHull3DView(parent, object);
      if(m_clipManipulatorButtonBi->state() == ClipPlaneButton::ClipOnly)
      {
	Q_ASSERT(m_clipPlane);
	convhull->addedClipPlane(m_clipPlane);
      }
      else if(m_clipManipulatorButtonBi->state() == ClipPlaneButton::ClipOn)
      {
	Q_ASSERT(m_clipPlaneManip);
	convhull->addedClipPlane(m_clipPlaneManip);
      }
      view = convhull;
    }
    else
    {
      view = new ConvHull2DView(parent, object);
    }
  }
  else if (object->isPoints())
  {
    if(object->is3D())
    {
      Points3DView * points = new Points3DView(parent, object);
      if(m_clipManipulatorButtonBi->state() == ClipPlaneButton::ClipOnly)
      {
	Q_ASSERT(m_clipPlane);
	points->addedClipPlane(m_clipPlane);
      }
      else if(m_clipManipulatorButtonBi->state() == ClipPlaneButton::ClipOn)
      {
	Q_ASSERT(m_clipPlaneManip);
	points->addedClipPlane(m_clipPlaneManip);
      }
      view = points;
    }
    else
    {
      view = new Points2DView(parent, object);
    }
  }
  else if (object->isValue())
  {
    if(object->is3D())
    {
      VolumeView *volume = new VolumeView(parent, object);
      connect(m_obliqueSliceButton, SIGNAL(clicked(bool)),
              volume, SLOT(setObliqueSlice(bool)));
      if(m_clipManipulatorButtonBi->state() == ClipPlaneButton::ClipOnly)
      {
	Q_ASSERT(m_clipPlane);
	volume->addedClipPlane(m_clipPlane);
      }
      else if(m_clipManipulatorButtonBi->state() == ClipPlaneButton::ClipOn)
      {
	Q_ASSERT(m_clipPlaneManip);
	volume->addedClipPlane(m_clipPlaneManip);
      }
      volume->setObliqueSlice(m_obliqueSliceButton->isChecked());
      view = volume;
    }
    else
    {
      view = new ImageView(parent, object);
    }
  }
  if(!view)
  {
    view = new EmptyView(parent, object);
  }
  return(view);
}

void ObjectSimpleViewer::
setBackgroundColour()
{
  QColor color = getBackgroundColour();
  m_viewer->setBackgroundColor(SbColor(color.red()/255.0f,
                                       color.green()/255.0f,
				       color.blue()/255.0f));
}

QColor ObjectSimpleViewer::
getBackgroundColour()
{
  return QColor(0, 0, 0);
}

void ObjectSimpleViewer::
init()
{
  setBackgroundColour();
}

bool ObjectSimpleViewer::
saveAsXml(
  QXmlStreamWriter *xmlWriter)
{
  Q_ASSERT(xmlWriter);
  xmlWriter->writeTextElement("Title", windowTitle());
  if(m_mixSlider)
  {
    xmlWriter->writeTextElement("Mix",
	QString("%1").arg(m_mixSlider->value()));
  }
  xmlWriter->writeStartElement("Geometry");
  const QRect geom = parentWidget()->geometry();
  xmlWriter->writeTextElement("X", QString("%1").arg(geom .x()));
  xmlWriter->writeTextElement("Y", QString("%1").arg(geom .y()));
  xmlWriter->writeTextElement("Width", QString("%1").arg(geom .width()));
  xmlWriter->writeTextElement("Height", QString("%1").arg(geom .height()));
  xmlWriter->writeEndElement();

  if(m_clipPlaneManip)
  {
    xmlWriter->writeStartElement("ClipPlane");

    SbPlane plane = m_clipPlaneManip->plane.getValue();
    SbVec3f vec = plane.getNormal();
    xmlWriter->writeStartElement("Normal");
    xmlWriter->writeTextElement("X", QString("%1").arg(vec[0]));
    xmlWriter->writeTextElement("Y", QString("%1").arg(vec[1]));
    xmlWriter->writeTextElement("Z", QString("%1").arg(vec[2]));
    xmlWriter->writeEndElement();

    xmlWriter->writeTextElement("Distance",
        QString("%1").arg(plane.getDistanceFromOrigin()));

    vec = m_clipPlaneManip->draggerPosition.getValue();
    xmlWriter->writeStartElement("Position");
    xmlWriter->writeTextElement("X", QString("%1").arg(vec[0]));
    xmlWriter->writeTextElement("Y", QString("%1").arg(vec[1]));
    xmlWriter->writeTextElement("Z", QString("%1").arg(vec[2]));
    xmlWriter->writeEndElement();
    SoDragger *dragger = m_clipPlaneManip->getDragger();
    if(dragger && dragger->isOfType(SoJackDragger::getClassTypeId()))
    {
      SoJackDragger *jd = (SoJackDragger*)dragger;
      vec = jd->scaleFactor.getValue();
      xmlWriter->writeStartElement("Scalefactor");
      xmlWriter->writeTextElement("X", QString("%1").arg(vec[0]));
      xmlWriter->writeTextElement("Y", QString("%1").arg(vec[1]));
      xmlWriter->writeTextElement("Z", QString("%1").arg(vec[2]));
      xmlWriter->writeEndElement();
    }
    xmlWriter->writeTextElement("Distance",
        QString("%1").arg(plane.getDistanceFromOrigin()));

    xmlWriter->writeEndElement();
  }

  for(int i = 0; i < views.size(); ++i)
  {
    views.at(i)->saveAsXml(xmlWriter);
  }
  return(true);
}

bool ObjectSimpleViewer::
parseDOMLine(
  const QDomElement &element)
{
  bool st = true;

  if(element.tagName() == "Title")
  {
    setWindowTitle(element.text());
  }
  else if (element.tagName() == "Mix")
  {
    if(m_mixSlider)
    {
      m_mixSlider->setValue(element.text().toInt());
    }
  }
  else if (element.tagName() == "Geometry")
  {
    parseGeometry(element);
  }
  else if (element.tagName() == "ClipPlane")
  {
    parseClipPlane(element);
  }
  else if (element.tagName() == View::xmlTag)
  {
    parseViews(element);
  }
  else
  {
    st = false;
  }
  return(st);
}

bool ObjectSimpleViewer::
parseClipPlane(
  const QDomElement &element)
{
  float distance=0;
  SbVec3f pos;
  SbVec3f normal;
  SbVec3f scale(1.0f, 1.0f, 1.0f);

  QDomNode child = element.firstChild();
  while(!child.isNull())
  {
    const QDomElement &element = child.toElement();
    if(element.tagName() == "Distance")
    {
      distance = element.text().toFloat();
    }
    else if(element.tagName() == "Position")
    {
      pos = parseSbVec3f(element);
    }
    else if(element.tagName() == "Normal")
    {
      normal = parseSbVec3f(element);
    }
    else if(element.tagName() == "Scalefactor")
    {
      scale = parseSbVec3f(element);
    }
    child = child.nextSibling();
  }
  if(!m_clipPlaneManip)
  {
    //create at first use
    m_clipPlaneManip = new SoClipPlaneManip;
    m_clipPlaneManip->ref();
    m_clipPlaneManip->plane.setValue(SbPlane(normal, distance));
    m_clipPlaneManip->draggerPosition = pos;
    SoDragger *dragger = m_clipPlaneManip->getDragger();
    if(dragger && dragger->isOfType(SoJackDragger::getClassTypeId()))
    {
      SoJackDragger *jd = (SoJackDragger*)dragger;
      jd->scaleFactor.setValue(scale);
    }
  }
  return(true);
}

SbVec3f ObjectSimpleViewer::
parseSbVec3f(
  const QDomElement &element) const
{
  float x=0.0f, y=0.0f, z=0.0f;

  QDomNode child = element.firstChild();
  while(!child.isNull())
  {
    const QDomElement &element = child.toElement();
    if(element.tagName() == "X")
    {
      x = element.text().toFloat();
    }
    else if (element.tagName() == "Y")
    {
      y = element.text().toFloat();
    }
    else if (element.tagName() == "Z")
    {
      z = element.text().toFloat();
    }
    child = child.nextSibling();
  }
  return(SbVec3f(x,y,z));
}

bool ObjectSimpleViewer::
parseGeometry(
  const QDomElement &element)
{
  QRect geom(-1,-1,-1,-1);
  QDomNode child = element.firstChild();
  while(!child.isNull())
  {
    const QDomElement &element = child.toElement();
    if(element.tagName() == "X")
    {
      geom.setX(element.text().toInt());
    }
    else if(element.tagName() == "Y")
    {
      geom.setY(element.text().toInt());
    }
    else if(element.tagName() == "Width")
    {
      geom.setWidth(element.text().toInt());
    }
    else if(element.tagName() == "Height")
    {
      geom.setHeight(element.text().toInt());
    }
    child = child.nextSibling();
  }
  if(geom.x()>=0 && geom.y()>=0 && geom.width()>=0 && geom.height()>=0)
  {
    parentWidget()->setGeometry(geom);
  }
  return(true);
}

bool ObjectSimpleViewer::
parseViews(
  const QDomElement& element)
{
  int objID = element.firstChildElement("ObjectID").toElement().text().toInt();
  for(int i = 0; i < views.size(); ++i)
  {
    ObjectView* view = views.at(i);
    if(view && view->isUsing(objID))
    {
      view->parseDOM(element);
    }
  }
  return(true);
}

bool ObjectSimpleViewer::
parseDOM(
  const QDomElement &element)
{
  QDomNode child = element.firstChild();
  while(!child.isNull())
  {
    parseDOMLine(child.toElement());
    child = child.nextSibling();
  }
  return(true);
}

void ObjectSimpleViewer::
setFlashSourceTarget(
  bool on)
{
  blinker_root->whichChild = (on)? 1 : 0;
}

void ObjectSimpleViewer::
alphaChanged(bool alpha)
{
  if(m_viewer)
  {
    if(alpha)
    {
      m_viewer->setTransparencyType(SoGLRenderAction::SORTED_LAYERS_BLEND);
    }
    else
    {
      m_viewer->setTransparencyType(SoGLRenderAction::SORTED_OBJECT_BLEND);
    }
    m_viewer->setAlphaChannel(alpha);
  }
}

