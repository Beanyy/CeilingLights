#include "effects.hpp"
#include "helpers.hpp"

void EffectParticle::Draw(LedStrip *strip)
{
    int p = this->Progress();
    CHSV color = (this->forceColor) ? forceColorHsv : colorHsv;

    int offset = mapFloat(p, 0, this->duration, 0, strip->Size());
    strip->SetDir(reverse).SetOffset(offset).SetWrap(true).SetViewport(0, strip->Size());
    if (gradient)
        strip->DrawGradient(color, color.h + hueLength, streakSize);
    else
        strip->DrawStreak(color, streakSize, false);
}

void EffectRainbow::Draw(LedStrip *strip)
{
    strip->SetDir(true).SetOffset(0).SetWrap(false).SetViewport(0, strip->Size());
    int hueOffset = mapFloat(this->Progress(), 0, this->duration, 0, 255);
    if (!mode)
        strip->DrawColor(CHSV(hueOffset, 255, 100));
    else
        strip->DrawRandom(30);
}

void EffectTheater::Draw(LedStrip *strip)
{
    const int skip = 3;
    CRGB color = (this->forceColor) ? this->forceColorRgb : this->colorRgb;
    int idx = (this->Progress() / 50) % skip;
    strip->SetDir(true).SetOffset(0).SetWrap(false).SetViewport(0, strip->Size());
    for (int i = idx; i < strip->Size(); i += skip)
        strip->SetLED(i, color);
}

void EffectFlash::Draw(LedStrip *strip)
{
    const int onDuration = 50;
    const int nSegments = 20;
    const int nFlashes = 2;

    int stripSize = strip->Size()/nSegments;
    int segment = 0;
    int offset = mapFloat(this->Progress(), 0, this->duration, 0, this->duration / onDuration);
    CRGB *leds = strip->Leds();
    for (int i = 0; i < strip->Size(); i++) {
      leds[i].r = (leds[i].r >= 10) ? (leds[i].r - 10) : 0;
      leds[i].g = (leds[i].g >= 10) ? (leds[i].g - 10) : 0;
      leds[i].b = (leds[i].b >= 10) ? (leds[i].b - 10) : 0;      
    }

    strip->SetDir(true).SetWrap(false);
    srand(seed);
    for (int i = 0; i < nFlashes; i++) {
        for (int i = 0; i < offset; i++)
            segment = rand() % nSegments;
        
        int ledOffset = segment * stripSize;
        strip->SetOffset(ledOffset).SetViewport(ledOffset, stripSize);
        strip->DrawColor(CHSV(offset * 144 + 256/nFlashes  * i, 255, 255));
    }
}

void EffectWipe::Draw(LedStrip *strip)
{
    CHSV color = (this->forceColor) ? this->forceColorHsv : this->colorHsv;
    float offset = mapFloat(this->Progress(), 0, this->duration, 0, strip->Size());
    if (invert)
        strip->SetViewport(offset, strip->Size() - offset);
    else
        strip->SetViewport(0, offset);
    strip->SetDir(true).SetOffset(0).SetWrap(true).DrawColor(color);
}

void EffectBreathe::Draw(LedStrip *strip)
{
    const int progressSlope = (1.0 / size) * this->duration;
    CHSV color = (this->forceColor) ? this->forceColorHsv : this->colorHsv;
    int pixelProgress = 0;
    for (int i = 0; i <= size / 2; i++)
    {
        float actualProgress = (this->Progress()  - pixelProgress);// % this->duration;
        int brightness;

        if (actualProgress < 0 || actualProgress >= lightPercent * this->duration)
           brightness = 0;
        else
           brightness = mapFloat(actualProgress, 0, lightPercent * this->duration, color.v, 0);

        int pixel0 = i + size/2;
        int pixel1 = size - pixel0 - 1;
        strip->SetWrap(true);
        strip->SetLED(offset + pixel0,  CHSV(color.h, color.s, brightness));
        strip->SetLED(offset + pixel1, CHSV(color.h, color.s, brightness));
        pixelProgress += progressSlope;
    }
}
