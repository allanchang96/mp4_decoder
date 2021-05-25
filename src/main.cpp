// Main file for MP4 Decoder

#include <iostream>
#include <string>
#include <unordered_map>
#include "block.h"
#include "blockdecode.h"
#include "videofile.h"

int main(void)
{

  const std::string fileName = "sample.mp4";
  VideoFile::file_open(fileName);

  std::cout << "Decoding \"" << fileName << "\"" << std::endl;
  
  uint64_t fileSize = VideoFile::getFileSize();
  std::cout << "File Size: " << fileSize << " bytes" << std::endl;

  uint64_t filePos = 0;
  while(filePos < fileSize - 1){
    Block blockInfo = VideoFile::getBlock();
    std::cout << "Block Size: " << blockInfo.blockSize << " bytes, Block Type: " << blockInfo.blockType << std::endl;

    // Branch into different blocks
    switch (BlockDecode::getBlockMapping(blockInfo.blockType))
    {
      case BlockDecode::type_ftyp:
        BlockDecode::ftyp::decode(blockInfo.blockSize);
        break;
      case BlockDecode::type_moov:
        BlockDecode::moov::decode(filePos, blockInfo.blockSize);
        break;
      default:
        break;
    }

    filePos += blockInfo.blockSize;
    VideoFile::setFilePos(filePos);
  }
  
  return 0;

}