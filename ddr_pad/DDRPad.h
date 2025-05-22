#include <FastLED.h>
#ifndef DDRPad_h
#define DDRPad_h

#define PINK CRGB(217, 85, 1)
#define BLUE CRGB(3, 252, 152)

enum Panel
{
    Right,
    Down,
    Left,
    Up
};

template <int NUM_LEDS_PER_PANEL>
class DDRPanel
{
public:
    DDRPanel() {}
    DDRPanel(CRGB *leds, Panel panel)
    {
        this->mLeds = leds;
        int skip = NUM_LEDS_PER_PANEL * panel;
        for (int i = 0; i < NUM_LEDS_PER_PANEL; i++)
        {
            mLoopRange[i] = i + skip;
        }

        this->mColor = (panel % 2) == 0 ? PINK : BLUE;
    }

    void loop(bool buttonPress)
    {
        unsigned long now = millis();

        if (this->mOn)
        {
            if (!buttonPress)
            {
                this->beginDelluminate();
            }
            else
            {
                return;
            }
        }
        else
        {
            if (buttonPress)
            {
                this->mOn = true;
                this->mDelluminating = false;
                this->mCurrentBrightness = 255;
                this->illunimate();
                return;
            }
            else
            {
                if (!this->mDelluminating)
                {
                    return;
                }
            }
        }

        if (this->mDelluminating)
        {
            if (now - this->mLastFadeTime >= this->mFadeIntervalMs)
            {
                this->mLastFadeTime = now;
                this->mCurrentBrightness -= this->mFadeStep;

                if (this->mCurrentBrightness < this->mMinBrightness)
                {
                    this->mDelluminating = false;
                    this->mCurrentBrightness = this->mMinBrightness;
                }

                this->delluminate();
            }
        }
    }

    void beginDelluminate(void)
    {
        this->mDelluminating = true;
        this->mOn = false;
    }

    void illunimate(void)
    {
        for (int i = 0; i < NUM_LEDS_PER_PANEL; i++)
        {
            this->mLeds[mLoopRange[i]] = this->mColor;
        }
        FastLED.show();
    }

    void delluminate(void)
    {
        CRGB dimColor = this->mColor;
        dimColor.nscale8(this->mCurrentBrightness);

        for (int i = 0; i < NUM_LEDS_PER_PANEL; i++)
        {
            this->mLeds[mLoopRange[i]] = dimColor;
        }
        FastLED.show();
    }

    void setPanelColor(CRGB color)
    {
        for (int i = 0; i < NUM_LEDS_PER_PANEL; i++)
        {
            this->mLeds[mLoopRange[i]] = color;
        }
        FastLED.show();
    }

    void turnOff(void)
    {
        CRGB dimColor = this->mColor;
        for (int i = 0; i < NUM_LEDS_PER_PANEL; i++)
        {
            this->mLeds[mLoopRange[i]] = dimColor.nscale8(0);
        }
        FastLED.show();
    }

    void initiate(void)
    {
        CRGB dimColor = this->mColor;
        for (int i = 1; i <= 8; i++)
        {
            uint8_t scaled = map(i, 0, 8, 255, this->mMinBrightness);
            CRGB dimColor = this->mColor;
            dimColor.nscale8(scaled); // Scale color intensity
            setPanelColor(dimColor);
            delay(25);
        }
    }

    int getMinBrightness() const
    {
        return this->mMinBrightness;
    }

    CRGB getPanelColor(void)
    {
        return this->mColor;
    }

private:
    CRGB *mLeds;
    int mLoopRange[NUM_LEDS_PER_PANEL];
    bool mDelluminating = false;
    int mMinBrightness = 20;
    int mCurrentBrightness = 20;
    bool mOn = false;

    unsigned long mLastFadeTime = 0;
    int mFadeIntervalMs = 25;
    int mFadeStep = 30;
    CRGB mColor;
};

template <int NUM_LEDS_PER_PANEL, int LED_DATA_PIN>
class DDRPad
{
public:
    DDRPanel<NUM_LEDS_PER_PANEL> up, right, down, left;
    DDRPanel<NUM_LEDS_PER_PANEL> *panels[4];

    DDRPad(CRGB *leds)
    {
        this->right = DDRPanel<NUM_LEDS_PER_PANEL>(leds, Panel::Right);
        this->down = DDRPanel<NUM_LEDS_PER_PANEL>(leds, Panel::Down);
        this->left = DDRPanel<NUM_LEDS_PER_PANEL>(leds, Panel::Left);
        this->up = DDRPanel<NUM_LEDS_PER_PANEL>(leds, Panel::Up);
        this->mLeds = leds;

        panels[0] = &right;
        panels[1] = &down;
        panels[2] = &left;
        panels[3] = &up;
    }

    void Begin(void)
    {
        FastLED.addLeds<WS2811, LED_DATA_PIN, GRB>(this->mLeds, NUM_LEDS_PER_PANEL * 4);
        FastLED.setBrightness(215);
        this->bootSequence();
    }

    void bootSequence(void)
    {
        // Turn all off first
        for (int i = 0; i < 4; i++)
        {
            panels[i]->turnOff();
            delay(500);
        }

        for (int i = 0; i < 4; i++)
        {
            panels[i]->illunimate();
            delay(500);
        }

        // Illuminate all at once
        for (int step = 1; step <= 8; step++)
        {
            uint8_t scaled = map(step, 0, 8, 255, panels[0]->getMinBrightness()); // All panels have same min
            for (int i = 0; i < 4; i++)
            {
                CRGB color = panels[i]->getPanelColor();
                color.nscale8(scaled);
                panels[i]->setPanelColor(color);
            }
            delay(25);
        }
    }

private:
    CRGB *mLeds;
};

#endif
