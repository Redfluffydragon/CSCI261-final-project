//
// Created by kderu on 12/6/2021.
//

#include <iostream> // For cin, cout
#include "Button.h"

using namespace std;

Button::Button() {
  Button::font.loadFromFile("data/arial.ttf");

}

Button::Button(string text, Vector2f position, Color color) {
    Button::position = position;

    Button::font.loadFromFile("data/arial.ttf");

    Button::text.setFont(Button::font);
    Button::text.setFillColor(Color::Black);
    Button::text.setString(text);
    Button::text.setPosition(Vector2f(Button::position.x + Button::padding, Button::position.y + Button::padding));

    FloatRect textSize = Button::text.getGlobalBounds();
    Button::size = Vector2f(textSize.width + Button::padding * 2, textSize.height + Button::padding * 2);

    Button::rect.setSize(Button::size);
    Button::rect.setPosition(Button::position);
    Button::rect.setFillColor(color);
}

void Button::draw(RenderWindow &window) {
    window.draw(Button::rect);
    window.draw(Button::text);
}

bool Button::isClicked(Vector2i clickPosition) {
    return
        clickPosition.x >= Button::position.x &&
        clickPosition.x <= Button::position.x + Button::size.x &&
        clickPosition.y >= Button::position.y &&
        clickPosition.y <= Button::position.y + Button::size.y;
}
