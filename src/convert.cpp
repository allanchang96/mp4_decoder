// Contains functions to convert an array of chars to unsigned integer values
#include <iostream>
#include <array>
#include <string>
#include "convert.h"

uint16_t Convert::convert16(std::array<char, 2> &arr)
{
  uint16_t val = 0;
  for(uint16_t i = 0; i < 1; i++){
    val |= reinterpret_cast<uint8_t&> (arr[i]);
    val <<= 8;
  }
  val |= reinterpret_cast<uint8_t&> (arr[1]);
  return val;
}

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

// Convert to human-readable date, given time in seconds since 1904-01-01
void Convert::convertDate(uint64_t seconds)
{
  // Initialization constants
  const uint64_t startYear = 1904;
  const uint64_t startWeek = 4;
  const uint64_t yearToLeap = (4 - (startYear % 4)) % 4;
  const std::array<uint64_t, 12> monthsInYear = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
  const std::array<uint64_t, 12> leapMonthsInYear = {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335};
  const std::array<std::string, 12> monthName = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  const std::array<std::string, 7> weekName = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
  uint64_t minutes = seconds / 60;
  uint64_t hours = minutes / 60;
  uint64_t days = hours / 24;
  // Add one because we want current date, and days is how many full days elapsed
  uint64_t remDays = days;
  uint64_t years = 0;
  if(remDays > yearToLeap * 365){
    years += yearToLeap;
    remDays -= yearToLeap * 365;
  }
  // Count by leap years
  uint64_t numOfLeapCycles = remDays / 1461;
  years += numOfLeapCycles * 4;
  remDays %= 1461;
  // Record if it is a leap year this year
  bool leapYear = true;
  // Handle first year leap
  if(remDays >= 366){
    years++;
    remDays -= 366;
    leapYear = false;
  }
  // Handle remaining years
  years += remDays / 365;
  remDays = leapYear ? remDays % 366 : remDays % 365;
  // Get day of week name
  uint64_t weekIndex = (startWeek + days) % 7;
  std::cout << weekName[weekIndex] << " ";
  // Get the month name and day of month
  uint64_t monthIndex = (leapYear ? std::upper_bound(leapMonthsInYear.begin(), leapMonthsInYear.end(), remDays) - leapMonthsInYear.begin()
                          : std::upper_bound(monthsInYear.begin(), monthsInYear.end(), remDays) - monthsInYear.begin()) - 1;
  uint64_t dayOfMonth = (leapYear ? remDays - leapMonthsInYear[monthIndex] : remDays - monthsInYear[monthIndex]) + 1;
  std::cout << monthName[monthIndex] << " " << dayOfMonth << " ";
  // Get the hour, minute and second
  uint64_t displaySeconds = seconds % 60;
  uint64_t displayMinutes = minutes % 60;
  uint64_t displayHours = hours % 24;
  if(displayHours < 10){
    std::cout << "0";
  }
  std::cout << displayHours << ":";
  if(displayMinutes < 10){
    std::cout << "0";
  }
  std::cout << displayMinutes << ":";
  if(displaySeconds < 10){
    std::cout << "0";
  }
  std::cout << displaySeconds << " ";
  std::cout << startYear + years;
}

// Convert time in number of total units, to HH:MM:SS
void Convert::convertDuration(uint64_t unitCount, uint64_t timeUnit)
{
  uint64_t seconds = (unitCount + (timeUnit / 2)) / timeUnit;
  uint64_t displayMinutes = seconds / 60;
  uint64_t displayHours = displayMinutes / 60;
  uint64_t displaySeconds = seconds % 60;
  if(displayHours < 10){
    std::cout << "0";
  }
  std::cout << displayHours << ":";
  if(displayMinutes < 10){
    std::cout << "0";
  }
  std::cout << displayMinutes << ":";
  if(displaySeconds < 10){
    std::cout << "0";
  }
  std::cout << displaySeconds;
}

// Convert from fixed point to float
float Convert::convertFixedPoint(uint64_t val, uint32_t offset)
{
  return val / (1 << offset);
}