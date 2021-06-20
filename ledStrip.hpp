#ifndef LEDSTRIP_HPP
#define LEDSTRIP_HPP

#ifdef SIMULATOR
#include <stdint.h>
#include <crgb.hpp>
#endif

class LedStrip
{
public:
  typedef enum BlendMode {
    BLEND_MODE_NONE = 0,
    BLEND_MODE_HALF,
    BLEND_MODE_ADD,
    BLEND_MODE_MAXIMUM
  } BlendMode;
  
  LedStrip(CRGB *leds_ = NULL, int size_ = 0);
  LedStrip &SetViewport(float pos, float size);
  LedStrip &SetWrap(bool w);
  LedStrip &SetOffset(int o);
  LedStrip &SetDir(bool f);
  LedStrip &SetBlendMode(BlendMode mode);
  LedStrip &SetLED(int pos, CRGB color);

  LedStrip &DrawColor(CRGB color);
  LedStrip &DrawGradient(uint16_t hueStart, uint16_t hueEnd, int size, int brightness = 255);
  LedStrip &DrawRandom(int num);
  LedStrip &DrawTriangle(int size, uint8_t hue);
  LedStrip &DrawStreak(int size, uint8_t hue, bool fowards, unsigned char brightness);
  LedStrip &Shift(int m);

  LedStrip &Reset();
  LedStrip &Clear();
  int Size() { return nLeds; }
  CRGB *Leds() { return leds; };

private:
  int CalcViewport(int pos);
  CRGB *leds;
  int nLeds;
  bool wrap;
  bool forward;
  int viewport[2];
  int offset;
  BlendMode blendMode = BLEND_MODE_NONE;
};

#endif
