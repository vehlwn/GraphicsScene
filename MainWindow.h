#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QToolBar>
#include <QAction>

#include <vector>

#include "MyScene.h"

namespace Ui {
class MainWindow;
}

// Main window contains GraphicsScene and tool bar.
class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private:
	Ui::MainWindow *ui;

	QGraphicsView* m_view;
	MyScene* m_scene;

	void createActions();
	void createConnections();
	void createToolBar();

	std::vector<QAction*> m_actions;
	QActionGroup *actionGroup;
	QToolBar* drawingToolBar;

};

#endif // MAINWINDOW_H
