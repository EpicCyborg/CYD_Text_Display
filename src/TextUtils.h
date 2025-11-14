#pragma once
#include <string>
#include <vector>

std::string readLocalFile(const std::string &filename);
std::string convertUTF8String(const std::string &input);
std::vector<std::string> splitTextByWord(const std::string &text, size_t maxLen);