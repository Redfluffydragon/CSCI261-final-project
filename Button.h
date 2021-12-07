//
// Created by kderu on 12/6/2021.
//

#ifndef FINAL_PROJECT_BUTTON_H
#define FINAL_PROJECT_BUTTON_H

using namespace std;

#include <SFML/Graphics.hpp>
using namespace sf;

class Button {
private:
  Vector2f size;
  Vector2f position;
  RectangleShape rect;
  Text text;
  Font font;
  Color color;
  int padding = 20;
public:
  Button();
  Button(string text, Vector2f position, Color color);

  void draw(RenderWindow &window);
  bool isClicked(Vector2i clickPosition);
  
};


#endif //FINAL_PROJECT_BUTTON_H
