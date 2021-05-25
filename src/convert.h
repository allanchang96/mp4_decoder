// Contains functions to convert an array of chars to unsigned integer values

#include <array>

#ifndef CONVERT_H
#define CONVERT_H

namespace Convert
{
  uint32_t convert32(std::array<char, 4> &arr);
  uint64_t convert64(std::array<char, 8> &arr);

  // Convert to human-readable date, given time in seconds since 1904-01-01
  void convertDate(uint64_t seconds);
};

#endif