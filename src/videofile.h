// Container to hold the mp4 after it is read into a stream

#ifndef VIDEOFILE_H
#define VIDEOFILE_H

#include <fstream>
#include <string>
#include <vector>
#include "block.h"

namespace VideoFile
{

  namespace{
    // Store the file in a stream
    std::fstream file;
    uint64_t fileSize;
  }

  // Set the position of the file stream
  void setFilePos(uint64_t pos);

  // Get fileSize
  uint64_t getFileSize(void);

  // Read bytes as ASCII string and update pos value
  std::string readString(uint32_t numOfBytes, uint64_t &pos);

  // Read bytes as 32-bit unsigned int and update pos value
  uint32_t read32(uint64_t &pos);

  // Read bytes as 64-bit unsigned int
  uint64_t read64(uint64_t &pos);

  // Generic function to read numOfBytes
  std::vector<char> readBytes(uint32_t numOfBytes, uint64_t &pos);

  // Read 8 bytes and returns information of the block
  Block getBlock(void);

  // Opening/Closing file
  void file_open(std::string fileName);
  void file_close(void);
  
};

#endif