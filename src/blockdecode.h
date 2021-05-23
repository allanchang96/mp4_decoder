// Decode functions

#ifndef BLOCK_DECODE_H
#define BLOCK_DECODE_H

#include <string>

namespace BlockDecode
{
  enum BlockType
  {
    type_ftyp = 1,
  };

  // Returns the enum mapping of the given string
  BlockType getBlockMapping(std::string blockType);

  // Functions for ftyp block
  namespace ftyp
  {
    void decode(uint64_t blockSize);
  }

}

#endif