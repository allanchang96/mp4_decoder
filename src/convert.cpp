// Contains functions to convert an array of chars to unsigned integer values

#include "convert.h"

uint32_t Convert::convert32(std::array<char, 4> &arr)
{
  uint32_t val = 0;
  for(uint32_t i = 0; i < 3; i++){
    val |= reinterpret_cast<uint8_t&> (arr[i]);
    val <<= 8;
  }
  val |= reinterpret_cast<uint8_t&> (arr[3]);
  return val;
}

uint64_t Convert::convert64(std::array<char, 8> &arr)
{
  int64_t val = 0;
  for(uint64_t i = 0; i < 7; i++){
    val |= reinterpret_cast<uint8_t&> (arr[i]);
    val <<= 8;
  }
  val |= reinterpret_cast<uint8_t&> (arr[7]);
  return val;
}