#include "MainWindow.h"
#include "ui_MainWindow.h"


#include "ResizableRectItem.h"
#include "ResizableLinetItem.h"
#include "ResizablePolygonItem.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	m_scene = new MyScene(this);
	m_scene->setSceneRect(0,0,200,200);
	m_view = new QGraphicsView(m_scene);
	//m_view->setRenderHints(QPainter::Antialiasing);
	m_view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	m_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	//m_view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
	this->setCentralWidget(m_view);

	createActions();
	createConnections();
	createToolBar();


	// If you add my custom item programmatically and then draw new item with a
	// mouse, you cannot resize the new item. It wont accept mouse events anymore.
	// It's a bug and I don't know how to fix it.
/*
	auto item = new ResizableRectItem;
	m_scene->addItem(item);
	item->setPen(Qt::NoPen);
	item->setBrush(QBrush(MyRandom::nextRgb()));
	item->setPos(0,0);
	item->setRect(0,0,200,100);
*/


//	auto item = new QGraphicsPolygonItem;
	/*auto item = new ResizablePolygonItem;
	m_scene->addItem(item);
	item->setPen(QPen(MyRandom::nextRgb(), 3));
	item->setPos(0,0);
	QPolygonF p;
	p << QPointF{0,0} << QPointF{100,-50} << QPointF{10,200} << QPointF{-100,200};
	item->setPolygon(p);
*/

}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::createActions()
{
	QAction *action;

	action = new QAction("Select object", this);
	action->setData(static_cast<int>(MyScene::SelectObject));
	action->setIcon(QIcon(":/icons/select.png"));
	action->setCheckable(true);
	m_actions.push_back(action);

	action = new QAction("Draw line", this);
	action->setData(static_cast<int>(MyScene::DrawLine));
	action->setIcon(QIcon(":/icons/line.png"));
	action->setCheckable(true);
	m_actions.push_back(action);

	action = new QAction("Draw rectangle", this);
	action->setData(static_cast<int>(MyScene::DrawRect));
	action->setIcon(QIcon(":/icons/rect.png"));
	action->setCheckable(true);
	m_actions.push_back(action);

	action = new QAction("Draw ellipse", this);
	action->setData(static_cast<int>(MyScene::DrawEllipse));
	action->setIcon(QIcon(":/icons/ellipse.png"));
	action->setCheckable(true);
	m_actions.push_back(action);

	action = new QAction("Draw polygon", this);
	action->setData(static_cast<int>(MyScene::DrawPolygon));
	action->setIcon(QIcon(":/icons/polygon.png"));
	action->setCheckable(true);
	m_actions.push_back(action);

	actionGroup = new QActionGroup(this);
	actionGroup->setExclusive(true);
	for(auto action : m_actions)
		actionGroup->addAction(action);
}

void MainWindow::createConnections()
{
	connect(actionGroup,
			&QActionGroup::triggered,
			this,
			[this](QAction *action)
			{
				m_scene->setMode(static_cast<MyScene::Mode>(action->data().toInt()));
			}
	);
}

void MainWindow::createToolBar()
{
	drawingToolBar = new QToolBar(this);
	this->addToolBar(Qt::TopToolBarArea, drawingToolBar);
	for(auto action : m_actions)
		drawingToolBar->addAction(action);
}
