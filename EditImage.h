
#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <vector>
#include "Crop.h"
#include "PNGChunk.h"
#include "upng.h" // For PNG decoding

using namespace std;

#include <SFML/Graphics.hpp>

using namespace sf;

class EditImage
{
private:
  string filename; // The name of the file to open
  string fileType; //  The image file extension

  int height; // The height in pixels
  int width; // The width in pixels
  double rotation; // The current rotation
  Crop currentCrop; // The current crop

  vector<uint8_t> buffer;
  vector<unsigned char> image1; // A vector holding a version of the current image
  vector<unsigned char> image2; // A vector holding the other version of the current image

  Sprite sprite; // For displaying the image
  Texture texture; // For displaying the image

  PNGChunk findChunk(unsigned int);

  unsigned int fourBytesInt(unsigned int);
  string fourBytesString(unsigned int);
public:
  EditImage(const string &);
  EditImage();
  void getFilename(); // Get a filename to open
  bool read();
  void makeSprite();
  void draw(RenderWindow &window);
  void rotate(const double &);
  void flip(const string &);
  void crop(Crop newCrop);
  bool save(string);
};

#endif