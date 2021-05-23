#include <array>
#include "videofile.h"
#include "convert.h"
#include <iostream>

// Set the position of the file stream
void VideoFile::setFilePos(uint64_t pos)
{
  file.seekg(pos);
  return;
}

// Read 8 bytes and returns information of the block
Block VideoFile::getBlock()
{

  Block blockInfo;

  // Read the block size
  std::array<char, 4> blockSizeBuffer;
  file.read(blockSizeBuffer.data(), blockSizeBuffer.size());

  blockInfo.blockSize = Convert::convert32(blockSizeBuffer);
  
  // Read the block type
  std::array<char, 4> blockTypeBuffer;
  file.read(blockTypeBuffer.data(), blockTypeBuffer.size());

  blockInfo.blockType = std::string(blockTypeBuffer.data(), blockTypeBuffer.size());

  return blockInfo;
}

// Constructor
VideoFile::VideoFile(std::string fileName)
{
  file.open(fileName, std::ios::in | std::ios::binary);
}

// Destructor
VideoFile::~VideoFile()
{
  file.close();
}