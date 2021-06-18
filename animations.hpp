#ifndef ANIMATIONS_HPP
#define ANIMATIONS_HPP
#include "animationBase.hpp"
#include "effects.hpp"

class AniWipe : public Animation
{
private:
    EffectWipe wipe;
    Effect spin;
    LedStrip *inner;
    LedStrip *outer;
    unsigned char hue = HUE_GREEN;
    int lastProgress = 0;

public:
    AniWipe(LedStrip *inner, LedStrip *outer) : inner(inner),
                                                outer(outer)
    {
        spin.duration = 3000;
        this->AddEffect(&spin);
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
        color.duration = 15000;
        this->AddEffect(&color);
        particle.duration = 1500;
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
        particle.duration = 1000;
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



#endif
