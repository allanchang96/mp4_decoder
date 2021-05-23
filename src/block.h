// Structure to hold information about mp4 blocks

#ifndef BLOCK_H
#define BLOCK_H

#include <string>

struct Block
{
  uint64_t blockSize;
  std::string blockType;
};

#endif