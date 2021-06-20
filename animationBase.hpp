#ifndef ANIMATIONBASE_HPP
#define ANIMATIONBASE_HPP
#include "effectBase.hpp"

class Animation
{
private:
    Effect *effects[3];
    int nEffects = 0;
    enum State
    {
        Running = 0,
        Paused = 1
    } state = Running;

public:
    virtual void DrawImpl(unsigned long time) = 0;

    void Draw(unsigned long time)
    {
        if (state == Paused)
        {
            for (int i = 0; i < nEffects; i++)
                effects[i]->lastTime = time;
            return;
        }
        for (int i = 0; i < nEffects; i++)
            effects[i]->Update(time);
        DrawImpl(time);
    }

    void AddEffect(Effect *effect)
    {
        effects[nEffects] = effect;
        nEffects++;
    }

    void Pause() {
        state = Paused;
    }
    void Resume() {
        state = Running;
    }
    void ColorOverride(CRGB colorRgb, CHSV colorHsv)
    {
        for (int i = 0; i < nEffects; i++) {
            effects[i]->forceColor = true;
            effects[i]->forceColorRgb = colorRgb;
            effects[i]->forceColorHsv = colorHsv;
        }
    }
    void DisableColorOverride()
    {
        for (int i = 0; i < nEffects; i++) {
            effects[i]->forceColor = false;
        }
    }
    void SetSpeed(float speed)
    {
        for (int i = 0; i < nEffects; i++)
            effects[i]->speed = speed;
    }
};
#endif