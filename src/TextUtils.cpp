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

std::string convertUTF8String(const std::string &input) // Converts UTF-8 characters for ASCII for TFT display
{
    std::string output;
    for (size_t i = 0; i < input.size(); ++i)
    {
        // Curly apostrophe: ’ (UTF-8: 0xE2 0x80 0x99)
        if ((unsigned char)input[i] == 0xE2 &&
            i + 2 < input.size() &&
            (unsigned char)input[i + 1] == 0x80 &&
            (unsigned char)input[i + 2] == 0x99)
        {
            output += '\'';
            i += 2;
        }
        // Curly single open quote: ‘ (UTF-8: 0xE2 0x80 0x98)
        else if ((unsigned char)input[i] == 0xE2 &&
                 i + 2 < input.size() &&
                 (unsigned char)input[i + 1] == 0x80 &&
                 (unsigned char)input[i + 2] == 0x98)
        {
            output += '\'';
            i += 2;
        }
        // Curly double open quote: “ (UTF-8: 0xE2 0x80 0x9C)
        else if ((unsigned char)input[i] == 0xE2 &&
                 i + 2 < input.size() &&
                 (unsigned char)input[i + 1] == 0x80 &&
                 (unsigned char)input[i + 2] == 0x9C)
        {
            output += '"';
            i += 2;
        }
        // Curly double close quote: ” (UTF-8: 0xE2 0x80 0x9D)
        else if ((unsigned char)input[i] == 0xE2 &&
                 i + 2 < input.size() &&
                 (unsigned char)input[i + 1] == 0x80 &&
                 (unsigned char)input[i + 2] == 0x9D)
        {
            output += '"';
            i += 2;
        }
        // En dash: –
        else if ((unsigned char)input[i] == 0xE2 &&
                 i + 2 < input.size() &&
                 (unsigned char)input[i + 1] == 0x80 &&
                 (unsigned char)input[i + 2] == 0x93)
        {
            output += '-';
            i += 2;
        }
        // Em dash: —
        else if ((unsigned char)input[i] == 0xE2 &&
                 i + 2 < input.size() &&
                 (unsigned char)input[i + 1] == 0x80 &&
                 (unsigned char)input[i + 2] == 0x94)
        {
            output += '-';
            i += 2;
        }
        // Ellipsis: …
        else if ((unsigned char)input[i] == 0xE2 &&
                 i + 2 < input.size() &&
                 (unsigned char)input[i + 1] == 0x80 &&
                 (unsigned char)input[i + 2] == 0xA6)
        {
            output += "...";
            i += 2;
        }
        // Basic accented letters (é, ü, etc.)
        else if ((unsigned char)input[i] == 0xC3 && i + 1 < input.size())
        {
            unsigned char next = (unsigned char)input[i + 1];
            switch (next)
            {
            case 0xA9:
                output += 'e';
                break; // é
            case 0xBC:
                output += 'u';
                break; // ü
            case 0xB6:
                output += 'o';
                break; // ö
            case 0xA4:
                output += 'a';
                break; // ä
            case 0xB1:
                output += 'n';
                break; // ñ
            // Add more as needed
            default:
                output += '?';
                break; // Unknown
            }
            i += 1;
        }
        else
        {
            output += input[i];
        }
    }
    return output;
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