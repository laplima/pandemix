#include "person.h"
#include <QRandomGenerator>
#include <QtMath>
#include <iostream>
#include "parameters.h"

using namespace std;

qreal distance(const QPointF& a, const QPointF& b)
{
	qreal dx = a.x() - b.x();
	qreal dy = a.y() - b.y();
	return qSqrt(dx*dx + dy*dy);
}

uint32_t healing_time()
{
	return QRandomGenerator::global()->bounded(MIN_HEALING_TIME, MAX_HEALING_TIME) * ONE_DAY;
}

uint32_t diagnostic_time()
{
	return QRandomGenerator::global()->bounded(MIN_DIAGNOSTIC_TIME, MAX_DIAGNOSTIC_TIME) * ONE_DAY;
}

QString to_string(HealthState s)
{
	switch (s) {
	case HealthState::HEALTHY:
		return QString{"HEALHTY"};
	case HealthState::INFECTED:
		return QString{"INFECTED"};
	case HealthState::DIAGNOSED:
		return QString{"DIAGNOSED"};
	case HealthState::HEALED:
		return QString{"HEALED"};
	case HealthState::DECEASED:
		return QString{"DEAD"};
	}
}

// -----------------------------------------------------

QPixmap* Person::s_healthypm = nullptr;
QPixmap* Person::s_infectedpm = nullptr;
QPixmap* Person::s_diagnosedpm = nullptr;
QPixmap* Person::s_healedpm = nullptr;
QPixmap* Person::s_deadpm = nullptr;
uint32_t Person::s_uid = 0;

// -----------------------------------------------------

Person::Person(QGraphicsItem *parent)
	: QGraphicsPixmapItem(parent)
{
	init(HealthState::HEALTHY);
}

Person::Person(HealthState st, QGraphicsItem *parent)
	: QGraphicsPixmapItem(parent)
{
	init(st);
}

void Person::init(HealthState s)
{
	rest_ = 0;		// do not start resting
	state_ = s;
	id_ = s_uid++;
	dlifetime_ = healing_time();
	diagnose_time_ = diagnostic_time();
	age_ = 0;
	change_direction = false;

	load_pixmap();
	setCacheMode(DeviceCoordinateCache);
}

void Person::load_pixmap()
{
	switch (state_) {
	case HealthState::HEALTHY:
		if (s_healthypm == nullptr)
			s_healthypm = new QPixmap{":/images/person-healthy.png"};
		setPixmap(*s_healthypm);
		break;
	case HealthState::INFECTED:
		if (s_infectedpm == nullptr)
			s_infectedpm = new QPixmap{":/images/person-infected.png"};
		setPixmap(*s_infectedpm);
		break;
	case HealthState::DIAGNOSED:
		if (s_diagnosedpm == nullptr)
			s_diagnosedpm = new QPixmap{":/images/person-infected-diag.png"};
		setPixmap(*s_diagnosedpm);
		break;
	case HealthState::HEALED:
		if (s_healedpm == nullptr)
			s_healedpm = new QPixmap{":/images/person-healed.png"};
		setPixmap(*s_healedpm);
		break;
	case HealthState::DECEASED:
		if (s_deadpm == nullptr)
			s_deadpm = new QPixmap{":/images/person-dead.png"};
		setPixmap(*s_deadpm);
		break;
	}
	setOffset(-pixmap().width()/2, -pixmap().height()/2);
}

void Person::randomize_state()
{
	state_ = static_cast<HealthState>(QRandomGenerator::global()->bounded(5));
	if (state_ == HealthState::DECEASED)
		state_ = HealthState::HEALTHY;
	load_pixmap();
}

void Person::state(HealthState s)
{
	if (s != state_) {
		state_ = s;
		load_pixmap();
	}
}

void Person::change_state(HealthState s)
{
	HealthState olds = state_;

	if (s != state_) {
		switch (state_) {
		case HealthState::HEALTHY:
			if (s == HealthState::INFECTED)
				state_ = s;
			else {
				// invalid
			}
			break;
		case HealthState::INFECTED:
			switch (s) {
			case HealthState::DIAGNOSED:
			case HealthState::DECEASED:
			case HealthState::HEALED:
				state_ = s;
				break;
			default:
				// invalid
				break;
			}
			break;
		case HealthState::DIAGNOSED:
			switch (s) {
			case HealthState::HEALED:
			case HealthState::DECEASED:
				state_ = s;
				break;
			default:
				// invalid
				break;
			}
			break;
		case HealthState::HEALED:
		case HealthState::DECEASED:
			break;
		}

		if (olds != state_)
			load_pixmap();
	}
}

void Person::randomize_position(const QRectF& r)
{
	randomize_position(r.x(),r.y(),r.width(),r.height());
}

void Person::randomize_position(qreal ox, qreal oy, qreal w, qreal h)
{
	qreal newx = QRandomGenerator::global()->bounded(int(ox), int(w + ox));
	qreal newy = QRandomGenerator::global()->bounded(int(oy), int(h + oy));
	this->setX(newx);
	this->setY(newy);
}

void Person::age()
{
	// increment time only if infected
	if (state_ != HealthState::INFECTED && state_ != HealthState::DIAGNOSED)
		return;

	++age_;

	const qreal R = QRandomGenerator::global()->bounded(100.0);

	if (state_ == HealthState::INFECTED) {
		if (age_ > diagnose_time_) {
			if (age_ < dlifetime_) {
				// can only be diagnosed
				if (R < DIAGNOSED_RATE) {
					change_state(HealthState::DIAGNOSED);
					change_direction = true;
				}
			} else {
				// can be diganosed, get healed or decease
				if (R < DEATH_RATE)
					change_state(HealthState::DECEASED);
				else {
					if (R < DIAGNOSED_RATE) {
						change_state(HealthState::DIAGNOSED);
						change_direction = true;
					} else
						change_state(HealthState::HEALED);
				}
			}
		}
	} else {
		// DIAGNOSED
		if (age_ > dlifetime_) {
			// decease or get healed
			if (R < DEATH_RATE)
				change_state(HealthState::DECEASED);
			else
				change_state(HealthState::HEALED);
		}
	}
}

bool Person::rest()
{
	--rest_;
	bool go = (rest_ <= 0);
	if (go)
		rest_ = QRandomGenerator::global()->bounded(default_rest);
	return go;
}
