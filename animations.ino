#include <FastLED.h>
#include "animations.hpp"
#include "helpers.hpp"

void AniWipe::DrawImpl(unsigned long time)
{
	leds->Clear();

	int progress = wipe.Progress();
	if ((wipe.speed > 0 && progress < lastProgress) || (wipe.speed < 0 && progress > lastProgress))
		hue += 144;

	lastProgress = progress;
	wipe.color = CHSV(hue, 255, 100);
	wipe.invert = 0;
	wipe.Draw(leds);
	wipe.color = CHSV(hue + 111, 255, 100);
	wipe.invert = 1;
	wipe.Draw(leds);
}

void AniFlash::DrawImpl(unsigned long time)
{
	flash.seed = 10;
	flash.Draw(leds);
}

void AniParticle::DrawImpl(unsigned long time)
{
	leds->Clear();
  const int particleCount = 12;
	particle.gradient = false;
	for (size_t i = 0; i < particleCount; i++)
	{
    particle.streakSize = leds->Size()/particleCount;
		particle.hue = 128 * i + mapFloat(color.Progress(), 0, color.duration, 0, 255);
		particle.progessOffset = (particle.duration / particleCount) * i;
		particle.Draw(leds);
	}
}

void AniConfetti::DrawImpl(unsigned long time)
{
	unsigned char hueOffset = mapFloat(hue.Progress(), 0, hue.duration, 0, 255);
	fadeToBlackBy(leds->Leds(), leds->Size(), 10);
	leds->SetDir(true).SetOffset(0).SetWrap(true).SetViewport(0, leds->Size());
	int pos;

	for (int i=0; i<10; i++) {
		pos = random16(leds->Size());
		leds->SetLED(pos, CHSV(hueOffset + random8(64), 200, 255));	
	}
}

void AniZoom::DrawImpl(unsigned long time)
{
	leds->Clear();
	unsigned char hueOffset = mapFloat(color.Progress(), 0, color.duration, 0, 255);
	particle.gradient = true;

	particle.hueBegin = hueOffset;
	particle.hue = hueOffset + 64;
	particle.reverse = false;
  particle.brightness = 128;
	particle.streakSize = leds->Size() / 2;
	particle.progessOffset = 0;
	particle.Draw(leds);
	particle.progessOffset = particle.duration / 2;
	particle.Draw(leds);
  
}

void AniSparkle::DrawImpl(unsigned long time)
{
	rainbow.mode = 1;
	rainbow.Draw(leds);
}

void AniRainbow::DrawImpl(unsigned long time)
{
	leds->Clear();
	particle.gradient = true;
	{
		particle.hueBegin = 0;
		particle.hue = 255;
		particle.reverse = true;
		particle.streakSize = leds->Size() / 2;
    particle.brightness = 128;
		particle.progessOffset = 0;
		particle.Draw(leds);
		particle.progessOffset += particle.duration/2;
		particle.Draw(leds);
	}
}

void AniTheater::DrawImpl(unsigned long time)
{
  leds->Clear();
  theater.color = CRGB(20, 20, 20);
  theater.Draw(leds);
}

void AniBreathe::DrawImpl(unsigned long time)
{
  leds->Clear();
  const int segmentSize = 29;
  int progress = breathe.Progress();
  if ((breathe.speed > 0 && progress < lastProgress) || (breathe.speed < 0 && progress > lastProgress))
  {
      breathe.hue += 5;
      extraOffset = (!extraOffset) ? segmentSize / 2 : 0;
  }
  lastProgress = progress;
  int lastHue = breathe.hue;
  for (int i = 0; i < 20; i++)
  {
      breathe.hue += 0;
      breathe.size = segmentSize;
      breathe.offset = breathe.size * i + extraOffset;
      breathe.Draw(leds);   
  }
  breathe.hue = lastHue;
}

void AniMultiParticle::DrawImpl(unsigned long time)
{
  leds->Clear();
  const int particleCount = 6;
  particle.gradient = false;
  for (size_t i = 0; i < particleCount; i++)
  {
    int hueOffset = mapFloat(color.Progress(), 0, color.duration, 0, 255);
    particle.hue = HUE_RED + hueOffset;
    particle.streakSize = leds->Size() / (particleCount*2);
    particle.progessOffset = (particle.duration / particleCount) * i;
    
    leds->SetBlendMode(LedStrip::BLEND_MODE_ADD);
    particle.reverse = false;
    particle.Draw(leds);
    particle.hue = HUE_GREEN + hueOffset;
    particle.reverse = true;
    particle.Draw(leds);
    leds->SetBlendMode(LedStrip::BLEND_MODE_NONE);
    
  }
}
