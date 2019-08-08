#include "MyScene.h"

#include <QGraphicsView>
#include <QtDebug>

#include "ResizableRectItem.h"
#include "ResizableEllipseItem.h"
#include "ResizableLinetItem.h"
#include "ResizablePolygonItem.h"
#include "MyRandom.h"


const double MyScene::POLY_END_DISTANCE = 10;


MyScene::MyScene(QObject* parent)
	: base(parent)
	,m_sceneMode(NoMode)
	,m_itemToDraw(nullptr)
{
}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	// Left button for line, rect and ellipse merely record mouse position.
	// For polygons - we either create new poly, add new point to existing one
	// or stop drawing.

	if(event->buttons() & Qt::LeftButton)
	{
		switch(m_sceneMode)
		{
		case DrawLine : // Fall through.
		case DrawRect :
		case DrawEllipse :
			m_startPos = event->scenePos();
			break;

		case DrawPolygon :
		{
			using Item_t = ResizablePolygonItem;
			if(m_poly.empty())
			{
				m_startPos = event->scenePos();
				// No poly exists. Create new one.
				// Coordinate, local for polygon.
				m_poly.push_back({0,0});
				// One fictitious point to change it in mouseMove.
				m_poly.push_back({0,0});
				auto itemToDraw = new Item_t;
				base::addItem(itemToDraw);
				itemToDraw->setPen(QPen(MyRandom::nextRgb(), 3));
				itemToDraw->setBrush(Qt::NoBrush);
				itemToDraw->setPos(m_startPos);
				itemToDraw->setPolygon(m_poly);
				m_itemToDraw = itemToDraw;
			}
			else
			{
				// Add point to existing poly or stop drawing.
				auto itemToDraw = dynamic_cast<Item_t*>(m_itemToDraw);
				if((event->scenePos() - m_startPos).manhattanLength()
						< POLY_END_DISTANCE)
				{
					// We are to close to startPos. Remove the fictitious point.
					m_poly.pop_back();
					itemToDraw->setPolygon(m_poly);
					m_poly.clear();
					m_itemToDraw = nullptr;
				}
				else
				{
					// We are too far from startPos.
					// Make the fictitious point real to change it in mouseMove
					// and push_back new fictitious point.
					m_poly.push_back(event->scenePos() - m_startPos);
					itemToDraw->setPolygon(m_poly);
				}
			}
			break;
		}

		default :
			m_startPos = {};
			break;
		}
	}
	base::mousePressEvent(event);
	qDebug() << "MyScene" << event;
	qDebug() << "m_startPos" << m_startPos;
}

void MyScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
	// Resize current item when mouse moving.
	const auto scenePos = event->scenePos();
	if(event->buttons() & Qt::LeftButton)
	{
		switch(m_sceneMode)
		{
		case DrawLine :
		{
			// ResizableLinetItem QGraphicsLineItem
			using Item_t = ResizableLinetItem;
			auto itemToDraw = dynamic_cast<Item_t*>(m_itemToDraw);
			if(!itemToDraw)
			{
				itemToDraw = new Item_t;
				base::addItem(itemToDraw);
				itemToDraw->setPen(QPen(MyRandom::nextRgb(), 3));
				itemToDraw->setPos(m_startPos);
			}
			// Translate to item coordinate.
			itemToDraw->setLine(0,0,
								scenePos.x() - m_startPos.x(),
								scenePos.y() - m_startPos.y());
			m_itemToDraw = itemToDraw;
			break;
		}
		case DrawRect :
		{
			// ResizableRectItem QGraphicsRectItem
			using Item_t = ResizableRectItem;
			auto itemToDraw = dynamic_cast<Item_t*>(m_itemToDraw);
			if(!itemToDraw)
			{
				itemToDraw = new Item_t;
				base::addItem(itemToDraw);
				itemToDraw->setPen(Qt::NoPen);
				itemToDraw->setBrush(QBrush(MyRandom::nextRgb()));
				itemToDraw->setPos(m_startPos);
			}
			itemToDraw->setRect(0,0,
								scenePos.x() - m_startPos.x(),
								scenePos.y() - m_startPos.y());
			m_itemToDraw = itemToDraw;
			qDebug() << "setRect" << scenePos.x() - m_startPos.x() << scenePos.y() - m_startPos.y();
			break;
		}
		case DrawEllipse :
		{
			// ResizableEllipseItem QGraphicsEllipseItem
			using Item_t = ResizableEllipseItem;
			auto itemToDraw = dynamic_cast<Item_t*>(m_itemToDraw);
			if(!itemToDraw)
			{
				itemToDraw = new Item_t;
				base::addItem(itemToDraw);
				itemToDraw->setPen(Qt::NoPen);
				itemToDraw->setBrush(QBrush(MyRandom::nextRgb()));
				itemToDraw->setPos(m_startPos);
			}
			itemToDraw->setRect(0,0,
								scenePos.x() - m_startPos.x(),
								scenePos.y() - m_startPos.y());
			m_itemToDraw = itemToDraw;
			qDebug() << "setRect" << scenePos.x() - m_startPos.x() << scenePos.y() - m_startPos.y();
			break;
		}
		default :
			break;
		} // switch(m_sceneMode)
	} // event->buttons() & Qt::LeftButton
	else
	{
		// New edge of a polygon is drawn without LMB.
		switch(m_sceneMode)
		{
		case DrawPolygon :
		{
			if(m_poly.empty())
				break;
			using Item_t = ResizablePolygonItem;
			auto itemToDraw = dynamic_cast<Item_t*>(m_itemToDraw);
			// Make the fictitious point follow the mouse.
			m_poly.back() = scenePos - m_startPos;
			itemToDraw->setPolygon(m_poly);
			qDebug() << "setRect" << scenePos.x() - m_startPos.x() << scenePos.y() - m_startPos.y();
			break;
		}
		default :
			break;
		}
	}
	base::mouseMoveEvent(event);
	qDebug() << "MyScene" << event;
}


void MyScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	// Reset item only when we ain't drawing a polygon.
	if(m_poly.empty())
		m_itemToDraw = nullptr;
	base::mouseReleaseEvent(event);
	qDebug() << "MyScene" << event;
}

void MyScene::setMode(const Mode mode)
{
	// Stop drawing polygon if there exists one.
	m_poly.clear();

	m_sceneMode = mode;
	auto vMode = QGraphicsView::NoDrag;
	switch(mode)
	{
	case DrawLine : // Fall through.
	case DrawRect :
	case DrawEllipse :
	case DrawPolygon :
		makeItemsControllable(false);
		vMode = QGraphicsView::NoDrag;
		break;

	case SelectObject :
		makeItemsControllable(true);
		vMode = QGraphicsView::RubberBandDrag;
		break;

	default :
		break;
	}

	if(auto mView = base::views().at(0))
		mView->setDragMode(vMode);
}

void MyScene::makeItemsControllable(const bool b)
{
	for(auto item : items())
	{
		item->setFlag(QGraphicsItem::ItemIsSelectable, b);
		item->setFlag(QGraphicsItem::ItemIsMovable, b);
		item->setFlag(QGraphicsItem::ItemIsFocusable, b);
	}
}

void MyScene::keyPressEvent(QKeyEvent *event)
{
	switch(event->key())
	{
	case Qt::Key_Delete:
	{
		for(auto item : selectedItems())
		{
			base::removeItem(item);
			delete item;
		}
		break;
	}
	}
	base::keyPressEvent(event);
}
