#include <Arduino.h>
#include "Typewriter.h"
#include "App.h" // <- ensure tftPrint declaration is visible

Typewriter::Typewriter() : textrunning_(false), texttime_(0), pendingDelay_(0), i_(0) {}

void Typewriter::reset()
{
    textrunning_ = false;
    texttime_ = 0;
    pendingDelay_ = 0;
    i_ = 0;
}

bool Typewriter::delayMillis_(unsigned long lastTime, unsigned long interval)
{
    return (millis() - lastTime) >= interval;
}

void Typewriter::tick(const char *text, int delay_ms)
{
    if (!textrunning_)
    {
        texttime_ = millis();
        pendingDelay_ = 0;
        textrunning_ = true;
    }

    char c = text[i_];
    if (c == '\0')
    {
        textrunning_ = false;
        i_ = 0;
        pendingDelay_ = 0;
        return;
    }

    // Wait for base delay + any pending extra delay
    if (!delayMillis_(texttime_, delay_ms + pendingDelay_))
        return;

    // Print now
    Serial.print(c);
    if (c != '\n')
        tftPrint(c);
    ++i_;

    // Compute extra delay that should apply AFTER this character (for the next interval)
    unsigned long extraDelay = 0;
    if (c == '.' || c == '!' || c == '?')
    {
        // longer pause after sentence end
        extraDelay = delay_ms * 8;
    }
    else if (c == '"' || c == '\'')
    {
        extraDelay = delay_ms * 2;
    }
    else if (c == ',' || c == ';' || c == ':')
    {
        extraDelay = delay_ms * 3;
    }
    else if (c == '\n')
    {
        extraDelay = delay_ms * 6;
    }

    // set pending delay for next tick and reset reference time
    pendingDelay_ = extraDelay;
    texttime_ = millis();
}