// Decode functions

#ifndef BLOCK_DECODE_H
#define BLOCK_DECODE_H

#include <string>

namespace BlockDecode
{
  enum BlockType
  {
    type_ftyp = 1,
    type_moov,
    type_mvhd,
    type_trak,
    type_tkhd,
  };

  // Returns the enum mapping of the given string
  BlockType getBlockMapping(std::string blockType);

  // Functions for ftyp block
  namespace ftyp
  {
    void decode(uint64_t blockSize);
  }

  // Functions for moov block
  namespace moov
  {
    void decode(uint64_t blockBase, uint64_t blockSize);
  }

  // Functions for mvhd block
  namespace mvhd
  {
    void decode(uint64_t blockSize);
  }

  // Functions for trak block
  namespace trak
  {
    void decode(uint64_t blockBase, uint64_t blockSize);
  }

  // Functions for tkhd block
  namespace tkhd
  {
    void decode(uint64_t blockSize);
  }

}

#endif