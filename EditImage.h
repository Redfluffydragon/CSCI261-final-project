
#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <vector>
#include "Crop.h"
#include "PNGChunk.h"

using namespace std;

#include <SFML/Graphics.hpp>

using namespace sf;

class EditImage
{
private:
  string fileType;
  uint8_t header[8];
  string filename;
  string format;
  int height; // The height in pixels
  int width; // The width in pixels
  double rotation; // The current rotation
  Crop currentCrop; // The current crop
  vector<uint8_t> buffer;
  vector<Color> image1; // A 2d vector holding a version of the current image
  vector<vector <Color>> image2; // A vector holding the other version of the current image
  Sprite sprite; // For displaying the image
  Texture texture; // For displaying the image

  PNGChunk findChunk(int);

  int fourBytesInt(int);
  string fourBytesString(int);
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