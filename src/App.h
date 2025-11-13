#pragma once

#include <Arduino.h>
#include <XPT2046_Touchscreen.h>
#include <TFT_eSPI.h>

extern TFT_eSPI tft;

// Debounce a boolean reading for a given id (0..29)
bool debounceButton(bool reading, uint8_t id);

// Print raw touch point to Serial (for debugging)
void printTouchToSerial(const TS_Point &p, Stream &out = Serial);

// Convenience helpers that query the touchscreen instance.
// Return true when a touch is present on the right/left half (threshold default 1900).
bool touchRight(XPT2046_Touchscreen &ts, int threshold = 1900);
bool touchLeft(XPT2046_Touchscreen &ts, int threshold = 1900);
bool delayMillis(unsigned long lastTime, unsigned long interval);

// TFT print helpers that behave like Serial.print / Serial.println
void tftPrint(TFT_eSPI &tft, const String &s);
void tftPrint(TFT_eSPI &tft, const char *s);
void tftPrint(TFT_eSPI &tft, char c);

// void tftPrintln(TFT_eSPI &tft);
// void tftPrintln(TFT_eSPI &tft, const String &s);
void tftPrintln(TFT_eSPI &tft, const char *s);
void tftPrintln(TFT_eSPI &tft, char c);

// --- New: Serial-like wrappers that use the global 'tft' ---
// void tftPrint(const String &s);
void tftPrint(const char *s);
void tftPrint(char c);

// void tftPrintln();
// void tftPrintln(const String &s);
void tftPrintln(const char *s);
void tftPrintln(char c);
