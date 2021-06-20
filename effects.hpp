#ifndef EFFECTS_HPP
#define EFFECTS_HPP
#include "effectBase.hpp"
#define BPM128 1875

class EffectStrip : public Effect
{
public:
	virtual void Draw(LedStrip* strip) = 0;
};

class EffectParticle : public EffectStrip
{
public:
	int streakSize;
	unsigned char hueLength;
	bool reverse;
	bool gradient;
	EffectParticle() {
		this->duration = BPM128;
		reverse = false;
	}
	virtual void Draw(LedStrip* strip) override;
};

class EffectTheater : public EffectStrip
{
public:
	EffectTheater() {
		this->duration = BPM128;
	}
	virtual void Draw(LedStrip* strip) override;
};

class EffectWipe : public EffectStrip
{
public:
	EffectWipe() {
		this->duration = BPM128;
	}
	char invert;
	int ledOffset;
	virtual void Draw(LedStrip* strip) override;
};

class EffectRainbow : public EffectStrip
{
public:
	EffectRainbow() {
		this->duration = BPM128;
	}
	int mode;
	virtual void Draw(LedStrip* strip) override;
};

class EffectFlash : public EffectStrip
{
public:
  EffectFlash() {
    this->duration = BPM128*4;
  }
  int seed;
  virtual void Draw(LedStrip* strip) override;
};

class EffectBreathe : public EffectStrip
{
public:
  EffectBreathe() {
    this->duration = BPM128;
  }
  int size;
  int offset;
  float lightPercent = 0.7;
  virtual void Draw(LedStrip* strip) override;
};
#endif
