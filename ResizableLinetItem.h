#ifndef RESIZABLELINETITEM_H
#define RESIZABLELINETITEM_H

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

// Custom line item with handles to resize for.
class ResizableLinetItem : public QGraphicsLineItem
{
	using base = QGraphicsLineItem;

public:
	ResizableLinetItem();

protected:

	virtual QRectF boundingRect() const override;
	QRectF rect() const;
	virtual QPainterPath shape() const override;

	virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

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

	std::array<std::unique_ptr<CornerGrabber>, 2>
		m_handles;
private:

};

#endif // RESIZABLELINETITEM_H
