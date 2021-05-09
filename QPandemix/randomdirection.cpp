#include "randomdirection.h"
#include <QRandomGenerator>

RandomDirection::RandomDirection(int n, const QRectF& area) : MobilityModel{area}, _n{n}
{
	for (int i=0; i<_n; ++i) {
		int r1 = (QRandomGenerator::global()->bounded(2) == 0) ? 1.0 : -1.0;
		int r2 = (QRandomGenerator::global()->bounded(2) == 0) ? 1.0 : -1.0;
		_direction.emplace_back(r1,r2);
	}
}

void RandomDirection::move(Person &p)
{
	const qreal INFX = _area.x() + p.boundingRect().width()/2;
	const qreal SUPX = 2 * _area.x() + _area.width() - INFX;
	const qreal INFY = _area.y() + p.boundingRect().height()/2;
	const qreal SUPY = 2 * _area.y() + _area.height() - INFY;

	p.setPos(p.pos() + _direction[p.id()]);

	if (p.x() < INFX) {
		p.setX(INFX);
		_direction[p.id()].setX(-_direction[p.id()].x());
	} else if (p.x() > SUPX) {
		p.setX(SUPX);
		_direction[p.id()].setX(-_direction[p.id()].x());
	}
	if (p.y()< INFY) {
		p.setY(INFY);
		_direction[p.id()].setY(-_direction[p.id()].y());
	} else if (p.y() > SUPY) {
		p.setY(SUPY);
		_direction[p.id()].setY(-_direction[p.id()].y());
	}
}
