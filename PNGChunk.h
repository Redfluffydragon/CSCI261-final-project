#ifndef __PNGCHUNK_H__
#define __PNGCHUNK_H__


#include <string>

using namespace std;

struct PNGChunk
{
  string type;
  int length;
  int start;
  int end;
  string crc;
};

#endif // __PNGCHUNK_H__