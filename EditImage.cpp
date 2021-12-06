#include "EditImage.h" // Header file for this class
#include "Crop.h" // Custom crop struct
#include <string> // For strings
#include <fstream> // For reading a writing to files
#include <iostream> // For cin, cout
#include <vector> // For vectors
#include "math.h" // For pow
#include "upng.h" // For PNG decoding

using namespace std;

#include <SFML/Graphics.hpp>

using namespace sf;

EditImage::EditImage(const string &inputFilename) {
  filename = inputFilename;

  if (!EditImage::read()) {
    cout << "Failed to open file";
  }

}

EditImage::EditImage() {
  // getFilename();
  // filename = "data/wallpaper.ppm";
  filename = "data/pack_icon.png";
  
  if (!EditImage::read()) {
    cout << "Failed to open file";
  }
}

void EditImage::getFilename() {
  cout << "Please enter the name of the file you would like to open: ";
  cin >> filename; // ? Check for errors? use getline for file names with spaces?
}

bool EditImage::read() {
  // read the image

  // declare ifstream object and open the file
  ifstream imageFile(filename, ios_base::binary);
  
  // check for an error
  if ( imageFile.fail() ) {
    return false;
  }
  cout << "Opened file" << endl;

  EditImage::fileType = filename.substr(filename.find(".") + 1, filename.size() - 1);
  cout << fileType << endl;

  if (EditImage::fileType == "ppm") {
    string imageType; // To read the type of image from the image file
    int maxValue; // To read the other header data

    imageFile >> imageType;

    // Check if it's the right image type
    if (imageType != "P3") {
      cerr << "Sorry, that file type isn't supported.";
      return false;
    }

    // Read in the rest of the header
    imageFile >> EditImage::height >> EditImage::width >> maxValue;


    // Read in all the pixels
    int red, blue, green;
    for (int i = 0; i < EditImage::width; i++) {
      for (int j = 0; j < EditImage::height; j++) {
        imageFile >> red >> green >> blue;
        EditImage::image1.emplace_back(red, green, blue);
        /* EditImage::image1.push_back(green);
        EditImage::image1.push_back(blue);
        EditImage::image1.push_back(255); // Fake alpha channel */
      }
    }
  }
  else if (EditImage::fileType == "png") {
    // read the image

    // https://www.reddit.com/r/cpp_questions/comments/m93tjb/certain_bytes_are_just_skipped_while_reading/
    EditImage::buffer = vector<uint8_t>((istreambuf_iterator<char>(imageFile)), istreambuf_iterator<char>());

    // Create a pointer to the byte array behind the buffer - https://stackoverflow.com/questions/2923272/how-to-convert-vector-to-array
    unsigned char* tempCharArray = &EditImage::buffer[0];

    // Create a upng instance and pass the byte array to the upng instance
    upng_t* upng = upng_new_from_bytes(tempCharArray, (unsigned long)buffer.size());

    upng_decode(upng);

    EditImage::width = upng_get_width(upng); // Returns width of image in pixels
    EditImage::height = upng_get_height(upng); // Returns height of image in pixels

    const unsigned char* getBuffer = upng_get_buffer(upng);

    // EditImage::image1 = vector<unsigned char>(getBuffer);

  }

  // close the file
  imageFile.close();

  // Now make a sprite to display the image
  EditImage::makeSprite();

  return true;
}

// Convert a hex number spread over four bytes into an unsigned int
unsigned int EditImage::fourBytesInt(unsigned int start) {
  unsigned int sum = 0;
  for (int i = start; i < start + 4; i++) {
    sum += (int)EditImage::buffer.at(i) * pow(16, (6 - 2 * (i - start)));
  }

  return sum;
}

string EditImage::fourBytesString(unsigned int start) {
  string stringSum = "";
  for (int i = start; i < start + 4; i++) {
    stringSum += EditImage::buffer.at(i);
  }
  return stringSum;
}

PNGChunk EditImage::findChunk(unsigned int start) {
  // Read in size of chunk
  unsigned int size = fourBytesInt(start);

  // Read in type of chunk
  string chunkType = fourBytesString(start + 4);

  string crc = fourBytesString(start + 8 + size);

  return PNGChunk{chunkType, size, start + 8, start + 8 + size, crc};
}

void EditImage::makeSprite() {

  EditImage::sprite = Sprite();

  /* EditImage::sprite.setOrigin(EditImage::width / 2, EditImage::height / 2);
  EditImage::sprite.setPosition(320, 150); */

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

void EditImage::draw(RenderWindow &window) {
  window.draw(EditImage::sprite);
}

void EditImage::rotate(const double &degrees)  {

}

void EditImage::flip(const string &direction) {

}

void EditImage::crop(Crop newCrop) {

}

bool EditImage::save(string newFilename) {
  if (newFilename.empty()) {
    newFilename = filename + "_edited";
  }

  ofstream newImage(filename);
  
  // check for an error
  if ( newImage.fail() ) {
    cerr << "Error opening output file";
    return false;
  }
  
  newImage << "data out";
  
  // close the file
  newImage.close();

  return true;
}
