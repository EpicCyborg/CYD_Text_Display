#pragma once

#include <Arduino.h>
#include <string>
#include <FS.h>
#include <SD.h>
#include <SPI.h>

// Function declarations for SD card operations
void setupSDCard();
void listDir(fs::FS &fs, const char *path, uint8_t levels);
void createDir(fs::FS &fs, const char *path);
void removeDir(fs::FS &fs, const char *path);
std::string readFile(fs::FS &fs, const char *path);
void writeFile(fs::FS &fs, const char *path, const char *message);
void appendFile(fs::FS &fs, const char *path, const char *message);
void renameFile(fs::FS &fs, const char *path1, const char *path2);
void deleteFile(fs::FS &fs, const char *path);