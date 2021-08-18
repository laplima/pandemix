#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsRectItem>
#include <vector>
#include "person.h"
#include "mobilitymodel.h"

//#include "renderarea.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow {
	Q_OBJECT
public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();
private slots:					// do not use "automatic" connections like on_...
	void start_animation();
	void animate();
	void stop_animation();
protected:
	QGraphicsRectItem* create_bar(HealthState hs);
	void update_bar(HealthState hs, int people);
protected:
	Ui::MainWindow* ui;
	QGraphicsScene* m_scene;
	std::vector<Person*> m_people;
	QTimer* m_timer;
	std::vector<QPointF> m_msteps;
	MobilityModel* m_mobility;
	QGraphicsSimpleTextItem* m_time_label;
	long m_timelapse;						// time lapse
	QGraphicsRectItem* m_ginfected;
	QGraphicsRectItem* m_gdiagnosed;
	QGraphicsRectItem* m_gdead;
	QGraphicsRectItem* m_ghealed;
	QGraphicsRectItem* m_ghealthy;
};

#endif // MAINWINDOW_H
