#include "TextUtils.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::string readLocalFile(const std::string &filename)
{
    std::ifstream file(filename, std::ios::in);
    if (!file)
    {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return "";
    }

    std::string content((std::istreambuf_iterator<char>(file)),
                        (std::istreambuf_iterator<char>()));
    return content;
}

// Splits text into segments of up to maxLen characters, cutting only at spaces.
// Preserves line breaks by processing each line independently.
std::vector<std::string> splitTextByWord(const std::string &text, size_t maxLen)
{
    std::vector<std::string> segments;
    std::istringstream textStream(text);
    std::string line;

    while (std::getline(textStream, line))
    {
        std::istringstream iss(line);
        std::string word, segment;

        while (iss >> word)
        {
            if (segment.empty())
                segment = word;
            else if (segment.length() + 1 + word.length() <= maxLen)
                segment += " " + word;
            else
            {
                segments.push_back(segment);
                segment = word;
            }
        }
        if (!segment.empty())
        {
            segment += '\n'; // keep original line break
            segments.push_back(segment);
        }
    }
    return segments;
}