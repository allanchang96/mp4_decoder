// Decode functions

#include <unordered_map>
#include <iostream>
#include <ctime>
#include "videofile.h"
#include "blockdecode.h"
#include "convert.h"

// Mapping from string to enum
std::unordered_map<std::string, BlockDecode::BlockType> decodeMap({
  {"ftyp", BlockDecode::type_ftyp},
  {"moov", BlockDecode::type_moov},
  {"mvhd", BlockDecode::type_mvhd},
});

// Returns the enum mapping of the given string
BlockDecode::BlockType BlockDecode::getBlockMapping(std::string blockType)
{
  return decodeMap[blockType];
}

// ftyp decode
void BlockDecode::ftyp::decode(uint64_t blockSize)
{
  // Current position within the box
  uint64_t blockPos = 8;

  std::cout << "Brand: " << VideoFile::readString(4, blockPos) << std::endl;
  std::cout << "Brand Version: " << VideoFile::read32(blockPos) << std::endl;
  if(blockPos < blockSize){
    std::cout << "Compatible Brands: ";
    while(blockPos < blockSize){
      std::cout << VideoFile::readString(4, blockPos);
      if(blockPos < blockSize){
        std::cout << ", ";
      }
    }
  }
  std::cout << std::endl;
}

// moov decode
void BlockDecode::moov::decode(uint64_t blockBase, uint64_t blockSize)
{
  // Current position within the box
  uint64_t blockPos = 8;

  while(blockPos < blockSize - 1){
    Block blockInfo = VideoFile::getBlock();
    std::cout << "-Block Size: " << blockInfo.blockSize << " bytes, Block Type: " << blockInfo.blockType << std::endl;

    // Branch into different blocks
    switch (BlockDecode::getBlockMapping(blockInfo.blockType))
    {
      case type_mvhd:
        BlockDecode::mvhd::decode(blockBase + blockPos, blockInfo.blockSize);
        break;
      default:
        break;
    }

    blockPos += blockInfo.blockSize;
    VideoFile::setFilePos(blockBase + blockPos);
  }
}

// mvhd decode
void BlockDecode::mvhd::decode(uint64_t blockBase, uint64_t blockSize)
{
  // Current position within the box
  uint64_t blockPos = 8;

  // If version is 1, date and duration values are 8 bytes long, otherwise 4.

  // VERSION
  std::vector<char> versionBuffer = VideoFile::readBytes(1, blockPos);
  uint32_t version = versionBuffer[0];

  std::cout << "--Version: " << version << std::endl;
  // VERSION

  // FLAGS
  std::vector<char> hexFlagsBuffer = VideoFile::readBytes(3, blockPos);

  std::cout << "--Flags: ";
  // Change output to hex
  std::cout << std::hex;
  for(auto c : hexFlagsBuffer){
    // https://stackoverflow.com/questions/673240/how-do-i-print-an-unsigned-char-as-hex-in-c-using-ostream
    std::cout << +c;
  }
  std::cout << std::endl;
  std::cout << std::dec;
  // FLAGS

  // CREATED MODIFIED TIME
  uint64_t createdTime;
  uint64_t modifiedTime;
  if(version == 0){
    createdTime = VideoFile::read32(blockPos);
    modifiedTime = VideoFile::read32(blockPos);
    std::cout << createdTime << std::endl;
    std::cout << modifiedTime << std::endl;
  }
  else{
    createdTime = VideoFile::read64(blockPos);
    modifiedTime = VideoFile::read64(blockPos);
  }
  Convert::convertDate(createdTime);
  Convert::convertDate(modifiedTime);
  
}