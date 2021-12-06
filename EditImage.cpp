#include "EditImage.h"
#include "Crop.h"
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

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
  ifstream imageFile(filename, std::ios::binary);
  
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
        if (i == 0) {
          cout << red << " " << green << " " << blue << " " << endl;
        }
        EditImage::image1.emplace_back(red, green, blue);
        /* EditImage::image1.push_back(green);
        EditImage::image1.push_back(blue);
        EditImage::image1.push_back(255); // Fake alpha channel */
      }
    }
  }
  else if (EditImage::fileType == "png") {
      // read the image

      // Read in header
      for (int i = 0; i < 8; i++) {
        if (imageFile >> EditImage::header[i]) {
          cout << +EditImage::header[i] << " ";
        }
      }

  }

  // close the file
  imageFile.close();

  // Now make a sprite to display the image
  EditImage::makeSprite();

  return true;
}

void EditImage::makeSprite() {

  EditImage::sprite = Sprite();

  /* EditImage::sprite.setOrigin(EditImage::width / 2, EditImage::height / 2);
  EditImage::sprite.setPosition(320, 150); */

  if (EditImage::fileType == "ppm") {
    Image tempImage;
    tempImage.create(EditImage::width, EditImage::height);

    for (int i = 0; i < EditImage::width; i++) {
      for (int j = 0; j < EditImage::height; j++) {
        tempImage.setPixel(i, j, EditImage::image1.at((i * EditImage::width) + j));
      }
    }

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