#ifndef CORNERGRABBER_H
#define CORNERGRABBER_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneMouseEvent>
#include <QColor>
#include <QPainter>
#include <QPen>
#include <QPointF>

// Small rectangle to resize my figures.
// Its mouse events are captured by parent figure with sceneEventFilter.
class CornerGrabber : public QGraphicsItem
{
	using base = QGraphicsItem;
	
public:
	enum Handle : int
	{
		NoHandle
		,HandleTopLeft
		,HandleTopMiddle
		,HandleTopRight
		,HandleMiddleLeft
		,HandleMiddleRight
		,HandleBottomLeft
		,HandleBottomMiddle
		,HandleBottomRight
		,HandleCount
	};
	int handleType() const;

	// 8 corners for rectangle and ellipse.
	// Unknown number of corners for polygon.
	explicit CornerGrabber(QGraphicsItem *parent = nullptr,  const int corner = NoHandle);


	// mouseState is set by parent figure.
	enum MouseState
	{
		MouseReleased
		,MouseDown
		,MouseMoving
	};
	void mouseState(const MouseState s);
	MouseState  mouseState() const;

	static const double HANDLE_SIZE;

	virtual QRectF boundingRect() const override;

private:

	virtual void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

	// Make corner active/inactive when mouse enters/leaves.
	virtual void hoverEnterEvent ( QGraphicsSceneHoverEvent * event ) override;
	virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event ) override;

	// Index of current handle.
	int m_corner;

	MouseState m_mouseState;

	// Color to fill with.
	QColor m_color;

	static const QColor ACTIVE_COLOR, INACTIVE_COLOR;
};

#endif // CORNERGRABBER_H
