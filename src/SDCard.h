#pragma once

#include <Arduino.h>
#include <string>
#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include "TextUtils.h"

extern bool SD_Reading; // Flag to indicate if SD card is being read

// Function declarations for SD card operations
void setupSDCard();
void listDir(fs::FS &fs, const char *path, uint8_t levels);
void createDir(fs::FS &fs, const char *path);
void removeDir(fs::FS &fs, const char *path);
std::vector<std::string> readFile(fs::FS &fs, const char *path, size_t maxLen, size_t startIdx, size_t windowSize);
bool writeFile(fs::FS &fs, const char *path, const char *message);
void appendFile(fs::FS &fs, const char *path, const char *message);
void renameFile(fs::FS &fs, const char *path1, const char *path2);
void deleteFile(fs::FS &fs, const char *path);