#ifndef PERSON_H
#define PERSON_H

#include <QPoint>
#include <QString>
#include <stdexcept>
#include <QPixmap>
#include <QGraphicsPixmapItem>

// states
enum class HealthState { HEALTHY, INFECTED, DIAGNOSED, HEALED, DECEASED };
QString to_string(HealthState s);

qreal distance(const QPointF& a, const QPointF& b);
constexpr int default_rest = 100;


class Person : public QGraphicsPixmapItem {
private:
	HealthState state_;
	uint32_t id_;
	uint32_t dlifetime_, diagnose_time_;
	uint32_t age_;
	short rest_;					// rest steps
	void init(HealthState s);
	void load_pixmap();
private:
	static uint32_t s_uid;
	static QPixmap* s_healthypm;
	static QPixmap* s_infectedpm;
	static QPixmap* s_diagnosedpm;
	static QPixmap* s_healedpm;
	static QPixmap* s_deadpm;
public:
	Person(QGraphicsItem *parent = nullptr);
	Person(HealthState st, QGraphicsItem *parent = nullptr);
	uint32_t id() const { return id_; }
	HealthState state() const { return state_; }
	void state(HealthState s);			// force state change (no checking)
	void change_state(HealthState s);	// only valid state changes
	void age();					// count if infected
	void randomize_state();
	void randomize_position(const QRectF& r);
	void randomize_position(qreal ox, qreal oy, qreal w, qreal h);	// origin-x, y, width, height
	qreal distance(const Person& p) const { return ::distance(pos(),p.pos()); }
	bool rest();		// keep resting?
public:
	bool change_direction;		// if true, change direction immediately
 };

#endif // PERSON_H
