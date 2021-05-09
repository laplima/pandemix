#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <QtGlobal>

inline extern constexpr int POPULATION = 100;
inline extern constexpr qreal CONTAMINATION_DISTANCE = 3.0;
inline extern constexpr qreal ISOLATION_DISTANCE = 10.0;		// maximum distance when diagnosed

// PERCENTAGES
inline extern constexpr qreal INITIALLY_INFECTED = 1.0;			// percentage of initially infected people
inline extern constexpr qreal DEATH_RATE = 6.0;					// death rate
inline extern constexpr qreal DIAGNOSED_RATE = 50.0;			// estimate percentage of diagnosed people

// DURATIONS (in days)
inline extern constexpr uint32_t ONE_DAY = 100;					// ONE DAY = n STEPS

inline extern constexpr uint32_t MIN_DIAGNOSTIC_TIME = 2;		// in days
inline extern constexpr uint32_t MAX_DIAGNOSTIC_TIME = 14;
inline extern constexpr uint32_t MIN_HEALING_TIME = 14;
inline extern constexpr uint32_t MAX_HEALING_TIME = 28;

#endif // PARAMETERS_H
