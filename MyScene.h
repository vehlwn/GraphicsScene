#ifndef MYSCENE_H
#define MYSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QGraphicsLineItem>

#include <memory>


// Custom scene on which you can draw items with mouse.
class MyScene : public QGraphicsScene
{
	using base = QGraphicsScene;

public:
	MyScene(QObject* parent = nullptr);

	enum Mode : int
	{
		NoMode
		,SelectObject
		,DrawLine
		,DrawRect
		,DrawEllipse
		,DrawPolygon
	};

	void setMode(const Mode mode);

protected:
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
	virtual void keyPressEvent(QKeyEvent *event) override;

private:
	// StartPos for line is its first point
	// For rects and ellipses - their top-left corner.
	// For polygons - first point in vector.
	// StartPos is in scene coordinates. When item is created, we subtract
	// event->scenePos() - m_startPos to get item's local coordinate.
	QPointF m_startPos;

	Mode m_sceneMode;

	// Current item to draw (if it exists).
	QGraphicsItem *m_itemToDraw;

	// Vector of points if user select Polygon.
	QPolygonF m_poly;

	// Distance from polygon's startPos to finish drawing a poly.
	static const double POLY_END_DISTANCE;

	void makeItemsControllable(const bool b);
};



#endif // MYSCENE_H
