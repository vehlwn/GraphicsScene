#include "ResizablePolygonItem.h"

#include <QtDebug>

#include <limits>

ResizablePolygonItem::ResizablePolygonItem()
{
	base::setAcceptHoverEvents(true);
}

QRectF ResizablePolygonItem::boundingRect() const
{
	const auto o = CornerGrabber::HANDLE_SIZE;
	return this->rect().adjusted(-o / 2, -o / 2, o / 2, o / 2);
}

QPainterPath ResizablePolygonItem::shape() const
{
	// Override shape and boundingRect in order to add HANDLE_SIZE.
	// Cannot call base::boundingRect because of infinite recursion with
	// base's shape and boundingRect.
	QPainterPath path;
	path.addRect(this->boundingRect());
	return path;

}

QRectF ResizablePolygonItem::rect() const
{
	auto x1 = std::numeric_limits<double>::infinity()
		,y1 = std::numeric_limits<double>::infinity()
		,x2 = -std::numeric_limits<double>::infinity()
		,y2 = -std::numeric_limits<double>::infinity();
	const QPolygonF poly = base::polygon();
	for(auto polyIt = poly.begin()
		; polyIt != poly.end()
		; ++polyIt)
	{
		x1 = std::min(x1, polyIt->x());
		y1 = std::min(y1, polyIt->y());
		x2 = std::max(x2, polyIt->x());
		y2 = std::max(y2, polyIt->y());
	}
	return QRectF(x1, y1, x2-x1, y2-y1);
}



bool ResizablePolygonItem::sceneEventFilter(QGraphicsItem *item, QEvent *event)
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

void ResizablePolygonItem::interactiveResize(const QPointF mousePos, const int h)
{
	// Perform shape interactive resize.
	QPolygonF poly = base::polygon();

	const QPointF from = m_handleMousePressPos;
	const QPointF to = mousePos;
	const QPointF diff = to - from;

	base::prepareGeometryChange();
	// Chage the only h-th vertex.
	poly[h] += diff;
	base::setPolygon(poly);

	updateHandlesPos();
}

void ResizablePolygonItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	base::mouseReleaseEvent(event);
}

void ResizablePolygonItem::mousePressEvent (QGraphicsSceneMouseEvent *event)
{
	m_dragStart = event->pos();
	qDebug() << "m_dragStart" << m_dragStart;
	base::mousePressEvent(event);
}

// for supporting moving the item across the scene
void ResizablePolygonItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	this->setPos(this->pos() + event->pos() - m_dragStart);
	base::mouseMoveEvent(event);
}

void ResizablePolygonItem::hoverEnterEvent(QGraphicsSceneHoverEvent *e)
{
	base::hoverEnterEvent(e);
}

void ResizablePolygonItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *e)
{
	base::hoverLeaveEvent(e);
}

void ResizablePolygonItem::updateHandlesPos()
{
	// Update current resize handles according to the vertices' position.
	const QPolygonF poly = base::polygon();
	auto polyIt = poly.begin();
	auto handIt = m_handles.begin();
	for(; polyIt != poly.end() && handIt != m_handles.end()
		; ++polyIt, ++handIt)
	{
		(*handIt)->setPos(*polyIt);
	}

	const double s = CornerGrabber::HANDLE_SIZE;
	for(auto &item : m_handles)
		item->moveBy(-s/2, -s/2);
}

void ResizablePolygonItem::focusInEvent(QFocusEvent *event)
{
	// Create handles.
	const QPolygonF poly = base::polygon();
	m_handles.clear();
	for(auto polyIt = poly.begin()
		; polyIt != poly.end()
		; ++polyIt)
	{
		auto hand = std::make_unique<CornerGrabber>(
					this
					,std::distance(poly.begin(), polyIt)
		);
		hand->installSceneEventFilter(this);
		m_handles.emplace_back(std::move(hand));
	}

	updateHandlesPos();
	base::focusInEvent(event);
	qDebug() << "ResizablePolygonItem focusInEvent";
}

void ResizablePolygonItem::focusOutEvent(QFocusEvent *event)
{
	// Delete handles.
	for(auto &item : m_handles)
	{
		item->setParentItem(nullptr);
	}
	m_handles.clear();
	base::focusOutEvent(event);
	qDebug() << "ResizablePolygonItem focusOutEvent";
}


void ResizablePolygonItem::paint (QPainter *painter, const QStyleOptionGraphicsItem *i, QWidget *w)
{
	/*painter->setPen(base::pen());
	painter->drawPolygon(base::polygon());
	if(base::isSelected())
	{
		painter->setBrush(Qt::NoBrush);
		QPen pen;
		pen.setStyle(Qt::DashLine);
		pen.setColor(Qt::black);
		painter->setPen(pen);
		painter->drawRect(this->rect());
	}*/
	base::paint(painter, i, w);
	qDebug() << "ResizablePolygonItem::paint" << boundingRect();
}
