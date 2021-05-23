// Main file for MP4 Decoder

#include <iostream>
#include "block.h"
#include "videofile.h"

int main(void)
{
  VideoFile file("sample.mp4");
  Block blockInfo = file.getBlock();
  std::cout << blockInfo.blockSize << std::endl;
  std::cout << blockInfo.blockType << std::endl;
  return 0;

}