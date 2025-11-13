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
void tftPrint(TFT_eSPI &tft, int v);
void tftPrint(TFT_eSPI &tft, unsigned int v);
void tftPrint(TFT_eSPI &tft, long v);
void tftPrint(TFT_eSPI &tft, unsigned long v);
void tftPrint(TFT_eSPI &tft, float v, int decimals = 2);
void tftPrint(TFT_eSPI &tft, double v, int decimals = 2);

void tftPrintln(TFT_eSPI &tft);
void tftPrintln(TFT_eSPI &tft, const String &s);
void tftPrintln(TFT_eSPI &tft, const char *s);
void tftPrintln(TFT_eSPI &tft, char c);
void tftPrintln(TFT_eSPI &tft, int v);
void tftPrintln(TFT_eSPI &tft, unsigned int v);
void tftPrintln(TFT_eSPI &tft, long v);
void tftPrintln(TFT_eSPI &tft, unsigned long v);
void tftPrintln(TFT_eSPI &tft, float v, int decimals = 2);
void tftPrintln(TFT_eSPI &tft, double v, int decimals = 2);

// --- New: Serial-like wrappers that use the global 'tft' ---
void tftPrint(const String &s);
void tftPrint(const char *s);
void tftPrint(char c);
void tftPrint(int v);
void tftPrint(unsigned int v);
void tftPrint(long v);
void tftPrint(unsigned long v);
void tftPrint(float v, int decimals = 2);
void tftPrint(double v, int decimals = 2);

void tftPrintln();
void tftPrintln(const String &s);
void tftPrintln(const char *s);
void tftPrintln(char c);
void tftPrintln(int v);
void tftPrintln(unsigned int v);
void tftPrintln(long v);
void tftPrintln(unsigned long v);
void tftPrintln(float v, int decimals = 2);
void tftPrintln(double v, int decimals = 2);