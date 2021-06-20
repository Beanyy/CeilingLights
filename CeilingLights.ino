


#include <WS2812B.h>
#include <FastLED.h>
#include "animations.hpp"
#include "dataDict.hpp"

#define NLEDS 562
#define NUM_ANIMATIONS 11

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
} state;

void ResetAppState()
{
	stateMutator.AddEntry('C', sizeof(state.color), &state.color);
	stateMutator.AddEntry('a', sizeof(state.animation), &state.animation);
	stateMutator.AddEntry('s', sizeof(state.speed), &state.speed);
	stateMutator.AddEntry('c', sizeof(state.colorOverride), &state.colorOverride);
	stateMutator.AddEntry('o', sizeof(state.off), &state.off);
	stateMutator.AddEntry('p', sizeof(state.pause), &state.pause);

	state.color = CRGB(CHSV(93, 255, 20));
	state.animation = -1;
	state.speed = 128;
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
		unsigned char blue = hexColor & 0xFF;
		unsigned char green = (hexColor & 0xFF00) >> 8;
		unsigned char red = (hexColor & 0xFF0000) >> 16;
		CRGB color = CRGB(red>>1, green>>1, blue>>1);
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
AniConfetti2 aniConfetti2(&ledsAll);

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
	registerAnimation(&aniSparkle, MODE_EXTEND);
	registerAnimation(&aniParticle, MODE_EXTEND);
	registerAnimation(&aniMultiParticle, MODE_EXTEND);
	registerAnimation(&aniConfetti, MODE_EXTEND);
	registerAnimation(&aniConfetti2, MODE_EXTEND);
	registerAnimation(&aniFlash, MODE_EXTEND);
	registerAnimation(&aniRainbow, MODE_EXTEND);
	registerAnimation(&aniWipe, MODE_EXTEND);
	registerAnimation(&aniZoom, MODE_EXTEND);
	registerAnimation(&aniTheater, MODE_EXTEND);
	registerAnimation(&aniBreathe, MODE_EXTEND);

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

CHSV RgbToHsv(CRGB rgb)
{
    CHSV hsv;
    unsigned char rgbMin, rgbMax;

    rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
    rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

    hsv.v = rgbMax;
    if (hsv.v == 0)
    {
        hsv.h = 0;
        hsv.s = 0;
        return hsv;
    }

    hsv.s = 255 * long(rgbMax - rgbMin) / hsv.v;
    if (hsv.s == 0)
    {
        hsv.h = 0;
        return hsv;
    }

    if (rgbMax == rgb.r)
        hsv.h = 0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
    else if (rgbMax == rgb.g)
        hsv.h = 85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
    else
        hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

    return hsv;
}

void runAnimation(Animation *a)
{
	if (state.colorOverride) {
		CHSV colorHsv = RgbToHsv(state.color);
		a->ColorOverride(state.color, colorHsv);
	} else {
		a->DisableColorOverride();
	}

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
