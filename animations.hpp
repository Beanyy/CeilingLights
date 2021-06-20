#ifndef ANIMATIONS_HPP
#define ANIMATIONS_HPP
#include "animationBase.hpp"
#include "effects.hpp"

class AniWipe : public Animation
{
private:
    EffectWipe wipe;
    LedStrip *leds;
    unsigned char hue = HUE_GREEN;
    int lastProgress = 0;

public:
    AniWipe(LedStrip *leds) : leds(leds)
    {
        wipe.duration = 6000;
        this->AddEffect(&wipe);
    }
    virtual void DrawImpl(unsigned long time) override;
};

class AniFlash : public Animation
{
private:
    EffectFlash flash;
    LedStrip *leds;

public:
    AniFlash(LedStrip *leds) : leds(leds)
    {
        this->AddEffect(&flash);
    }
    virtual void DrawImpl(unsigned long time) override;
};

class AniParticle : public Animation
{
private:
    EffectParticle particle;
    Effect color;
    LedStrip *leds;

public:
    AniParticle(LedStrip *leds) : leds(leds)
    {
        color.duration = 30000;
        this->AddEffect(&color);
        particle.duration = 10000;
        particle.reverse = false;
        particle.streakSize = 49;
        this->AddEffect(&particle);
    }
    virtual void DrawImpl(unsigned long time) override;
};

class AniConfetti : public Animation
{
private:
    LedStrip *leds;
    Effect hue;

public:
    AniConfetti(LedStrip *leds) : leds(leds)
    {
        hue.duration = 15000;
        this->AddEffect(&hue);
    }
    virtual void DrawImpl(unsigned long time) override;
};

class AniZoom : public Animation
{
private:
    EffectParticle particle;
    Effect color;
    LedStrip *leds;

public:
    AniZoom(LedStrip *leds) : leds(leds)
    {
        color.duration = 15000;
        particle.duration = 5000;
        this->AddEffect(&color);
        this->AddEffect(&particle);
    }
    virtual void DrawImpl(unsigned long time) override;
};

class AniSparkle : public Animation
{
private:
    EffectRainbow rainbow;
    LedStrip *leds;

public:
    AniSparkle(LedStrip *leds) : leds(leds)
    {
        this->AddEffect(&rainbow);
    }
    virtual void DrawImpl(unsigned long time) override;
};

class AniRainbow : public Animation
{
private:
    EffectParticle particle;
    LedStrip *leds;

public:
    AniRainbow(LedStrip *leds) : leds(leds)
    {
        particle.duration = 1500;
        this->AddEffect(&particle);
    }
    virtual void DrawImpl(unsigned long time) override;
};


class AniTheater : public Animation
{
private:
    EffectTheater theater;
    LedStrip *leds;

public:
    AniTheater(LedStrip *leds) : leds(leds)
    {
        this->AddEffect(&theater);
    }
    virtual void DrawImpl(unsigned long time) override;
};


class AniBreathe : public Animation
{
private:
    EffectBreathe breathe;
    LedStrip *leds;
    int lastProgress = 0;
    int extraOffset = 0;
    
public:
    AniBreathe(LedStrip *leds) : leds(leds)
    {
        breathe.duration = 1000;
        breathe.lightPercent = 0.45;
        this->AddEffect(&breathe);
    }
    virtual void DrawImpl(unsigned long time) override;
};

class AniMultiParticle : public Animation
{
private:
    EffectParticle particle;
    Effect color;
    LedStrip *leds;
    
public:
    AniMultiParticle(LedStrip *leds) : leds(leds)
    {
        color.duration = 30000;
        this->AddEffect(&color);
        particle.duration = 10000;
        this->AddEffect(&particle);
    }
    virtual void DrawImpl(unsigned long time) override;
};

#endif
