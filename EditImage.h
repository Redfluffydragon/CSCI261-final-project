
#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <vector>
#include "Crop.h"
#include "upng.h" // For PNG decoding
#include "lodepng.h" // For PNG encoding

using namespace std;

#include <SFML/Graphics.hpp>

using namespace sf;

class EditImage
{
private:
  string filename; // The name of the file to open
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

  Text message; // For displaying info for the user
  Text rotationText; // For displaying the current rotation
  Text instructions; // For displaying instructions
  Font font; // For loading a font (Arial)

  void writePixel(unsigned int from, unsigned int to); // Write a pixel from the read vector to the write vector at the positions specified
  void rotate90(); // Rotate the image 90 degrees
  void updateRText(); // Update the rotation text
  void makeSprite(); // Create a sprite to display the image

  void calcRotate(); // Calculate the rotation and actually move pixels
  void calcFlip(); // Calculate the flipping and actually move pixels
  bool readFile(const string& filename); // Try to read a file into memory
  void GUISetUp(); // Set up the GUI for the image editing
public:
  explicit EditImage(const string &filename); // Parameterized constructor with file name
  EditImage(); // Default constructor (loads default image)
  void setFile(const string& filename); // Set a new filename to edit
  void draw(RenderWindow &window); // Draw the image and GUI in the window
  void rotate(const float &degrees); // Rotate the sprite and set the rotation data member of the image from where it is currently
  void setRotation(const float &degrees); // Rotate the sprite and set the rotation data member of the image from zero
  void flip(const string &); // Flip the sprite and mark for flipping later
  void crop(Crop newCrop);
  void reset(); // Reset the image and sprite to original
  bool save(string); // Save the new image to file (this is where it does the actual image processing)
};

#endif