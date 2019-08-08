#ifndef RESIZABLEELLIPSEITEM_H
#define RESIZABLEELLIPSEITEM_H

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

#include "BaseResizableRectEllipseItem.h"


// Inherits interactiveResize and handles to resize for.
class ResizableEllipseItem : public BaseResizableRectEllipseItem
{
	using base = BaseResizableRectEllipseItem;

public:

protected:
	virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif // RESIZABLEELLIPSEITEM_H
