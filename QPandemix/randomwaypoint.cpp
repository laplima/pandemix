#include "randomwaypoint.h"
#include <QRandomGenerator>
#include <QtMath>
#include "parameters.h"
#include <iostream>

using namespace std;

constexpr qreal MARGIN = 3.0;

RandomWaypoint::RandomWaypoint(int n, const QRectF& area) : MobilityModel{area}
{
	m_mob.resize(n);		// create all with s == 0
	m_maxdist = qSqrt(_area.height()*_area.height() + _area.width()*_area.width());
}

void RandomWaypoint::choose_new_target(MobData& md, qreal maxdistance)
{
	if (maxdistance <= 0.0) {
		int lowest = _area.x() + MARGIN;
		int highest = _area.x() + _area.width();
		md.x = QRandomGenerator::global()->bounded(lowest, highest) - MARGIN;
		lowest = _area.y()+MARGIN;
		highest = _area.y() + _area.height();
		md.y = QRandomGenerator::global()->bounded(lowest, highest) - MARGIN;
		lowest = m_maxdist/8;
		highest = m_maxdist/4;
		md.s = QRandomGenerator::global()->bounded(lowest, highest);
	} else {
		md.x += QRandomGenerator::global()->bounded((int)(-ISOLATION_DISTANCE/2), (int)(ISOLATION_DISTANCE/2));
		md.y += QRandomGenerator::global()->bounded((int)(-ISOLATION_DISTANCE/2), (int)(ISOLATION_DISTANCE/2));
		if (md.x < _area.x()+MARGIN) md.x = _area.x()+MARGIN;
		if (md.x > _area.x()+_area.width()-MARGIN) md.x = _area.x() + _area.width()-MARGIN;
		if (md.y < _area.y()+MARGIN) md.y = _area.y()+MARGIN;
		if (md.y > _area.y()+_area.height()-MARGIN) md.y = _area.y()+_area.height()-MARGIN;
		md.s = ISOLATION_DISTANCE;
	}
}

void RandomWaypoint::move(Person &person)
{
	MobData& t = m_mob[person.id()];
	QPointF p = person.pos();

	if (person.change_direction) {
		person.change_direction = false;
		if (person.state() == HealthState::HEALTHY) {
			t.x = _area.width() - t.x;
			t.y = _area.height() - t.y;
			int lowest = m_maxdist/8;
			int highest = m_maxdist/4;
			t.s = QRandomGenerator::global()->bounded(lowest, highest);
		} else {
			t.x = person.x();
			t.y = person.y();
			t.s = 1;
		}
	}

	if (--t.s > 0) {
		qreal delta = distance(QPointF{t.x,t.y}, p);
		qreal theta = qAtan2(t.y-p.y(), t.x-p.x());
		qreal m = delta / t.s;
		qreal dx = m * qCos(theta);
		qreal dy = m * qSin(theta);
		person.moveBy(dx,dy);
	} else {
		if (person.state() == HealthState::DIAGNOSED) {
			if (!person.resting())
				choose_new_target(t,ISOLATION_DISTANCE);    // isolation
		} else {
			if (!person.resting())
				choose_new_target(t);
		}
	}
}
