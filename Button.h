//
// Created by kderu on 12/6/2021.
//

#ifndef FINAL_PROJECT_BUTTON_H
#define FINAL_PROJECT_BUTTON_H

#include <string>
using namespace std;

#include <SFML/Graphics.hpp>
using namespace sf;

class Button {
private:
  Vector2f size; // Width and height of the rectangle. Calculated from the text size
  Vector2f position; // Position of the rectangle (top left corner)
  RectangleShape rect; // The rectangle to display
  Text text; // Button text object
  Font font; // Font - only does Arial
  Color color; // Color of the rectangle - text is always black
  int padding = 20; // Padding around the text

public:
  string getText();

  void setText(const string &text);

  const Color &getColor() const;

  void setColor(const Color &color);

  Button();
  Button(string text, Vector2f position, Color color);

  void draw(RenderWindow &window); // Draw the button in the window
  bool isWithin(Vector2i mousePosition); // Check if the coordinates are inside the button
  
};


#endif //FINAL_PROJECT_BUTTON_H
