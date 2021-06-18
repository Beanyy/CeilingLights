#include <FastLED.h>
#include "animations.hpp"
#include "helpers.hpp"

void AniWipe::DrawImpl(unsigned long time)
{
	inner->Clear();
	outer->Clear();

	int progress = wipe.Progress();
	if ((wipe.speed > 0 && progress < lastProgress) || (wipe.speed < 0 && progress > lastProgress))
		hue += 144;

	lastProgress = progress;
	wipe.ledOffset = mapFloat(spin.Progress(), 0, spin.duration, 0, inner->Size());
	wipe.color = CHSV(hue, 255, 100);
	wipe.invert = 0;
	wipe.Draw(inner);
	wipe.Draw(outer);

	wipe.ledOffset = mapFloat(spin.Progress(), 0, spin.duration, 0, outer->Size());
	wipe.color = CHSV(hue + 111, 255, 100);
	wipe.invert = 1;
	wipe.Draw(inner);
	wipe.Draw(outer);
}

void AniFlash::DrawImpl(unsigned long time)
{
	flash.seed = 10;
	flash.Draw(leds);
}

void AniParticle::DrawImpl(unsigned long time)
{
	leds->Clear();
	particle.gradient = false;
	for (size_t i = 0; i < 4; i++)
	{
		particle.hue = 128 * i + mapFloat(color.Progress(), 0, color.duration, 0, 255);

		particle.reverse = true;
		particle.streakSize = 20;
		particle.progessOffset = (particle.duration / 4) * i;
		particle.Draw(leds);
	}
}

void AniConfetti::DrawImpl(unsigned long time)
{
	unsigned char hueOffset = mapFloat(hue.Progress(), 0, hue.duration, 0, 255);
	fadeToBlackBy(leds->Leds(), leds->Size(), 10);
	leds->SetDir(true).SetOffset(0).SetWrap(true).SetViewport(0, leds->Size());
	int pos;

	for (int i=0; i<8; i++) {
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
		particle.progessOffset = 0;
		particle.Draw(leds);
		particle.progessOffset += particle.duration/2;
		particle.Draw(leds);
	}
}
