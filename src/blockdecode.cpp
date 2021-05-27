// Decode functions

#include <unordered_map>
#include <iostream>
#include <ctime>
#include "videofile.h"
#include "blockdecode.h"
#include "convert.h"

// TODO: Placeholder spot until better solution
uint32_t timeUnit;

// Mapping from string to enum
std::unordered_map<std::string, BlockDecode::BlockType> decodeMap({
  {"ftyp", BlockDecode::type_ftyp},
  {"moov", BlockDecode::type_moov},
  {"mvhd", BlockDecode::type_mvhd},
  {"trak", BlockDecode::type_trak},
  {"tkhd", BlockDecode::type_tkhd},
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
  assert(blockPos == blockSize);
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
        BlockDecode::mvhd::decode(blockInfo.blockSize);
        break;
      case type_trak:
        BlockDecode::trak::decode(blockBase + blockPos, blockInfo.blockSize);
        break;
      default:
        break;
    }

    blockPos += blockInfo.blockSize;
    VideoFile::setFilePos(blockBase + blockPos);
  }
}

// mvhd decode
void BlockDecode::mvhd::decode(uint64_t blockSize)
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
  std::cout << "--Flags: ";
  // Change output to hex
  std::cout << std::hex;
  for(auto c : VideoFile::readBytes(3, blockPos)){
    // https://stackoverflow.com/questions/673240/how-do-i-print-an-unsigned-char-as-hex-in-c-using-ostream
    if(c < 0x10){
      std::cout << 0;
    }
    std::cout << +c;
  }
  std::cout << std::endl;
  std::cout << std::dec;
  // FLAGS

  // CREATED MODIFIED TIME
  std::cout << "--Creation Time: ";
  version ? Convert::convertDate(VideoFile::read64(blockPos)) 
          : Convert::convertDate(VideoFile::read32(blockPos));
  std::cout << std::endl;
  std::cout << "--Modification Time: ";
  version ? Convert::convertDate(VideoFile::read64(blockPos)) 
          : Convert::convertDate(VideoFile::read32(blockPos));
  std::cout << std::endl;
  // CREATED MODIFIED TIME

  // TIME SCALE
  timeUnit = VideoFile::read32(blockPos);
  std::cout << "--Time Scale: " << timeUnit << std::endl;
  // TIME SCALE

  // DURATION
  uint64_t duration = version ? VideoFile::read64(blockPos) : VideoFile::read32(blockPos);
  std::cout << "--Duration: " << duration << " (";
  Convert::convertDuration(duration, timeUnit);
  std::cout << ")" << std::endl;
  // DURATION

  // USER PLAYBACK SPEED
  std::cout << "--Preferred Rate: " << VideoFile::read32(blockPos) << std::endl;
  // USER PLAYBACK SPEED

  // USER VOLUME
  std::cout << "--Preferred Volume: " << VideoFile::read16(blockPos) << std::endl;
  // USER VOLUME

  // RESERVED 10 BYTES
  std::cout << "--Reserved: ";
  for(int i = 0; i < 4; i++){
    std::cout << VideoFile::read16(blockPos) << " ";
  }
  std::cout << VideoFile::read16(blockPos) << std::endl;
  // RESERVRED 10 BYTES

  // TRANSFORMATION MATRIX
  std::cout << "--Matrix Structure:" << std::endl;
  std::cout << VideoFile::read32(blockPos) << " " << VideoFile::read32(blockPos) << " " << VideoFile::read32(blockPos) << std::endl;
  std::cout << VideoFile::read32(blockPos) << " " << VideoFile::read32(blockPos) << " " << VideoFile::read32(blockPos) << std::endl;
  std::cout << VideoFile::read32(blockPos) << " " << VideoFile::read32(blockPos) << " " << VideoFile::read32(blockPos) << std::endl;
  // TRANSFORMATION MATRIX

  // QUICKTIME PREVIEW
  uint32_t previewStartTime = VideoFile::read32(blockPos);
  uint32_t previewDuration = VideoFile::read32(blockPos);
  std::cout << "--Preview Time: " << previewStartTime << " (";
  Convert::convertDuration(previewStartTime, timeUnit); 
  std::cout << ")" << std::endl;
  std::cout << "--Preview Duration: " << previewDuration << " (";
  Convert::convertDuration(previewDuration, timeUnit);
  std::cout << ")" << std::endl;
  // QUICKTIME PREVIEW

  // QUICKTIME STILL POSTER
  uint32_t stillPosterTime = VideoFile::read32(blockPos);
  std::cout << "--Poster Time: " << stillPosterTime << " (";
  Convert::convertDuration(stillPosterTime, timeUnit);
  std::cout << ")" << std::endl;
  // QUICKTIME STILL POSTER

  // QUICKTIME SELECTION TIME
  uint32_t selectionStartTime = VideoFile::read32(blockPos);
  uint32_t selectionDuration = VideoFile::read32(blockPos);
  std::cout << "--Selection Time: " << selectionStartTime << " (";
  Convert::convertDuration(selectionStartTime, timeUnit); 
  std::cout << ")" << std::endl;
  std::cout << "--Selection Duration: " << selectionDuration << " (";
  Convert::convertDuration(selectionDuration, timeUnit);
  std::cout << ")" << std::endl;
  // QUICKTIME SELECTION TIME

  // QUICKTIME CURRENT TIME
  uint32_t currentTime = VideoFile::read32(blockPos);
  std::cout << "--Current Time: " << currentTime << " (";
  Convert::convertDuration(currentTime, timeUnit);
  std::cout << ")" << std::endl;
  // QUICKTIME CURRENT TIME

  // NEXT TRACK ID
  std::cout << "--Next Track ID: " << VideoFile::read32(blockPos) << std::endl;
  // NEXT TRACK ID

  assert(blockPos == blockSize);
}

// trak decode
void BlockDecode::trak::decode(uint64_t blockBase, uint64_t blockSize)
{
  // Current position within the box
  uint64_t blockPos = 8;

  while(blockPos < blockSize - 1){
    Block blockInfo = VideoFile::getBlock();
    std::cout << "--Block Size: " << blockInfo.blockSize << " bytes, Block Type: " << blockInfo.blockType << std::endl;

    // Branch into different blocks
    switch (BlockDecode::getBlockMapping(blockInfo.blockType))
    {
      case type_tkhd:
        BlockDecode::tkhd::decode(blockInfo.blockSize);
      default:
        break;
    }

    blockPos += blockInfo.blockSize;
    VideoFile::setFilePos(blockBase + blockPos);
  }
}

// tkhd decode
void BlockDecode::tkhd::decode(uint64_t blockSize)
{
  // Current position within the box
  uint64_t blockPos = 8;

  // If version is 1, date and duration values are 8 bytes long, otherwise 4.

  // VERSION
  std::vector<char> versionBuffer = VideoFile::readBytes(1, blockPos);
  uint32_t version = versionBuffer[0];

  std::cout << "---Version: " << version << std::endl;
  // VERSION

  // FLAGS
  std::cout << "---Flags: ";
  // Change output to hex
  std::cout << std::hex;
  for(auto c : VideoFile::readBytes(3, blockPos)){
    // https://stackoverflow.com/questions/673240/how-do-i-print-an-unsigned-char-as-hex-in-c-using-ostream
    if(c < 0x10){
      std::cout << 0;
    }
    std::cout << +c;
  }
  std::cout << std::endl;
  std::cout << std::dec;
  // FLAGS

  // CREATED MODIFIED TIME
  std::cout << "---Creation Time: ";
  version ? Convert::convertDate(VideoFile::read64(blockPos)) 
          : Convert::convertDate(VideoFile::read32(blockPos));
  std::cout << std::endl;
  std::cout << "---Modification Time: ";
  version ? Convert::convertDate(VideoFile::read64(blockPos)) 
          : Convert::convertDate(VideoFile::read32(blockPos));
  std::cout << std::endl;
  // CREATED MODIFIED TIME

  // TRACK ID
  std::cout << "---Track ID: " << VideoFile::read32(blockPos) << std::endl;
  // TRACK ID

  // RESERVED 4 BYTES
  std::cout << "---Reserved: " << VideoFile::read32(blockPos) << std::endl;
  // RESERVED 4 BYTES

  // DURATION
  uint64_t duration = version ? VideoFile::read64(blockPos) : VideoFile::read32(blockPos);
  std::cout << "---Duration: " << duration << " (";
  Convert::convertDuration(duration, timeUnit);
  std::cout << ")" << std::endl;
  // DURATION

  // RESERVED 8 BYTES
  std::cout << "---Reserved: ";
  std::cout << VideoFile::read32(blockPos) << " ";
  std::cout << VideoFile::read32(blockPos) << std::endl;
  // RESERVED 8 BYTES

  // VIDEO LAYER
  std::cout << "---Layer: " << VideoFile::read16(blockPos) << std::endl;
  // VIDEO LAYER

  // ALTERNATE TRACK ID
  std::cout << "---Alternate Group: " << VideoFile::read16(blockPos) << std::endl;
  // ALTERNATE TRACK ID

  // TRACK VOLUME
  std::cout << "---Volume: " << VideoFile::read16(blockPos) << std::endl;
  // TRACK VOLUME

  // RESERVED 2 BYTES
  std::cout << "---Reserved: " << VideoFile::read16(blockPos) << std::endl;
  // RESERVED 2 BYTES

  // TODO: According to Apple QUICKTIME documents, this is the correct format, but it seems like
  // the offset is incorrect.

  // TRANSFORMATION MATRIX
  std::cout << "---Matrix Structure:" << std::endl;
  std::cout << VideoFile::read32(blockPos) << " " << VideoFile::read32(blockPos) << " " << VideoFile::read32(blockPos) << std::endl;
  std::cout << VideoFile::read32(blockPos) << " " << VideoFile::read32(blockPos) << " " << VideoFile::read32(blockPos) << std::endl;
  std::cout << VideoFile::read32(blockPos) << " " << VideoFile::read32(blockPos) << " " << VideoFile::read32(blockPos) << std::endl;
  // TRANSFORMATION MATRIX

  // VIDEO FRAME SIZE
  std::cout << "---Track Width: " << VideoFile::read32(blockPos) << std::endl;
  std::cout << "---Track Height: " << VideoFile::read32(blockPos) << std::endl;
  // VIDEO FRAME SIZE

  assert(blockPos == blockSize);
}