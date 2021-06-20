#ifndef EFFECTBASE_HPP
#define EFFECTBASE_HPP

#include "ledStrip.hpp"

#ifdef SIMULATOR
#include <ctime>
unsigned long millis();
#endif

class Effect
{
private:
	int prog;

public:
	unsigned long lastTime;
	int duration; //Try to make this a number with many factors.
	int progessOffset;
	float speed;
	CRGB colorRgb;
	CHSV colorHsv;
	CRGB forceColorRgb;
	CHSV forceColorHsv;
	bool forceColor = false;

	Effect() : duration(0),
			   speed(1.0f)
	{
		Reset();
	}

	virtual void Update(unsigned long time);
	void Reset();
	int Progress();
};

#endif
