#include "BaseResizableRectEllipseItem.h"

#include <QBrush>
#include <QtDebug>

#include "MyRandom.h"

BaseResizableRectEllipseItem::BaseResizableRectEllipseItem()
{
	base::setAcceptHoverEvents(true);
}

QRectF BaseResizableRectEllipseItem::rect() const
{
	return m_rect;
}

void BaseResizableRectEllipseItem::setRect(const QRectF &rect)
{
	m_rect = rect;
	base::update();
}

void BaseResizableRectEllipseItem::setRect(const double x, const double y, const double w, const double h)
{
	return setRect(QRectF(x, y, w, h));
}

void BaseResizableRectEllipseItem::setPen(const QPen &p)
{
	m_pen = p;
}

QPen BaseResizableRectEllipseItem::pen() const
{
	return m_pen;
}

void BaseResizableRectEllipseItem::setBrush(const QBrush &b)
{
	m_brush = b;
}

QBrush BaseResizableRectEllipseItem::brush() const
{
	return m_brush;
}

QRectF BaseResizableRectEllipseItem::boundingRect() const
{
	const auto o = CornerGrabber::HANDLE_SIZE;
	return m_rect.adjusted(-o / 2, -o / 2, o / 2, o / 2);
}

// shape is used to determine area of selection when item is focused.
QPainterPath BaseResizableRectEllipseItem::shape() const
{
	//  Returns the shape of this item as a QPainterPath in local coordinates.
	QPainterPath path;
	path.addRect(this->boundingRect());
	return path;
}

/**
  * This scene event filter has been registered with all four corner grabber items.
  * When called, a pointer to the sending corner is provided along with a generic
  * event.  A dynamic_cast is used to determine if the event type is one of the events
  * we are interrested in.
  */
bool BaseResizableRectEllipseItem::sceneEventFilter(QGraphicsItem *item, QEvent *event)
{
	qDebug() << "sceneEventFilter" << event->type();

	const auto corner = dynamic_cast<CornerGrabber*>(item);
	if(!corner)
		return false; // not expected to get here

	const auto mevent = dynamic_cast<QGraphicsSceneMouseEvent*>(event);
	if(!mevent)
		return false; // this is not one of the mouse events we are interrested in

	switch(event->type())
	{
	case QEvent::GraphicsSceneMousePress :
	{
		corner->mouseState(CornerGrabber::MouseDown);
		// Record coordinate inside the corner.
		m_handleMousePressPos = mevent->pos();
		qDebug() << "case MouseDown";
		break;
	}
	case QEvent::GraphicsSceneMouseRelease:
	{
		corner->mouseState(CornerGrabber::MouseReleased);
		qDebug() << "case MouseReleased";
		break;
	}
	case QEvent::GraphicsSceneMouseMove:
	{
		corner->mouseState(CornerGrabber::MouseMoving);
		qDebug() << "case MouseMoving";
		break;
	}
	default:
		// we dont care about the rest of the events
		return false;
	}

	if(corner->mouseState() == CornerGrabber::MouseMoving)
	{
		// Pass new coordinate relative to the corner.
		interactiveResize(mevent->pos(), corner->handleType());
		base::update();
	}
	return true;// true => do not send event to watched - we are finished with this event
}

void BaseResizableRectEllipseItem::interactiveResize(const QPointF mousePos, const int h)
{
	// Perform shape interactive resize.
	QRectF rect = this->rect();

	const QPointF from = m_handleMousePressPos;
	const QPointF to = mousePos;
	const QPointF diff = to - from;

	base::prepareGeometryChange();

	switch(h)
	{
	case CornerGrabber::HandleTopLeft :
	{
		rect.setTopLeft(rect.topLeft() + diff);
		break;
	}
	case CornerGrabber::HandleTopMiddle :
	{
		rect.setTop(rect.top() + diff.y());
		break;
	}
	case CornerGrabber::HandleTopRight :
	{
		rect.setTopRight(rect.topRight() + diff);
		break;
	}
	case CornerGrabber::HandleMiddleLeft :
	{
		rect.setLeft(rect.left() + diff.x());
		break;
	}
	case CornerGrabber::HandleMiddleRight :
	{
		rect.setRight(rect.right() + diff.x());
		break;
	}
	case CornerGrabber::HandleBottomLeft :
	{
		rect.setBottomLeft(rect.bottomLeft() + diff);
		break;
	}
	case CornerGrabber::HandleBottomMiddle :
	{
		rect.setBottom(rect.bottom() + diff.y());
		break;
	}
	case CornerGrabber::HandleBottomRight :
	{
		rect.setBottomRight(rect.bottomRight() + diff);
		break;
	}
	default: break;
	}
	this->setRect(rect);
	updateHandlesPos();
}

void BaseResizableRectEllipseItem::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
	base::mouseReleaseEvent(event);
}

// for supporting moving the item across the scene
void BaseResizableRectEllipseItem::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
	m_dragStart = event->pos();
	qDebug() << "m_dragStart" << m_dragStart;
	base::mousePressEvent(event);
}

// for supporting moving the item across the scene
void BaseResizableRectEllipseItem::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
	// Subtract to get item's local coordinate.
	this->setPos(this->pos() + event->pos() - m_dragStart);
	base::mouseMoveEvent(event);
}

void BaseResizableRectEllipseItem::hoverEnterEvent(QGraphicsSceneHoverEvent *e)
{
	base::hoverEnterEvent(e);
}

void BaseResizableRectEllipseItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *e)
{
	base::hoverLeaveEvent(e);
}

void BaseResizableRectEllipseItem::updateHandlesPos()
{
	// Update current resize handles according to the figure's size and position.
	const double s = CornerGrabber::HANDLE_SIZE;
	const QRectF r = this->rect();
	// Set position for top-left points.
	m_handles[CornerGrabber::HandleTopLeft]->     setPos(r.topLeft());
	m_handles[CornerGrabber::HandleTopMiddle]->   setPos(r.center().x(), r.top());
	m_handles[CornerGrabber::HandleTopRight]->    setPos(r.topRight());
	m_handles[CornerGrabber::HandleMiddleLeft]->  setPos(r.left(), r.center().y());
	m_handles[CornerGrabber::HandleMiddleRight]-> setPos(r.right(), r.center().y());
	m_handles[CornerGrabber::HandleBottomLeft]->  setPos(r.bottomLeft());
	m_handles[CornerGrabber::HandleBottomMiddle]->setPos(r.center().x(), r.bottom());
	m_handles[CornerGrabber::HandleBottomRight]-> setPos(r.bottomRight());
	// Move corners top-left by half of their size.
	for(auto &item : m_handles)
		if(item)
			item->moveBy(-s/2, -s/2);
}

void BaseResizableRectEllipseItem::focusInEvent(QFocusEvent *event)
{
	// Create handles.
	for(const auto hand :
		{
			CornerGrabber::HandleTopLeft
			,CornerGrabber::HandleTopMiddle
			,CornerGrabber::HandleTopRight
			,CornerGrabber::HandleMiddleLeft
			,CornerGrabber::HandleMiddleRight
			,CornerGrabber::HandleBottomLeft
			,CornerGrabber::HandleBottomMiddle
			,CornerGrabber::HandleBottomRight
		})
	{
		m_handles[hand] = std::make_unique<CornerGrabber>(this,hand);
		m_handles[hand]->installSceneEventFilter(this);
	}
	updateHandlesPos();
	base::focusInEvent(event);
	qDebug() << "myResizableItem focusInEvent";
}

void BaseResizableRectEllipseItem::focusOutEvent(QFocusEvent *event)
{
	// Delete handles.
	for(auto &item : m_handles)
	{
		if(item)
		{
			item->setParentItem(nullptr);
			item.reset();
		}
	}
	base::focusOutEvent(event);
	qDebug() << "myResizableItem focusOutEvent";
}
