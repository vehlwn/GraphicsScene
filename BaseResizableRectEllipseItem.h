#ifndef MYRESIZABLEITEM_H
#define MYRESIZABLEITEM_H

#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QColor>
#include <QPainter>
#include <QPen>
#include <QPointF>
#include <QFocusEvent>

#include <array>
#include <memory>

#include "cornergrabber.h"
#include "MyRandom.h"


// Abstract class for my resizable rectangular and ellipses.
// It owns array of CornerGrabbers and recieves their mouse events.
class BaseResizableRectEllipseItem : public QGraphicsItem
{
	using base = QGraphicsItem;

public:
	BaseResizableRectEllipseItem();

	QRectF rect() const;
	void setRect(const QRectF &setRect);
	void setRect(const double x, const double y, const double w, const double h);

	void setPen(const QPen& p);
	QPen pen() const;

	void setBrush(const QBrush& b);
	QBrush brush() const;

protected:

	// virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) =0;
	virtual QRectF boundingRect() const override;
	virtual QPainterPath shape() const override;

	virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event ) override;
	virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ) override;
	virtual void mouseMoveEvent ( QGraphicsSceneMouseEvent * event ) override;
	virtual void mousePressEvent (QGraphicsSceneMouseEvent * event ) override;
	virtual void mouseReleaseEvent (QGraphicsSceneMouseEvent * event ) override;
	virtual void focusInEvent(QFocusEvent *event) override;
	virtual void focusOutEvent(QFocusEvent *event) override;

	virtual bool sceneEventFilter (QGraphicsItem * item, QEvent * event ) override;

	void updateHandlesPos();
	void interactiveResize(const QPointF mousePos, const int h);

	QPointF m_handleMousePressPos;
	QPointF m_dragStart;

	std::array<
		std::unique_ptr<CornerGrabber>
		,CornerGrabber::HandleCount
	>
		m_handles;


private:
	QPen m_pen;
	QBrush m_brush;
	QRectF m_rect;
};

#endif // MYRESIZABLEITEM_H
