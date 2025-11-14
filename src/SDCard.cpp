#include "SDCard.h"

bool SD_Reading = false; // Flag to indicate if SD card is being read

void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root)
  {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory())
  {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file)
  {
    if (file.isDirectory())
    {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels)
      {
        listDir(fs, file.path(), levels - 1);
      }
    }
    else
    {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void createDir(fs::FS &fs, const char *path)
{
  Serial.printf("Creating Dir: %s\n", path);
  if (fs.mkdir(path))
  {
    Serial.println("Dir created");
  }
  else
  {
    Serial.println("mkdir failed");
  }
}

void removeDir(fs::FS &fs, const char *path)
{
  Serial.printf("Removing Dir: %s\n", path);
  if (fs.rmdir(path))
  {
    Serial.println("Dir removed");
  }
  else
  {
    Serial.println("rmdir failed");
  }
}

std::vector<std::string> readFile(fs::FS &fs, const char *path, size_t maxLen, size_t startIdx, size_t windowSize)
{
  File file = fs.open(path);
  if (!file)
  {
    Serial.println("Failed to open file for reading");
    return {};
  }

  std::vector<std::string> segments;
  std::string buffer;

  while (file.available() && segments.size() < (startIdx + windowSize))
  {
    SD_Reading = true;
    char c = file.read();
    buffer += c;

    // If buffer is full or newline, push as a segment (don't split at every space)
    if (buffer.length() >= maxLen || c == '\n')
    {
      size_t splitPos = buffer.find_last_of(" ");
      if (splitPos == std::string::npos || c == '\n')
      {
        // No space found or newline: split at end
        segments.push_back(convertUTF8String(buffer));
        buffer.clear();
      }
      else
      {
        // Split at last space
        segments.push_back(convertUTF8String(buffer.substr(0, splitPos)));
        // Remove segment and the space from buffer
        buffer = buffer.substr(splitPos + 1);
      }
      // Trim trailing spaces/newlines
      while (!buffer.empty() && isspace(buffer.back()))
        buffer.pop_back();
    }
  }
  // Add any remaining buffer as a segment
  if (!buffer.empty() && segments.size() < (startIdx + windowSize))
    segments.push_back(buffer);
  file.close();

  // Get only the window of segments
  std::vector<std::string> window;
  for (size_t i = startIdx; i < startIdx + windowSize && i < segments.size(); ++i)
  {
    window.push_back(segments[i]);
  }
  return window;
}

bool writeFile(fs::FS &fs, const char *path, const char *message)
{
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file)
  {
    Serial.println("Failed to open file for writing");
    return false;
  }
  if (file.print(message))
  {
    Serial.println("File written");
  }
  else
  {
    Serial.println("Write failed");
  }
  file.close();
  return file.print(message);
}

void appendFile(fs::FS &fs, const char *path, const char *message)
{
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file)
  {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.print(message))
  {
    Serial.println("Message appended");
  }
  else
  {
    Serial.println("Append failed");
  }
  file.close();
}

void renameFile(fs::FS &fs, const char *path1, const char *path2)
{
  Serial.printf("Renaming file %s to %s\n", path1, path2);
  if (fs.rename(path1, path2))
  {
    Serial.println("File renamed");
  }
  else
  {
    Serial.println("Rename failed");
  }
}

void deleteFile(fs::FS &fs, const char *path)
{
  Serial.printf("Deleting file: %s\n", path);
  if (fs.remove(path))
  {
    Serial.println("File deleted");
  }
  else
  {
    Serial.println("Delete failed");
  }
}