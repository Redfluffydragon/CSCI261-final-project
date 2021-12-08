#include "EditImage.h" // Header file for this class
#include <string> // For strings
#include <fstream> // For reading a writing to files
#include <iostream> // For cin, cout
#include <vector> // For vectors
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
  EditImage::filename = "default_image.png";
  
  // Try to read the file (readFile does error handling) with EditImage::message
  EditImage::readFile(EditImage::filename);

  // Set up the GUI elements
  EditImage::GUISetUp();
}

// Set up GUI elements
void EditImage::GUISetUp() {
  // Load Arial from file
  EditImage::font.loadFromFile("data/arial.ttf");

  // Set up the rotation text
  EditImage::updateStateText();
  EditImage::rotationText.setFillColor(Color::Black);
  EditImage::rotationText.setFont(EditImage::font);

  // Set up the message text
  EditImage::message.setFillColor(Color::Black);
  EditImage::message.setFont(EditImage::font);
  EditImage::message.setPosition(Vector2f(10, 350));

  // Set up the instructions
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
  ifstream imageFile(filename, ios_base::binary);
  
  // check for an error
  if ( imageFile.fail() ) {
    EditImage::message.setString("Could not load new file. The previous file is still loaded.");
    return false;
  }

  // Find the file extension of the file
  // Get a 3-character substr because the largest file it cares about should only be 3 characters long
  EditImage::fileType = filename.substr(filename.find('.') + 1, 3);

  // only support files that end in .png or .PNG
  if (EditImage::fileType == "png" || EditImage::fileType == "PNG") {
    // read the image

    // Use an istreambuf_iterator to get read all the image data correctly
    // !https://www.reddit.com/r/cpp_questions/comments/m93tjb/certain_bytes_are_just_skipped_while_reading/
    EditImage::buffer = vector<unsigned char>((istreambuf_iterator<char>(imageFile)), istreambuf_iterator<char>());

    // Create a pointer to the byte array behind the buffer vector 
    // !https://stackoverflow.com/questions/2923272/how-to-convert-vector-to-array
    unsigned char* tempCharArray = &EditImage::buffer[0];

    // Create a upng instance and pass the byte array to said instance
    upng_t* upng = upng_new_from_bytes(tempCharArray, (unsigned long)buffer.size());

    // Decode the image (decompress it) with the upng library
    upng_decode(upng);

    // Get the dimensions of the image in pixels
    EditImage::originalWidth = upng_get_width(upng);
    EditImage::width = upng_get_width(upng);

    EditImage::originalHeight = upng_get_height(upng);
    EditImage::height = upng_get_height(upng);

    // Get the image data in a temporary buffer
    const unsigned char* getBuffer = upng_get_buffer(upng);

    //Get the size of the image data in bytes
    EditImage::size = upng_get_size(upng);

    // Clear all vectors (in case of loading a new file)
    EditImage::original.clear();
    EditImage::write.clear();
    EditImage::read.clear();

    // Transfer into a vector so it can be a member of the object
    // !https://stackoverflow.com/questions/259297/how-do-you-copy-the-contents-of-an-array-to-a-stdvector-in-c-without-looping
    EditImage::original.insert(EditImage::original.end(), &getBuffer[0], &getBuffer[EditImage::size]);

    // Also transfer into the read and write vectors so they're the right size
    EditImage::write.insert(EditImage::write.end(), &getBuffer[0], &getBuffer[EditImage::size]);
    EditImage::read.insert(EditImage::read.end(), &getBuffer[0], &getBuffer[EditImage::size]);

    // Now that it's in a vector it don't need the char array anymore, so free it from memory
    upng_free(upng);

    // Only set the object filename once the read has succeeded
    EditImage::filename = filename;

    // Make a sprite to display the image
    EditImage::makeSprite();

    // Tell the user
    EditImage::message.setString("Loaded " + filename);
  }
  else {
    EditImage::message.setString("That file type is not supported. The previous file is still loaded.");
    return false;
  }

  // close the file
  imageFile.close();

  return true;
}

// Update the displayed rotation value
void EditImage::updateStateText() {
  EditImage::rotationText.setString(
    "Rotation: " + to_string((int)EditImage::rotation) + char(176) + 
    (EditImage::flipState[0] == -1 ? ", flipped horizontally" : "") + 
    (EditImage::flipState[1] == -1 ? ", flipped vertically" : "")
  );
}

// Make a sprite to display the image
void EditImage::makeSprite() {
  // Make a new sprite (Clear any old stuff out)
  EditImage::sprite = Sprite();

  // Set origin to the center for rotation
  EditImage::sprite.setOrigin(EditImage::width / 2, EditImage::height / 2);

  // Put it in the middle near the top of the window
  EditImage::sprite.setPosition(420, 150);

  // Load the image file as a texture
  EditImage::texture.loadFromFile(EditImage::filename);

  // Set the texture for the sprite
  EditImage::sprite.setTexture(EditImage::texture);
}

// Reset the sprite and internal values to original
void EditImage::reset() {
  // Reset sprite
  EditImage::sprite.setScale(Vector2f(1, 1));
  EditImage::sprite.setRotation(0);

  // Reset internal values
  EditImage::rotation = 0;
  EditImage::flipState[0] = 1;
  EditImage::flipState[1] = 1;

  // Tell the user that it reset
  EditImage::message.setString("Reset!");
  EditImage::updateStateText();
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
  EditImage::sprite.rotate(degrees); // Rotate the sprite
  EditImage::rotation = EditImage::sprite.getRotation(); // Get the new absolute rotation
  EditImage::updateStateText(); // Update the displayed rotation value
}

// Set the rotation of the image relative to zero degrees
void EditImage::setRotation(const float &degrees) {
  EditImage::sprite.setRotation(degrees); // Rotate the sprite
  EditImage::rotation = EditImage::sprite.getRotation(); // Get the new absolute rotation
  EditImage::updateStateText(); // Update the displayed rotation value
  EditImage::message.setString("Rotated to " + to_string((int)EditImage::rotation) + " degrees!");
}

// Actually move pixels around to rotate the image 90 degrees
void EditImage::rotate90() {
  // Loop through the image vector
  for (unsigned int y = 0; y < EditImage::height; y++) {
    for (unsigned int x = 0; x < EditImage::width; x++) {
      EditImage::writePixel(
        4 * (y * EditImage::width + x), // Original position
        4 * (EditImage::height - y - 1 + EditImage::height * x) // New position
      );
    }
  }

  // Swap the width and height
  unsigned int temp = EditImage::height;
  EditImage::height = EditImage::width;
  EditImage::width = temp;
  
  // Set the read vector equal to the write vector so it can work on the newly modified image
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

  // Set the read vector equal to the write vector so it can work on the newly modified image
  EditImage::read = EditImage::write;
}

// Flip the image vertically or horizontally
void EditImage::flip(const string &direction) {
  // Mark as flipped horizontally
  if (direction == "h") {
    EditImage::flipState[0] = EditImage::flipState[0] == 1 ? -1 : 1;
    // Tell the user
    EditImage::message.setString("Flipped horizontally!");
  }
  // Mark as flipped vertically
  else if (direction == "v") {
    EditImage::flipState[1] = EditImage::flipState[1] == 1 ? -1 : 1;
    // Tell the user
    EditImage::message.setString("Flipped vertically!");
  }

  // Display the changes in text
  EditImage::updateStateText();

  // Use scale to mirror the sprite appropriately
  EditImage::sprite.setScale(EditImage::flipState[0], EditImage::flipState[1]);
}

// Actually do the flipping based on flipstate
void EditImage::calcFlip() {
  // Flip horizontally
  if (EditImage::flipState[0] == -1) {
    // Loop through the image vector
    for (unsigned int y = 0; y < EditImage::height; y++) {
      for (unsigned int x = 0; x < EditImage::width; x++) {
        EditImage::writePixel(
          4 * (y * EditImage::width + x), // Old position
          4 * (y * EditImage::width + (EditImage::width - x - 1)) // New position - reverse x
        );
      }
    }
    // Set the read vector equal to the write vector so it can work on the newly modified image
    EditImage::read = EditImage::write;
  }

  // Flip vertically
  if (EditImage::flipState[1] == -1) {
    for (unsigned int y = 0; y < EditImage::height; y++) {
      for (unsigned int x = 0; x < EditImage::width; x++) {
        EditImage::writePixel(
          4 * (y * EditImage::width + x), // Old position
          4 * ((EditImage::height - y - 1) * EditImage::width + x) // New position - reverse y
        );
      }
    }
    // Set the read vector equal to the write vector so it can work on the newly modified image
    EditImage::read = EditImage::write;
  }
}

// Save the image to file
bool EditImage::save(string newFilename) {
  EditImage::message.setString("Saving..."); // Can't see it with small images, might be able to with larger images

  // Reset everything to the original image (These only change when you save anyway)
  EditImage::read = EditImage::original;
  EditImage::write = EditImage::original;
  EditImage::width = EditImage::originalWidth;
  EditImage::height = EditImage::originalHeight;

  // Then do transforms - only actually move the pixels when saving
  EditImage::calcFlip();
  EditImage::calcRotate();

  // If the file name is empty, set it to the original file name + "_edited"
  if (newFilename.empty()) {
    unsigned long long lastDot = EditImage::filename.find_last_of('.');
    newFilename = EditImage::filename.substr(0, lastDot) + "_edited" + EditImage::filename.substr(lastDot, EditImage::filename.size() - lastDot);
  }

  // fopen needs a char array instead of a C++ string
  const char* newFilePointer = &newFilename[0];

  // wb = write binary
  EditImage::newImage = fopen(newFilePointer, "wb");

  
  // check for an error
  if ( !EditImage::newImage ) {
    EditImage::message.setString("Error opening output file.");
    return false;
  }

  // Clear the buffer
  EditImage::buffer.clear();

  // Encode the image into the buffer with lodepng
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

  // Close the file
  fclose(EditImage::newImage);

  return true;
}
