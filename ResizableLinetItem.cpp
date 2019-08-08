#include "ResizableLinetItem.h"

#include <QtDebug>

ResizableLinetItem::ResizableLinetItem()
{
	base::setAcceptHoverEvents(true);
}

QRectF ResizableLinetItem::boundingRect() const
{
	const auto o = CornerGrabber::HANDLE_SIZE;
	return rect().adjusted(-o / 2, -o / 2, o / 2, o / 2);
}

QRectF ResizableLinetItem::rect() const
{
	const QLineF r = base::line();
	const double
			 x = std::min(r.p1().x(), r.p2().x())
			,y = std::min(r.p1().y(), r.p2().y())
			,w = std::abs(r.p1().x() - r.p2().x())
			,h = std::abs(r.p1().y() - r.p2().y());
	return QRectF(x, y, w, h);
}

QPainterPath ResizableLinetItem::shape() const
{
	QPainterPath path;
	path.addRect(this->boundingRect());
	return path;
}

// Capture events from handles.
bool ResizableLinetItem::sceneEventFilter(QGraphicsItem *item, QEvent *event)
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
	// if the mouse went down, record the x,y coords of the press, record it inside the corner object
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

void ResizableLinetItem::interactiveResize(const QPointF mousePos, const int h)
{
	// Perform shape interactive resize.
	QLineF line = base::line();

	const QPointF from = m_handleMousePressPos;
	const QPointF to = mousePos;
	const QPointF diff = to - from;

	base::prepareGeometryChange();

	switch(h)
	{
	case 0 :
	{
		line.setP1(line.p1() + diff);
		break;
	}
	case 1 :
	{
		line.setP2(line.p2() + diff);
		break;
	}
	default: break;
	}
	base::setLine(line);
	updateHandlesPos();
}

void ResizableLinetItem::mouseReleaseEvent ( QGraphicsSceneMouseEvent * event )
{
	base::mouseReleaseEvent(event);
}

void ResizableLinetItem::mousePressEvent ( QGraphicsSceneMouseEvent * event )
{
	m_dragStart = event->pos();
	qDebug() << "m_dragStart" << m_dragStart;
	base::mousePressEvent(event);
}

// for supporting moving the item across the scene
void ResizableLinetItem::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
	this->setPos(this->pos() + event->pos() - m_dragStart);
	base::mouseMoveEvent(event);
}

void ResizableLinetItem::hoverEnterEvent(QGraphicsSceneHoverEvent *e)
{
	base::hoverEnterEvent(e);
}

void ResizableLinetItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *e)
{
	base::hoverLeaveEvent(e);
}

void ResizableLinetItem::updateHandlesPos()
{
	// Update current resize handles according to the shape size and position.
	const double s = CornerGrabber::HANDLE_SIZE;
	const QLineF r = base::line();
	m_handles[0]->setPos(r.p1());
	m_handles[1]->setPos(r.p2());
	for(auto &item : m_handles)
		if(item)
			item->moveBy(-s/2, -s/2);
}

void ResizableLinetItem::focusInEvent(QFocusEvent *event)
{
	// Create handles.
	for(int i = 0; i < 2; i++)
	{
		m_handles[i] = std::make_unique<CornerGrabber>(this,i);
		m_handles[i]->installSceneEventFilter(this);
	}
	updateHandlesPos();
	base::focusInEvent(event);
	qDebug() << "ResizableLinetItem focusInEvent";
}

void ResizableLinetItem::focusOutEvent(QFocusEvent *event)
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
	qDebug() << "ResizableLinetItem focusOutEvent";
}


void ResizableLinetItem::paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	painter->setPen(base::pen());
	painter->drawLine(base::line());
	if(base::isSelected())
	{
		// Dont call base::pait and draw selection rect manually.
		// Handles will be a bit beyond the selection rect.
		painter->setBrush(Qt::NoBrush);
		QPen pen;
		pen.setStyle(Qt::DashLine);
		pen.setColor(Qt::black);
		painter->setPen(pen);
		painter->drawRect(this->rect());
	}
	qDebug() << "ResizableLinetItem::paint" << boundingRect();
}
