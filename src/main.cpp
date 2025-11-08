#include <Arduino.h>
#include "Typewriter.h"
#include "TextUtils.h"
#include "App.h"
#include "SDCard.h"
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <SPI.h>

// Variables
static std::string text;
static std::vector<std::string> segments;
static size_t segIdx = 0;
static size_t pageIdx = 0;
static Typewriter tw; // Typewriter instance
#define NEXTPIN 22    // Pulldown button pins
#define PREVPIN 21
#define DEBOUNCE_MS 50 // Debounce time in milliseconds
bool next;
bool prev;
bool segfinished = false;  // false if not finished, true when line is finished
bool pagefinished = false; // false if not finished, true when page is finished
bool whenpressed = false;  // false if not finished, true when pressed after finished text
int lines = 10;            // Number of lines per page
int fasttext = 10;         // Fast text delay (ms)
int slowtext = 40;         // Slow text delay (ms)

char c; // Character being typed

// Touchscreen setup
#define XPT2046_IRQ 36
#define XPT2046_MOSI 13
#define XPT2046_MISO 12
#define XPT2046_CLK 14
#define XPT2046_CS 33
#define TFT_BL 27                // Backlight Pin
SPIClass sdSPI = SPIClass(VSPI); // VSPI default pins, for SD card
SPIClass mySPI(VSPI);            // VSPI default pins, for touchscreen

XPT2046_Touchscreen ts(XPT2046_CS, XPT2046_IRQ);
TFT_eSPI tft = TFT_eSPI();

void setup()
{
  Serial.begin(115200);
  delay(500); // Wait for USB CDC

  // Initialize SD card

  int i = 0;
  while (!SD.begin(SS, sdSPI, 4000000))
  {
    Serial.println("Card Mount Failed");
    delay(10);
    i++;
  }
  Serial.printf("Card mount succeeded in %i tries", i);
  Serial.println();
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE)
  {
    Serial.println("No SD card attached");
    return;
  }

  Serial.println("Typewriter starting...");

  pinMode(NEXTPIN, INPUT_PULLUP);
  pinMode(PREVPIN, INPUT_PULLUP);

  // Retrieve text from SD card:
  text = readFile(SD, "/text.txt");
  Serial.println("Raw text from SD:");
  Serial.println(text.c_str()); // Print the raw text
  Serial.println("---END OF RAW TEXT---");

  Serial.printf("Text length: %d\n", text.length());

  Serial.println("Splitting text...");
  segments = splitTextByWord(text, 26);

  Serial.printf("Loaded %d segments\n", segments.size());
  if (segments.size() > 0)
  {
    Serial.println("First segment:");
    Serial.println(segments[0].c_str());
  }
  Serial.println("---END OF SEGMENTS---");
  Serial.println("Setup complete! Starting typewriter effect...");

  // Start TFT
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH); // Turn backlight on

  // Start SPI for touchscreen
  mySPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
  ts.begin(mySPI);
  ts.setRotation(1);

  // Start TFT
  tft.init();
  tft.setRotation(1);
  tft.writecommand(0x36);
  tft.writedata(0x48);

  tft.fillScreen(TFT_BLACK);

  // Set font and color
  tft.setTextColor(TFT_YELLOW, TFT_BLACK); // Yellow text, black background
  tft.setTextDatum(MC_DATUM);              // Middle center
  tft.setTextSize(2);

  tft.drawString("Touch Screen to Start", tft.width() / 2, 100);
  tft.setTextDatum(TL_DATUM);
}

void loop()
{
  // Detect Touch Screen
  if (ts.touched())
  {
    TS_Point p = ts.getPoint();
    // printTouchToSerial(p);
  }
  next = debounceButton(touchRight(ts), 2) == true;
  prev = debounceButton(touchLeft(ts), 3) == true;
  // next = debounceButton(digitalRead(NEXTPIN), 0) == LOW;
  //  prev = debounceButton(digitalRead(PREVPIN), 1) == LOW;

  // Always call tick while segment is not finished
  if (!segfinished && segIdx < segments.size())
  { // Show 10 segments (lines) on a page
    tw.tick(segments[segIdx].c_str(), fasttext * next + slowtext * (1 - next));
    // If typewriter finished, set finished flag
    if (!tw.textrunning_)
    {
      tw.reset();
      segfinished = true;
    }

    if (prev)
    {
      tw.reset();
      tft.setCursor(0, 0);
      tftPrint(tft, segments[segIdx].c_str());
      segfinished = true;
    }
  }
  // Handle button presses to change segments
  else if (segfinished)
  {
    if (next == 0 && prev == 0)
    {
      whenpressed = 1; // Set flag for when button is pressed while segment is finished
    }
    if (whenpressed == 1 && next && segIdx < segments.size() - 1) // If not last segment
    {
      segIdx++;
      whenpressed = 0;
      tw.reset();
      segfinished = false;
      // Clear screen
      tft.fillScreen(TFT_BLACK);
      tft.setCursor(0, 0);
    }
    else if (whenpressed == 1 && prev && segIdx > 0) // If not first segment
    {
      segIdx--;
      whenpressed = 0;
      tw.reset();
      segfinished = false;
      // Clear screen
      tft.fillScreen(TFT_BLACK);
      tft.setCursor(0, 0);
    }
  }
}