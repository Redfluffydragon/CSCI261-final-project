//
// Created by kderu on 12/6/2021.
//

#include "Button.h"

using namespace std;

// Default constructor gives a small square black button with no text, located at 0, 0
Button::Button() {
    Button::size = Vector2f(50, 50);
    Button::rect.setSize(Button::size);
    Button::rect.setFillColor(Color::Black);
}

// Makes a button that displays the given text, with approximately Button::padding space around the text in the rectangle
Button::Button(string text, Vector2f position, Color color) {
    Button::position = position; // Set position

    Button::font.loadFromFile("data/arial.ttf"); // Load Arial from file

    Button::text = Text(text, Button::font); // Create a new text object with font and text
    Button::text.setFillColor(Color::Black); // Set fill to black
    // Set the text position relative to the rect
    Button::text.setPosition(Vector2f(Button::position.x + Button::padding - 5, Button::position.y + Button::padding - 5));

    FloatRect textSize = Button::text.getGlobalBounds(); // Get text size
    // Set button size with padding around the text
    Button::size = Vector2f(textSize.width + Button::padding * 2, textSize.height + Button::padding * 2);

    // Set the rect size, position, and color
    Button::rect.setSize(Button::size);
    Button::rect.setPosition(Button::position);
    Button::rect.setFillColor(color);
}

// Draw the rect and text in the window
void Button::draw(RenderWindow &window) {
    window.draw(Button::rect);
    window.draw(Button::text);
}

// Check if the given coordinates are inside the button rect
bool Button::isWithin(Vector2i mousePosition) {
    return
        mousePosition.x >= Button::position.x &&
        mousePosition.x <= Button::position.x + Button::size.x &&
        mousePosition.y >= Button::position.y &&
        mousePosition.y <= Button::position.y + Button::size.y;
}

const Color &Button::getColor() const {
    return color;
}

void Button::setColor(const Color &color) {
    Button::color = color;
}

// Get the text string
string Button::getText() {
    return Button::text.getString();
}

// Set the text string
void Button::setText(const string &text) {
    Button::text.setString(text);
}
