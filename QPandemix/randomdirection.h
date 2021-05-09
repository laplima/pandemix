#ifndef RANDOMDIRECTION_H
#define RANDOMDIRECTION_H

#include "mobilitymodel.h"
#include <vector>

class RandomDirection : public MobilityModel {
public:
	RandomDirection(int n, const QRectF& area);
	void move(Person &p) override;
private:
	int _n;
	std::vector<QPointF> _direction;
};

#endif // RANDOMDIRECTION_H
