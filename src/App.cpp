#include "App.h"

// ensure global tft is visible
extern TFT_eSPI tft;

// --- Implementations that take a TFT_eSPI& ---
// These forward directly to the TFT_eSPI print/println methods.
// void tftPrint(TFT_eSPI &tft, const String &s) { tft.print(s); }
void tftPrint(TFT_eSPI &tft, const char *s) { tft.print(s); }
void tftPrint(TFT_eSPI &tft, char c) { tft.print(c); }

// void tftPrintln(TFT_eSPI &tft) { tft.println(); }
// void tftPrintln(TFT_eSPI &tft, const String &s) { tft.println(s); }
void tftPrintln(TFT_eSPI &tft, const char *s) { tft.println(s); }
void tftPrintln(TFT_eSPI &tft, char c) { tft.println(c); }

// --- Forwarders that use the global 'tft' ---
// void tftPrint(const String &s) { tftPrint(tft, s); }
void tftPrint(const char *s) { tftPrint(tft, s); }
void tftPrint(char c) { tftPrint(tft, c); }

// void tftPrintln() { tftPrintln(tft); }
// void tftPrintln(const String &s) { tftPrintln(tft, s); }
void tftPrintln(const char *s) { tftPrintln(tft, s); }
void tftPrintln(char c) { tftPrintln(tft, c); }

bool delayMillis(unsigned long lastTime, unsigned long interval)
{
    return (millis() - lastTime) >= interval;
}

// --- Existing utilities ---
bool debounceButton(bool reading, uint8_t id)
{
    static const uint8_t MAX_BUTTONS = 30;
    static bool lastState[MAX_BUTTONS] = {false};
    static unsigned long lastChange[MAX_BUTTONS] = {0};
    const unsigned long DEBOUNCE_MS = 50;

    if (id >= MAX_BUTTONS)
    {
        return reading;
    }

    unsigned long now = millis();
    if (reading != lastState[id] && (now - lastChange[id]) >= DEBOUNCE_MS)
    {
        lastState[id] = reading;
        lastChange[id] = now;
    }
    return lastState[id];
}

void printTouchToSerial(const TS_Point &p, Stream &out)
{
    out.print(F("Pressure = "));
    out.print(p.z);
    out.print(F(", x = "));
    out.print(p.x);
    out.print(F(", y = "));
    out.print(p.y);
    out.println();
}

bool touchRight(XPT2046_Touchscreen &ts, int threshold)
{
    if (!ts.touched())
        return false;
    TS_Point p = ts.getPoint();
    return (p.y < threshold);
}

bool touchLeft(XPT2046_Touchscreen &ts, int threshold)
{
    if (!ts.touched())
        return false;
    TS_Point p = ts.getPoint();
    return (p.y > threshold);
}
