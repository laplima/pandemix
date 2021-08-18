#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "parameters.h"
#include <QGraphicsView>
#include <QThread>
#include <QRandomGenerator>
#include <iostream>
#include "randomdirection.h"
#include "randomwaypoint.h"

using namespace std;

class View : public QGraphicsView {
public:
	View(QGraphicsScene *scene) : QGraphicsView(scene) {}
protected:
//	void resizeEvent(QResizeEvent *event) override
//	{
//		QGraphicsView::resizeEvent(event);
//		fitInView(sceneRect(), Qt::KeepAspectRatio);
//	}
};


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow), m_mobility{nullptr}, m_timelapse{0}
{
	ui->setupUi(this);

	m_scene = new QGraphicsScene{0, 0, 640, 480};
	QRectF sc = m_scene->sceneRect();

	for (int i=0; i<POPULATION; ++i) {
		m_people.emplace_back(new Person{HealthState::HEALTHY});
		m_people[i]->setZValue(i);
		if (i<INITIALLY_INFECTED*POPULATION/100)
			m_people[i]->state(HealthState::INFECTED);
//		m_people[i]->randomize_state();
		m_people[i]->randomize_position(sc);
		m_scene->addItem(m_people[i]);
	}

	m_scene->addRect(sc,QPen{QColor{0x66,0x66,0x66}});

	// time lapse

	m_time_label = new QGraphicsSimpleTextItem(tr("0"));
	m_time_label->setX(sc.x()+10);
	m_time_label->setY(sc.y()+sc.height()-20);
	m_time_label->setZValue(0);
	m_time_label->setBrush(QBrush{QColor{0x66,0x66,0x66}});
	m_time_label->setFont(QFont{"Bebas Neue"});
	m_scene->addItem(m_time_label);

	// graphic bars

	m_ghealthy = create_bar(HealthState::HEALTHY);
	update_bar(HealthState::HEALTHY, POPULATION);
	m_scene->addItem(m_ghealthy);

	m_ginfected = create_bar(HealthState::INFECTED);
	update_bar(HealthState::INFECTED, 0);
	m_scene->addItem(m_ginfected);

	m_gdiagnosed = create_bar(HealthState::DIAGNOSED);
	update_bar(HealthState::DIAGNOSED, 0);
	m_scene->addItem(m_gdiagnosed);

	m_ghealed = create_bar(HealthState::HEALED);
	update_bar(HealthState::HEALED, 0);
	m_scene->addItem(m_ghealed);

	m_gdead = create_bar(HealthState::DECEASED);
	update_bar(HealthState::DECEASED, 0);
	m_scene->addItem(m_gdead);

	// view

	View* view = new View{m_scene};
	setCentralWidget(view);

	QSize wsz{view->width()+2, view->height()+2};		// TODO: these constants are not portable...

	setMinimumSize(wsz);
	resize(wsz);

	m_timer = new QTimer(this);
	connect(m_timer, &QTimer::timeout, this, QOverload<>::of(&MainWindow::animate));
	connect(ui->actionStart, &QAction::triggered, this, &MainWindow::start_animation);
	connect(ui->actionStop, &QAction::triggered, this, &MainWindow::stop_animation);

	// m_mobility = new RandomDirection{100, m_scene->sceneRect()};
	m_mobility = new RandomWaypoint{POPULATION, m_scene->sceneRect()};
}

MainWindow::~MainWindow()
{
	delete ui;
	for (Person* p : m_people)
		delete p;
	if (m_mobility != nullptr)
		delete m_mobility;
}

QGraphicsRectItem* MainWindow::create_bar(HealthState hs)
{
	QColor c;
	QGraphicsRectItem* gi = new QGraphicsRectItem;

	switch (hs) {
	case HealthState::HEALTHY:
		c.setRgb(32, 192, 241);
		break;
	case HealthState::INFECTED:
		c.setRgb(234, 29, 93);
		break;
	case HealthState::DIAGNOSED:
		c.setRgb(0xFF, 0xB1, 0x35);
		break;
	case HealthState::HEALED:
		c.setRgb(0x8E,0xC4,0x49);
		break;
	case HealthState::DECEASED:
		c.setRgb(0x66,0x66,0x66);
	}

	QPen pen{c};
	gi->setBrush(QBrush{c});
	gi->setPen(pen);

	return gi;
}

void MainWindow::update_bar(HealthState hs, int people)
{
	constexpr int MAX = 100;				// maximum bar height

	const int XBASE = m_scene->sceneRect().width() - 60;
	const int YBASE = m_scene->sceneRect().height() - 10;
	const int h = MAX * people / POPULATION;

	int offset = 0;
	QGraphicsRectItem* gi = nullptr;

	switch (hs) {
	case HealthState::HEALTHY:
		offset = 0;
		gi = m_ghealthy;
		break;
	case HealthState::INFECTED:
		offset = 10;
		gi = m_ginfected;
		break;
	case HealthState::DIAGNOSED:
		offset = 20;
		gi = m_gdiagnosed;
		break;
	case HealthState::HEALED:
		offset = 30;
		gi = m_ghealed;
		break;
	case HealthState::DECEASED:
		offset = 40;
		gi = m_gdead;
	}

	gi->setRect(XBASE + offset, YBASE - h, 9, h);
}


void MainWindow::animate()
{
	++m_timelapse;
	QStringList sl = QString::number(m_timelapse/static_cast<float>(ONE_DAY)).split(".");
	QString right = (sl.length() > 1) ? sl[1] : "0";
	m_time_label->setText(sl[0]+"."+right.chopped(right.size()-1));

	int infected = 0;
	int diagnosed = 0;
	int healthy = 0;
	int healed = 0;
	int deceased = 0;

	for (Person* p : m_people) {
		if (p->state() == HealthState::DECEASED)
			++deceased;
		else {
			m_mobility->move(*p);
			p->age();
			switch (p->state()) {
			case HealthState::INFECTED: ++infected; break;
			case HealthState::DIAGNOSED: ++diagnosed; break;
			case HealthState::HEALED: ++healed; break;
			case HealthState::HEALTHY: ++healthy; break;
			case HealthState::DECEASED: ++deceased; break;		// since state may change
			}
		}
	}

	update_bar(HealthState::HEALTHY, healthy);
	update_bar(HealthState::INFECTED, infected);
	update_bar(HealthState::DIAGNOSED, diagnosed);
	update_bar(HealthState::HEALED, healed);
	update_bar(HealthState::DECEASED, deceased);

	if (infected == 0 && diagnosed == 0) {
		m_timer->stop();
		// report

	} else {
		// check infection
		for (Person* p : m_people) {

			switch (p->state()) {
			case HealthState::INFECTED:
				for (Person* q : m_people) {
					if (p != q) {
						if (p->distance(*q) < CONTAMINATION_DISTANCE && q->state() == HealthState::HEALTHY)
							q->change_state(HealthState::INFECTED);
					}
				}
				break;
			case HealthState::DIAGNOSED:
				// repel healthy people
				for (Person* q : m_people)
					if (p != q)
						if (p->distance(*q) < CONTAMINATION_DISTANCE*2 && q->state() == HealthState::HEALTHY)
							q->change_direction = true;		// deflect healthy
				break;
			default:
				break;
			}
		}
	}
}

void MainWindow::start_animation()
{
	m_timelapse = 0;
	m_timer->start(33);		// ~30fps
}


void MainWindow::stop_animation()
{
	m_timer->stop();
}
