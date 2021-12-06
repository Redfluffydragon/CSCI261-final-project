/* CSCI 261 FINAL PROJECT
 *
 * Author: KAI-SHEN DERU
 *
 * !More complete description here...
 */

// The include section adds extra definitions from the C++ standard library.
#include <iostream> // For cin, cout, etc.
#include "EditImage.h"

// We will (most of the time) use the standard library namespace in our programs.
using namespace std;

#include <SFML/Graphics.hpp>
using  namespace sf;

// Define any constants below this comment.
const int WINDOW_SIZE = 640;

int main() {

    EditImage image; // Automatically gets file name and reads in the file and makes a sprite


    RenderWindow window( VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Image editor" );

    // while our window is open, keep it open
    // this is our draw loop
    while( window.isOpen() ) {
        window.clear( Color::White );           // clear the contents of the old frame
                                                // by setting the window to black

        //****************************************
        //  ADD ALL OF OUR DRAWING BELOW HERE
        //****************************************

        image.draw(window);

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
            else if ( event.type == Event::MouseButtonReleased ) {

            }
            else if ( event.type == Event::KeyPressed && event.key.code == Keyboard::D ) {

            }
        }
    }

    return EXIT_SUCCESS;                        // report our program exited successfully
}