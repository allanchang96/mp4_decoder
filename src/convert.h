// Contains functions to convert an array of chars to unsigned integer values

#include <array>

#ifndef CONVERT_H
#define CONVERT_H

namespace Convert
{
  uint16_t convert16(std::array<char, 2> &arr);
  uint32_t convert32(std::array<char, 4> &arr);
  uint64_t convert64(std::array<char, 8> &arr);

  // Convert to human-readable date, given time in seconds since 1904-01-01
  void convertDate(uint64_t seconds);

  // Convert time in number of total units, to HH:MM:SS
  void convertDuration(uint64_t unitCount, uint64_t timeUnit);

  // Convert from fixed point to float
  float convertFixedPoint(uint64_t val, uint32_t offset);
};

#endif