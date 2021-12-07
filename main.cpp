/* CSCI 261 FINAL PROJECT
 *
 * Author: KAI-SHEN DERU
 *
 * !More complete description here...
 */

// The include section adds extra definitions from the C++ standard library.
#include <iostream> // For cin, cout, etc.
#include "EditImage.h" // For the image to edit
#include "Button.h" // For buttons

// We will (most of the time) use the standard library namespace in our programs.
using namespace std;

#include <SFML/Graphics.hpp>
using  namespace sf;

// Define any constants below this comment.
const int WINDOW_SIZE = 640;

int main() {

    EditImage image; // Automatically gets file name and reads in the file and makes a sprite

    // Whether or not the R key is pressed, for rotating the image
    bool rDown = false;

    // The starting point of the mouse to calculate rotation
    Vector2i startingPoint;

    Button rButtons[4]{
      Button("0 deg", Vector2f(0, 40), Color::Cyan),
      Button("90 deg", Vector2f(0, 110), Color::Cyan),
      Button("180 deg", Vector2f(0, 180), Color::Cyan),
      Button("270 deg", Vector2f(0, 250), Color::Cyan),
    };

    RenderWindow window( VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Image editor" );

    Cursor cursor;
    cursor.loadFromSystem(Cursor::Hand);
    bool overButton = false;

    // while our window is open, keep it open
    // this is our draw loop
    while( window.isOpen() ) {
        window.clear( Color::White );           // clear the contents of the old frame
                                                // by setting the window to black

        //****************************************
        //  ADD ALL OF OUR DRAWING BELOW HERE
        //****************************************

        image.draw(window);

        for (int i = 0; i < 4; i++) {
          rButtons[i].draw(window);
        }

        if (overButton) {
          window.setMouseCursor(cursor);
        }

        //****************************************
        //  ADD ALL OF OUR DRAWING ABOVE HERE
        //****************************************

        window.display();                       // display the window

        //****************************************
        // HANDLE EVENTS BELOW HERE
        //****************************************
        Event event;
        while( window.pollEvent(event) ) {      // ask the window if any events occurred
            if( event.type == Event::Closed ) { // if event type is a closed event
                // i.e. they clicked the X on the window
                window.close();                 // then close our window
            }
            else if (event.type == Event::Resized) { // Make it so things don't stretch when the window is resized
              FloatRect visibleArea(0, 0, event.size.width, event.size.height);
              window.setView(View(visibleArea));
            }
            else if ( event.type == Event::MouseButtonPressed ) {

            }
            else if ( event.type == Event::MouseMoved && rDown ) {
              image.rotate(float(Mouse::getPosition().x - startingPoint.x) / 3);
              startingPoint = Mouse::getPosition();

              overButton = false;
              for (int i = 0; i < 4; i++) {
                if (rButtons[i].isWithin(Mouse::getPosition(window))) {
                  overButton = true;
                  window.setMouseCursor(cursor);
                }
              }

            }
            else if ( event.type == Event::MouseButtonReleased ) {
              for (int i = 0; i < 4; i++) {
                if (rButtons[i].isWithin(Mouse::getPosition(window))) {
                  image.setRotation(i * 90);
                }
              }
            }
            else if ( event.type == Event::KeyPressed) {
              if (event.key.code == Keyboard::H) {
                image.flip("h");
              }
              else if (event.key.code == Keyboard::V) {
                image.flip("v");
              }
              else if (event.key.code == Keyboard::S) {
                image.save("");
              }
              else if (event.key.code == Keyboard::R) {
                startingPoint = Mouse::getPosition();
                rDown = true;
              }
            }
            else if (event.type == Event::KeyReleased) {
              if (event.key.code == Keyboard::R) {
                rDown = false;
              }
            }
        }
    }

    return EXIT_SUCCESS;                        // report our program exited successfully
}