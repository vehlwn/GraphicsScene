#ifndef RESIZABLEPOLYGONITEM_H
#define RESIZABLEPOLYGONITEM_H

#include <QGraphicsItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QColor>
#include <QPainter>
#include <QPen>
#include <QPointF>
#include <QFocusEvent>

#include <memory>
#include <vector>

#include "cornergrabber.h"

// Custom polygon with vector of handles to resize for.
class ResizablePolygonItem : public QGraphicsPolygonItem
{
	using base = QGraphicsPolygonItem;

public:
	ResizablePolygonItem();

protected:

	virtual QRectF boundingRect() const override;
	virtual QPainterPath shape() const override;
	QRectF rect() const;


	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
	virtual void focusInEvent(QFocusEvent *event) override;
	virtual void focusOutEvent(QFocusEvent *event) override;

	virtual bool sceneEventFilter(QGraphicsItem *item, QEvent *event) override;

	void updateHandlesPos();
	void interactiveResize(const QPointF mousePos, const int h);

	QPointF m_handleMousePressPos;
	QPointF m_dragStart;

	std::vector<std::unique_ptr<CornerGrabber>>
		m_handles;
private:
};

#endif // RESIZABLEPOLYGONITEM_H
