


#include <WS2812B.h>
#include <FastLED.h>
#include "animations.hpp"
#include "dataDict.hpp"

#define NLEDS 562
#define NUM_ANIMATIONS 10

WS2812B strip = WS2812B(NLEDS);
CRGB leds[NLEDS];
LedStrip ledsAll(&leds[0], NLEDS);
DataDict stateMutator;

// void sendCommand(const char *cmd)
// {
// 	Serial1.print(cmd);
// 	delay(500);
// 	while (Serial1.available())
// 	{
// 		Serial.write(Serial1.read());
// 	}
// }

struct AppState {
	CRGB color;
	int animation;
	int speed;
	int off;
	int pause;
	int colorOverride;
	int rotationOverride;
} state;

void ResetAppState()
{
	stateMutator.AddEntry('C', sizeof(state.color), &state.color);
	stateMutator.AddEntry('a', sizeof(state.animation), &state.animation);
	stateMutator.AddEntry('s', sizeof(state.speed), &state.speed);
	stateMutator.AddEntry('c', sizeof(state.colorOverride), &state.colorOverride);
	stateMutator.AddEntry('x', sizeof(state.rotationOverride), &state.rotationOverride);
	stateMutator.AddEntry('o', sizeof(state.off), &state.off);
	stateMutator.AddEntry('p', sizeof(state.pause), &state.pause);

	state.color = CRGB(CHSV(93, 255, 20));
	state.animation = -1;
	state.speed = 128;
	state.rotationOverride = 0;
	state.colorOverride = 0;
	state.off = 0;
	state.pause = 0;
	return;
}

void executeCommand(char *cmd)
{
	void *data;
	Serial.println(cmd);
	if (cmd[0] == 'r') {
		ResetAppState();
	}
	else if (cmd[0] == 'C') {
		long hexColor = strtol(&cmd[1], NULL, 16);
		CRGB color = CRGB(hexColor & 0xFF, hexColor & 0xFF00 >> 8, hexColor & 0xFF0000 >> 16);
		stateMutator.SetEntry(cmd[0], &color);
	}
	else {
		int value = strtol(&cmd[1], NULL, 10);
		stateMutator.SetEntry(cmd[0], &value);
	}
}

void parseCommand()
{
	static char cmd[16];
	static int i = 0;
	bool cmdValid = false;
	while (Serial2.available())
	{
		cmd[i] = Serial2.read();
		if (cmd[i] == '@')
		{
			cmd[i] = 0;
			i = 0;
			executeCommand(cmd);
			return;
		}
		i++;
	}
}

enum AnimationMode
{
	MODE_NORMAL = 0,
	MODE_SKIP,
	MODE_EXTEND
};

struct AnimationList
{
	int count;
	int current;
	Animation *animation[NUM_ANIMATIONS];
	AnimationMode mode[NUM_ANIMATIONS];
} aniList;

// This is intentional. Fixes a compilation error.
void registerAnimation(Animation *ani, enum AnimationMode mode);
void registerAnimation(Animation *ani, enum AnimationMode mode)
{
	aniList.animation[aniList.count] = ani;
	aniList.mode[aniList.count] = mode;
	aniList.count++;
}

unsigned long curTime;
unsigned long lastSwapTime;
AniWipe aniWipe(&ledsAll);
AniFlash aniFlash(&ledsAll);
AniParticle aniParticle(&ledsAll);
AniMultiParticle aniMultiParticle(&ledsAll);
AniZoom aniZoom(&ledsAll);
AniRainbow aniRainbow(&ledsAll);
AniSparkle aniSparkle(&ledsAll);
AniConfetti aniConfetti(&ledsAll);
AniTheater aniTheater(&ledsAll);
AniBreathe aniBreathe(&ledsAll);

void setup()
{
	Serial.begin(9600);
	Serial2.begin(9600);
	strip.begin(); // Sets up the SPI
	strip.show();  // Clears the strip, as by default the strip data is set to all LED's off.

	aniList.count = 0;
	aniList.current = 0;
	for (int i = 0; i < NUM_ANIMATIONS; i++)
		aniList.animation[i] = NULL;
	//REMEMBER TO UPDATE NUM_ANIMATIONS
//  registerAnimation(&aniSparkle, MODE_EXTEND);
//  registerAnimation(&aniParticle, MODE_EXTEND);
  registerAnimation(&aniMultiParticle, MODE_EXTEND);
//  registerAnimation(&aniConfetti, MODE_EXTEND);
//  registerAnimation(&aniFlash, MODE_EXTEND);
//  registerAnimation(&aniRainbow, MODE_EXTEND);
//	registerAnimation(&aniWipe, MODE_EXTEND);
//	registerAnimation(&aniZoom, MODE_EXTEND);
//	registerAnimation(&aniRainbow, MODE_EXTEND);
//  registerAnimation(&aniTheater, MODE_EXTEND);
//  registerAnimation(&aniBreathe, MODE_EXTEND);

  
	curTime = millis();
	lastSwapTime = curTime;

	ResetAppState();
}

int getLoopAnimation()
{
	const int skip = 1;
	int swapTime = (aniList.mode[aniList.current] == MODE_EXTEND) ? 10000 : 5000;
	if (curTime - lastSwapTime > swapTime)
	{
		lastSwapTime = curTime;
		for (int i = 0; i < skip; i++)
		{
			do
			{
				aniList.current = (aniList.current + 1) % aniList.count;
			} while (aniList.mode[aniList.current] == MODE_SKIP);
		}
	}
	return aniList.current;
}

void runAnimation(Animation *a)
{
	if (state.colorOverride)
		a->ColorOverride(state.color);

	if (state.pause)
		a->Pause();
	else
		a->Resume();

	a->SetSpeed((float)state.speed/128.0f);
	a->Draw(curTime);
}

void loop()
{
	int minDelay = 20;
	curTime = millis();
	parseCommand();

	int a = (state.animation == -1) ? getLoopAnimation() : state.animation;
	if (a < NUM_ANIMATIONS && aniList.animation[a])
		runAnimation(aniList.animation[a]);

	for (int i = 0; i < NLEDS; i++) {
		if (!state.off)
			strip.setPixelColor(i, strip.Color(leds[i].r, leds[i].g, leds[i].b));
		else
			strip.setPixelColor(i, strip.Color(0, 0, 0));
	}
	strip.show();

	unsigned long duration = millis() - curTime;
	if (duration < minDelay)
		delay(minDelay - duration);
}
