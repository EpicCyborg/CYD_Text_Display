#pragma once
#include <string>
#include <vector>

std::string readLocalFile(const std::string &filename);
std::vector<std::string> splitTextByWord(const std::string &text, size_t maxLen);