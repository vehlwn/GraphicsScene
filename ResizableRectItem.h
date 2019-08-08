#ifndef vIIlgRxeJrCHIhkvdIvR
#define vIIlgRxeJrCHIhkvdIvR

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
class ResizableRectItem : public BaseResizableRectEllipseItem
{
	using base = BaseResizableRectEllipseItem;

public:

protected:
	virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
};

#endif // STATEBOX_H
