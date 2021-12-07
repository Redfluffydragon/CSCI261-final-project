/* CSCI 261 FINAL PROJECT
 *
 * Author: KAI-SHEN DERU
 * 
 * Resources used (Office Hours, Tutoring, Other Students, etc & in what capacity):
 *
 * !More complete description here...
 * https://github.com/lvandeve/lodepng/blob/master/examples/example_encode.cpp
 * https://github.com/elanthis/upng
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
const int WINDOW_SIZE = 840;

int main() {

    EditImage image; // Automatically gets file name and reads in the file and makes a sprite

    // If the mouse is over a button or not (for changing the cursor)
    bool overButton = false;

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

    Button resetBtn("Reset", Vector2f(350, 400), Color::Red);

    RenderWindow window( VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Image editor" );

    Cursor handCursor;
    if (!handCursor.loadFromSystem(Cursor::Hand)) {
      cerr << "Failed to load hand cursor" << endl;
    }
    Cursor arrowCursor;
    if (!arrowCursor.loadFromSystem(Cursor::Arrow)) {
      cerr << "Failed to load default cursor" << endl;
    }


    // while our window is open, keep it open
    // this is our draw loop
    while( window.isOpen() ) {
        window.clear( Color::White );           // clear the contents of the old frame
                                                // by setting the window to black

        //****************************************
        //  ADD ALL OF OUR DRAWING BELOW HERE
        //****************************************
        
        // window.setMouseCursor(cursor);
        image.draw(window);

        // Buttons
        for (int i = 0; i < 4; i++) {
          rButtons[i].draw(window);
        }
        resetBtn.draw(window);

        // Make the cursor a hand when you hover over buttons
        overButton ?
          window.setMouseCursor(handCursor) :
          window.setMouseCursor(arrowCursor);

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
            else if ( event.type == Event::MouseMoved ) {
              if (rDown) {
                image.rotate(float(Mouse::getPosition().x - startingPoint.x) / 3);
                startingPoint = Mouse::getPosition();
              }

              overButton = false;
              for (int i = 0; i < 4; i++) {
                if (rButtons[i].isWithin(Mouse::getPosition(window))) {
                  overButton = true;
                }
              }
              if (resetBtn.isWithin(Mouse::getPosition(window))) {
                overButton = true;
              }

            }
            else if ( event.type == Event::MouseButtonReleased ) {
              for (int i = 0; i < 4; i++) {
                if (rButtons[i].isWithin(Mouse::getPosition(window))) {
                  image.setRotation(i * 90);
                }
              }
              if (resetBtn.isWithin(Mouse::getPosition(window))) {
                image.reset();
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