#include "ResizableRectItem.h"

#include <QtDebug>

void ResizableRectItem::paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	painter->setPen(base::pen());
	painter->setBrush(base::brush());
	painter->drawRect(base::rect());
	if(base::isSelected())
	{
		// Don't call base::pait and draw selection rect manually.
		// Handles will be a bit beyond the selection rect.
		painter->setBrush(Qt::NoBrush);
		QPen pen;
		pen.setStyle(Qt::DashLine);
		pen.setColor(Qt::black);
		painter->setPen(pen);
		painter->drawRect(base::rect());
	}
	qDebug() << "ResizableRectItem::paint" << boundingRect();

}
