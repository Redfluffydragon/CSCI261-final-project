#include "EditImage.h" // Header file for this class
#include "Crop.h" // Custom crop struct
#include <string> // For strings
#include <fstream> // For reading a writing to files
#include <iostream> // For cin, cout
#include <vector> // For vectors
#include "math.h" // For pow
#include "upng.h" // For PNG decoding

using namespace std;

#include <SFML/Graphics.hpp> // Include SFML Graphics library

using namespace sf;

// Parameterized constructor that takes a file name
EditImage::EditImage(const string &inputFilename) {
  // Set the internal file name to the input file name
  EditImage::filename = inputFilename;

  // Try to read the file (readFile does error handling) with EditImage::message
  EditImage::readFile(EditImage::filename);

  // Set up the GUI elements
  EditImage::GUISetUp();
}

// Default constructor that loads a default image
EditImage::EditImage() {
  // Default file
  EditImage::filename = "data/tearTreeSmall.png";
  
  // Try to read the file (readFile does error handling) with EditImage::message
  EditImage::readFile(EditImage::filename);

  // Set up the GUI elements
  EditImage::GUISetUp();
}

// Set up GUI elements
void EditImage::GUISetUp() {
  EditImage::font.loadFromFile("data/arial.ttf");

  EditImage::updateRText();
  EditImage::rotationText.setFillColor(Color::Black);
  EditImage::rotationText.setFont(EditImage::font);

  EditImage::message.setFillColor(Color::Black);
  EditImage::message.setFont(EditImage::font);
  EditImage::message.setPosition(Vector2f(10, 350));

  EditImage::instructions.setString(
    "Press V or H to flip the image horizontally or vertically.\nClick the buttons to rotate the image.\nPress S to save the image.\nOnly supports PNG images.");
  EditImage::instructions.setFillColor(Color::Black);
  EditImage::instructions.setFont(EditImage::font);
  EditImage::instructions.setPosition(Vector2f(50, 630));
}

// Change to a new image file
void EditImage::setFile(const string& filename) {
  EditImage::readFile(filename);
}

// Write a pixel (four bytes/chars) from the location in read to the location in write
void EditImage::writePixel(unsigned int from, unsigned int to) {
  EditImage::write.at(to) = EditImage::read.at(from);
  EditImage::write.at(to + 1) = EditImage::read.at(from + 1);
  EditImage::write.at(to + 2) = EditImage::read.at(from + 2);
  EditImage::write.at(to + 3) = EditImage::read.at(from + 3);
}

// Read an image from file into a vector
bool EditImage::readFile(const string& filename) {
  // declare ifstream object and open the file in binary read mode so it can read all the image data correctly
  ifstream imageFile(EditImage::filename, ios_base::binary);
  
  // check for an error
  if ( imageFile.fail() ) {
    EditImage::message.setString("Could not load new file. The previous file is still loaded.");
    return false;
  }

  EditImage::fileType = EditImage::filename.substr(EditImage::filename.find(".") + 1, EditImage::filename.size() - 1);
  cout << "Opened " << EditImage::fileType << " file" << endl;

  if (EditImage::fileType == "png") {
    // read the image

    // https://www.reddit.com/r/cpp_questions/comments/m93tjb/certain_bytes_are_just_skipped_while_reading/
    EditImage::buffer = vector<unsigned char>((istreambuf_iterator<char>(imageFile)), istreambuf_iterator<char>());

    // Create a pointer to the byte array behind the buffer vector - https://stackoverflow.com/questions/2923272/how-to-convert-vector-to-array
    unsigned char* tempCharArray = &EditImage::buffer[0];

    // Create a upng instance and pass the byte array to the upng instance
    upng_t* upng = upng_new_from_bytes(tempCharArray, (unsigned long)buffer.size());

    // Decode the image (decompress it)
    upng_decode(upng);

    // EditImage::header = upng_header(upng); // Get the image header
    EditImage::originalWidth = upng_get_width(upng); // Returns width of image in pixels
    EditImage::width = upng_get_width(upng); // Returns width of image in pixels

    EditImage::originalHeight = upng_get_height(upng); // Returns height of image in pixels
    EditImage::height = upng_get_height(upng); // Returns height of image in pixels

    // Temporary buffer
    const unsigned char* getBuffer = upng_get_buffer(upng);

    EditImage::size = upng_get_size(upng);

    // Transfer into a vector so that way it can be a member of the object
    // https://stackoverflow.com/questions/259297/how-do-you-copy-the-contents-of-an-array-to-a-stdvector-in-c-without-looping
    EditImage::original.insert(EditImage::original.end(), &getBuffer[0], &getBuffer[EditImage::size]);


    // Also transfer into the read and write vectors so they're the right size
    EditImage::write.insert(EditImage::write.end(), &getBuffer[0], &getBuffer[EditImage::size]);
    EditImage::read.insert(EditImage::read.end(), &getBuffer[0], &getBuffer[EditImage::size]);

    // Now that it's in a vector we don't need the char array anymore, so free it from memory
    upng_free(upng);

    // Now make a sprite to display the image
    EditImage::makeSprite();
  }
  else {
    EditImage::message.setString("That file type is not supported. The previous file is still loaded.");
    return false;
  }

  // close the file
  imageFile.close();

  // Only set the object filename once the read has succeeded
  EditImage::filename = filename;
  return true;
}

// Update the displayed rotation value
void EditImage::updateRText() {
  EditImage::rotationText.setString("Rotation: " + to_string((int)EditImage::rotation) + char(176));
}

// Make a sprite to display the image
void EditImage::makeSprite() {

  EditImage::sprite = Sprite();

  EditImage::sprite.setOrigin(EditImage::width / 2, EditImage::height / 2);
  EditImage::sprite.setPosition(420, 150);

  if (EditImage::fileType == "ppm") {
    Image tempImage;
    tempImage.create(EditImage::width, EditImage::height);

    /* for (int i = 0; i < EditImage::width; i++) {
      for (int j = 0; j < EditImage::height; j++) {
        tempImage.setPixel(i, j, EditImage::image1.at((i * EditImage::width) + j));
      }
    } */

    // Create texture
    EditImage::texture = Texture();
    EditImage::texture.create(EditImage::width, EditImage::height);

    EditImage::texture.loadFromImage(tempImage);
  }
  else {
    EditImage::texture.loadFromFile(EditImage::filename);
  }


  EditImage::sprite.setTexture(EditImage::texture);

}

// Reset the sprite and internal values to original
void EditImage::reset() {
  EditImage::sprite.setScale(Vector2f(1, 1));
  EditImage::sprite.setRotation(0);
  EditImage::rotation = 0;
  EditImage::flipState[0] = 1;
  EditImage::flipState[1] = 1;
  EditImage::message.setString("Reset!");
}

// Draw the image sprite and stuff in the window
void EditImage::draw(RenderWindow &window) {
  window.draw(EditImage::sprite);
  window.draw(EditImage::rotationText);
  window.draw(EditImage::message);
  window.draw(EditImage::instructions);
}

// Rotate the image relative to where it is currently
void EditImage::rotate(const float &degrees)  {
  EditImage::sprite.rotate(degrees);
  EditImage::rotation = EditImage::sprite.getRotation();
  EditImage::updateRText();
}

// Set the rotation of the image relative to zero degrees
void EditImage::setRotation(const float &degrees) {
  EditImage::sprite.setRotation(degrees);
  EditImage::rotation = EditImage::sprite.getRotation();
  EditImage::updateRText();
}

// Actually move pixels around to rotate the image 90 degrees
void EditImage::rotate90() {
  cout << "Rotate90" << endl;

  for (unsigned int y = 0; y < EditImage::height; y++) {
    for (unsigned int x = 0; x < EditImage::width; x++) {
      // THIS WORKED THE FIRST TRY!!!!!!!!!!!!!!!! (On a square image)
      EditImage::writePixel(
        4 * (y * EditImage::width + x),
        4 * (EditImage::height - y - 1 + EditImage::height * x)
      );
    }
  }

  // Swap the width and height
  unsigned int temp = EditImage::height;
  EditImage::height = EditImage::width;
  EditImage::width = temp;
  
  // Set the read vector equal to the write vector so we can work on the newly modified image
  EditImage::read = EditImage::write;
}

// Move pixels and calculate the rotation of the image
void EditImage::calcRotate() {
  // Rotate 90 degrees the correct amount of times
  if ((int)EditImage::rotation % 90 == 0) {
    for (int i = 0; i < (int)EditImage::rotation / 90; i++) {
      EditImage::rotate90();
    }
  }

  // Set the read vector equal to the write vector so we can work on the newly modified image
  EditImage::read = EditImage::write;
}

// Flip the image vertically or horizontally
void EditImage::flip(const string &direction) {
  // Mark as flipped horizontally
  if (direction == "h") {
    EditImage::flipState[0] = EditImage::flipState[0] == 1 ? -1 : 1;
  }
  // Mark as flipped vertically
  else if (direction == "v") {
    EditImage::flipState[1] = EditImage::flipState[1] == 1 ? -1 : 1;
  }

  // Use scale to mirror the displayed image appropriately
  EditImage::sprite.setScale(EditImage::flipState[0], EditImage::flipState[1]);
}

// Actually do the flipping based on flipstate
void EditImage::calcFlip() {
  if (EditImage::flipState[0] == -1) {
    for (unsigned int y = 0; y < EditImage::height; y++) {
      for (unsigned int x = 0; x < EditImage::width; x++) {
        EditImage::writePixel(4 * (y * EditImage::width + x), 4 * (y * EditImage::width + (EditImage::width - x - 1)));
      }
    }
    // Set the read vector equal to the write vector so we can work on the newly modified image
    EditImage::read = EditImage::write;
  }
  if (EditImage::flipState[1] == -1) {
    for (unsigned int y = 0; y < EditImage::height; y++) {
      for (unsigned int x = 0; x < EditImage::width; x++) {
        EditImage::writePixel(4 * (y * EditImage::width + x), 4 * ((EditImage::height - y - 1) * EditImage::width + x));
      }
    }
    // Set the read vector equal to the write vector so we can work on the newly modified image
    EditImage::read = EditImage::write;
  }
}

// Crop the image
void EditImage::crop(Crop newCrop) {

  // Set the read vector equal to the write vector so we can work on the newly modified image
  EditImage::read = EditImage::write;
}

// Save the image to file
bool EditImage::save(string newFilename) {
  EditImage::message.setString("Saving..."); // Can't see it with small images, might be able to with larger images

  //Only actually move the pixels when saving

  // Reset everything to the original image (These only change when you save anyway)
  EditImage::read = EditImage::original;
  EditImage::write = EditImage::original;
  EditImage::width = EditImage::originalWidth;
  EditImage::height = EditImage::originalHeight;

  // Then do transforms
  EditImage::calcFlip();
  EditImage::calcRotate();

  // If the file name is empty, set it to the original file name + "_edited"
  if (newFilename.empty()) {
    int lastDot = EditImage::filename.find_last_of(".");
    newFilename = EditImage::filename.substr(0, lastDot) + "_edited" + EditImage::filename.substr(lastDot, EditImage::filename.size() - lastDot);
  }

  // fopen needs a char array instead of a C++ string
  const char* newFilePointer = &newFilename[0];

  // https://www.cplusplus.com/reference/cstdio/fopen/
  // wb = write binary
  EditImage::newImage = fopen(newFilePointer, "wb");

  
  // check for an error
  if ( !EditImage::newImage ) {
    cerr << "Error opening output file";
    return false;
  }

  // Clear the buffer
  EditImage::buffer.clear();

  // Encode the image into the buffer
  lodepng::encode(EditImage::buffer, EditImage::read, EditImage::width, EditImage::height);

  // Write the image to file
  // 1 is the size of one char
  int written = fwrite(&EditImage::buffer[0], 1, EditImage::buffer.size(), EditImage::newImage);
  if (written == EditImage::buffer.size()) {
    EditImage::message.setString("Saved to " + newFilename);
  }
  else {
    EditImage::message.setString("Saved incorrectly.");
    cerr << "Fail. only wrote " << written << " elements out of " << EditImage::buffer.size();
  }

  fclose(EditImage::newImage);
  cout << "Saved!" << endl;

  return true;
}
