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

  public:

    // Set the position of the file stream
    void setFilePos(uint64_t pos);

    // Read 8 bytes and returns information of the block
    Block getBlock();

    // Constructor/Destructor
    VideoFile(std::string fileName);;
    ~VideoFile();
  

};

#endif