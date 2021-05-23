// Decode functions

#include <unordered_map>
#include <iostream>
#include "videofile.h"
#include "blockdecode.h"

extern VideoFile file;

// Mapping from string to enum
static std::unordered_map<std::string, BlockDecode::BlockType> decodeMap({
  {"ftyp", BlockDecode::type_ftyp}
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
  std::cout << "Brand: " << file.readString(4, blockPos) << std::endl;
  std::cout << "Brand Version: " << file.read32(blockPos) << std::endl;
  if(blockPos < blockSize){
    std::cout << "Compatible Brands: ";
    while(blockPos < blockSize){
      std::cout << file.readString(4, blockPos);
      if(blockPos < blockSize){
        std::cout << ", ";
      }
    }
  }
  std::cout << std::endl;
}