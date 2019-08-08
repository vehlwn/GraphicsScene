#include "ResizableEllipseItem.h"

#include <QtDebug>


// Draw ellipse.
void ResizableEllipseItem::paint (QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	painter->setPen(base::pen());
	painter->setBrush(base::brush());
	painter->drawEllipse(base::rect());
	if(base::isSelected())
	{
		// Dont call base::pait ('cause it's pure virtual) so draw selection
		// rect manually. Handles will be a bit beyond the selection rect.
		painter->setBrush(Qt::NoBrush);
		QPen pen;
		pen.setStyle(Qt::DashLine);
		pen.setColor(Qt::black);
		painter->setPen(pen);
		painter->drawRect(base::rect());
	}
	qDebug() << "ResizableEllipseItem::paint" << boundingRect();

}
