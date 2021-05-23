// Container to hold the mp4 after it is read into a stream

#ifndef VIDEOFILE_H
#define VIDEOFILE_H

#include <fstream>
#include <string>
#include "block.h"

class VideoFile
{

  private:
    // Store the file in a stream
    std::fstream file;
    uint64_t fileSize;

  public:

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

    // Constructor/Destructor
    VideoFile(std::string fileName);
    ~VideoFile(void);
  

};

#endif