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
	wipe.colorHsv = CHSV(hue, 255, 100);
	wipe.invert = 0;
	wipe.Draw(leds);
	unsigned char originalHue = wipe.forceColorHsv.h;
	wipe.forceColorHsv.h += 111;
	wipe.colorHsv = CHSV(hue + 111, 255, 100);
	wipe.invert = 1;
	wipe.Draw(leds);
	wipe.forceColorHsv.h = originalHue;
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
		particle.colorHsv = CHSV(128 * i + mapFloat(color.Progress(), 0, color.duration, 0, 255), 255, 128);
		particle.progessOffset = (particle.duration / particleCount) * i;
		particle.Draw(leds);
	}
}

void AniConfetti::DrawImpl(unsigned long time)
{
	unsigned char hueOffset;
	if (!hue.forceColor)
		hueOffset = mapFloat(hue.Progress(), 0, hue.duration, 0, 255);
	else
		hueOffset = hue.forceColorHsv.h;
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

	particle.colorHsv = CHSV(hueOffset, 255, 128);
	particle.hueLength = 64;
	particle.reverse = false;
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
		particle.colorHsv = CHSV(0, 255, 128);
		particle.hueLength = 255;
		particle.reverse = true;
		particle.streakSize = leds->Size() / 2;
		particle.progessOffset = 0;
		particle.Draw(leds);
		particle.progessOffset += particle.duration/2;
		particle.Draw(leds);
	}
}

void AniTheater::DrawImpl(unsigned long time)
{
  leds->Clear();
  theater.colorRgb = CRGB(20, 20, 20);
  theater.Draw(leds);
}

void AniBreathe::DrawImpl(unsigned long time)
{
  leds->Clear();
  const int segmentSize = 29;
  int progress = breathe.Progress();
  if ((breathe.speed > 0 && progress < lastProgress) || (breathe.speed < 0 && progress > lastProgress))
  {
      breathe.colorHsv.h += 144;
      extraOffset = (!extraOffset) ? segmentSize / 2 : 0;
  }
  lastProgress = progress;
  breathe.colorHsv.s = 255;
  breathe.colorHsv.v = 150;
  int lastHue = breathe.colorHsv.h;
  for (int i = 0; i < 20; i++)
  {
      breathe.colorHsv.h += 0;
      breathe.size = segmentSize;
      breathe.offset = breathe.size * i + extraOffset;
      breathe.Draw(leds);   
  }
  breathe.colorHsv.h = lastHue;
}

void AniMultiParticle::DrawImpl(unsigned long time)
{
  leds->Clear();
  const int particleCount = 6;
  particle.gradient = false;
  for (size_t i = 0; i < particleCount; i++)
  {
    int hueOffset = mapFloat(color.Progress(), 0, color.duration, 0, 255);
    particle.colorHsv = CHSV(HUE_RED + hueOffset, 255, 128);
    particle.streakSize = leds->Size() / (particleCount*2);
    particle.progessOffset = (particle.duration / particleCount) * i;
    
    leds->SetBlendMode(LedStrip::BLEND_MODE_ADD);
    particle.reverse = false;
    particle.Draw(leds);
	particle.colorHsv.h = HUE_GREEN + hueOffset;

	unsigned char originalHue = particle.forceColorHsv.h;
	particle.forceColorHsv.h += HUE_GREEN;
    particle.reverse = true;
    particle.Draw(leds);
	particle.forceColorHsv.h = originalHue;
    leds->SetBlendMode(LedStrip::BLEND_MODE_NONE);
    
  }
}

void AniConfetti2::DrawImpl(unsigned long time)
{
	unsigned char hueOffset = mapFloat(hue.Progress(), 0, hue.duration, 0, leds->Size());
	fadeToBlackBy(leds->Leds(), leds->Size(), 10);
	leds->SetDir(true).SetOffset(0).SetWrap(true).SetViewport(0, leds->Size());
	int pos;

	for (int i=0; i<10; i++) {
		pos = random16(leds->Size());
		unsigned char hueStart = mapFloat(pos, 0, leds->Size(), 0, 255);
		leds->SetLED(pos, CHSV(hueOffset + hueStart + random8(64), 200, 255));	
	}
}