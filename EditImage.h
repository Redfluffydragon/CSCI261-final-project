
#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <vector>
#include "Crop.h"
#include "PNGChunk.h"
#include "upng.h" // For PNG decoding
#include "lodepng.h" // For PNG encoding

using namespace std;

#include <SFML/Graphics.hpp>

using namespace sf;

class EditImage
{
private:
  string filename; // The name of the file to open
  vector<unsigned char> header; // The file header
  string fileType; //  The image file extension

  unsigned int height; // The height in pixels
  unsigned int width; // The width in pixels
  unsigned int size; // The size of the pixel array

  double rotation = 0.0; // The current rotation
  Crop currentCrop; // The current crop
  int flipState[2] = {1, 1}; // The current horizontal and vertical flip

  vector<unsigned char> buffer; // To get the image data to pass to upng

  unsigned int originalHeight;
  unsigned int originalWidth;
  vector<unsigned char> original; // A vector that always holds the original image
  vector<unsigned char> read; // A vector holding a version of the current image to read from
  vector<unsigned char> write; // A vector to write the edited image to 

  FILE* newImage;

  Sprite sprite; // For displaying the image
  Texture texture; // For displaying the image

  Text message;
  Text rotationText;
  Text instructions;
  Font font;

  void writePixel(unsigned int from, unsigned int to);
  void rotate90();
  void updateRText();
  void makeSprite();

  void calcRotate();
  void calcFlip();
public:
  EditImage(const string &);
  EditImage();
  void getFilename(); // Get a filename to open
  bool readFile();
  void draw(RenderWindow &window);
  void rotate(const float &degrees);
  void setRotation(const float &degrees);
  void flip(const string &);
  void crop(Crop newCrop);
  bool save(string);
};

#endif