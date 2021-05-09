#ifndef RANDOMWAYPOINT_H
#define RANDOMWAYPOINT_H

#include "mobilitymodel.h"
#include <vector>

class RandomWaypoint : public MobilityModel {
public:
	RandomWaypoint(int n, const QRectF& area);
	void move(Person &p) override;
protected:
	struct MobData {
		MobData() : x{0.0}, y{0.0}, s{0} {}		// s = 0 will force calling choose_new_target()
		MobData(qreal xd, qreal yd, int sd) : x{xd}, y{yd}, s{sd} {}
		qreal x, y;		// target point
		int s;			// steps/speed
	};
	std::vector<MobData> m_mob;			// person id = index
	qreal m_maxdist;					// maximum distance in the area
	void choose_new_target(MobData& md, qreal maxdistance = 0.0);
};

#endif // RANDOMWAYPOINT_H
