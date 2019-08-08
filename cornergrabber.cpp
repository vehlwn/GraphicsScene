#include "cornergrabber.h"

#include <QtDebug>

const double
	CornerGrabber::HANDLE_SIZE = 8;

const QColor CornerGrabber::ACTIVE_COLOR = Qt::red
	, CornerGrabber::INACTIVE_COLOR = Qt::black;


CornerGrabber::CornerGrabber(QGraphicsItem *parent,  const int corner)
	: base(parent)
	,m_corner(corner)
	,m_mouseState(MouseReleased)
	,m_color(INACTIVE_COLOR)
{
   base::setAcceptHoverEvents(true);
}

void CornerGrabber::mouseState(const MouseState s)
{
    m_mouseState = s;
}

CornerGrabber::MouseState 
	CornerGrabber::mouseState() const
{
    return m_mouseState;
}

int CornerGrabber::handleType() const
{
	return m_corner;
}

void CornerGrabber::hoverLeaveEvent(QGraphicsSceneHoverEvent *e)
{
	m_color = INACTIVE_COLOR;
    base::update();
	base::hoverLeaveEvent(e);
}

void CornerGrabber::hoverEnterEvent(QGraphicsSceneHoverEvent *e)
{
	m_color = ACTIVE_COLOR;
    base::update();
	base::hoverEnterEvent(e);
}

QRectF CornerGrabber::boundingRect() const
{
    return QRectF(0,0,HANDLE_SIZE,HANDLE_SIZE);
}

void CornerGrabber::paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	painter->setPen(QPen(m_color, 0, Qt::NoPen));
    painter->fillRect(boundingRect(), m_color);
}
