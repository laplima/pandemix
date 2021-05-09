#ifndef MOBILITYMODEL_H
#define MOBILITYMODEL_H

#include "person.h"

class MobilityModel {
public:
	MobilityModel(const QRectF& area) : _area{area} {}
	virtual ~MobilityModel() {}
	virtual void move(Person& p) = 0;
protected:
	QRectF _area;
};

#endif // MOBILITYMODEL_H
