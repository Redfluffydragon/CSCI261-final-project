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
const int WINDOW_SIZE = 850;

int main() {

    EditImage image; // Automatically gets file name and reads in the file and makes a sprite

    // If the mouse is over a button or not (for changing the cursor)
    bool overButton = false;

    // The starting point of the mouse to calculate rotation
    Vector2i startingPoint;

    Button rButtons[4]{
      Button("0 deg", Vector2f(0, 40), Color::Cyan),
      Button("90 deg", Vector2f(0, 115), Color::Cyan),
      Button("180 deg", Vector2f(0, 190), Color::Cyan),
      Button("270 deg", Vector2f(0, 265), Color::Cyan),
    };

    Button fileBtn("Choose file", Vector2f(350, 400), Color(112, 112, 112));
    Button resetBtn("Reset", Vector2f(350, 470), Color::Red);
    Button saveBtn("Save", Vector2f(350, 540), Color::Green);

    RenderWindow window( VideoMode(WINDOW_SIZE, WINDOW_SIZE), "Image editor" );
    RenderWindow fileWindow;
    string filename = "";

    // Load arial font from file
    Font arial;
    arial.loadFromFile("data/arial.ttf");

    // Window icon
    Image icon;
    icon.loadFromFile("FP_icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Get hand cursor and arrow (default) cursor for buttons
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
        
        if (!filename.empty()) { // If filename has a value
          image.setFile(filename); // Set it as the new file for the image
          filename = ""; // Clear filename
        }

        // window.setMouseCursor(cursor);
        image.draw(window);

        // Draw buttons
        for (int i = 0; i < 4; i++) {
          rButtons[i].draw(window);
        }
        resetBtn.draw(window);
        fileBtn.draw(window);
        saveBtn.draw(window);

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
            else if ( event.type == Event::MouseMoved ) { // Check if cursor is over a button
              overButton = false;
              for (int i = 0; i < 4; i++) {
                if (rButtons[i].isWithin(Mouse::getPosition(window))) {
                  overButton = true;
                }
              }
              if (
                resetBtn.isWithin(Mouse::getPosition(window)) ||
                fileBtn.isWithin(Mouse::getPosition(window)) ||
                saveBtn.isWithin(Mouse::getPosition(window))
              ) {
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
              else if (saveBtn.isWithin(Mouse::getPosition(window))) {
                image.save("");
              }
              else if (fileBtn.isWithin(Mouse::getPosition(window))) {
                fileWindow.create( VideoMode(750, 300), "Choose file");
                fileWindow.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

                Text message("Type a file path and type enter or close this window\nto stick with the already loaded default image.", arial);
                message.setFillColor(Color::Black);
                message.setPosition(Vector2f(10, 10));

                Text showFilename(filename, arial);
                showFilename.setFillColor(Color::Black);
                showFilename.setPosition(Vector2f(10, 100));

                // Hang the main window with the file select window - I think this is fine?
                while(fileWindow.isOpen()) {

                  fileWindow.clear( Color::White );           // clear the contents of the old frame

                  fileWindow.draw(message);
                  fileWindow.draw(showFilename);
                  
                  fileWindow.display();

                  Event fileEvent;
                  while (fileWindow.pollEvent(fileEvent)) {

                    if( fileEvent.type == Event::Closed ) { // if event type is a closed event
                      fileWindow.close();
                    }
                    else if (fileEvent.type == Event::TextEntered) {
                      if (fileEvent.text.unicode < 128) {
                        if (fileEvent.text.unicode == 8) { // Backspace
                          if(!filename.empty()) {
                            filename.pop_back();
                          }
                        }
                        else if (fileEvent.text.unicode == 13) { // Enter
                          fileWindow.close();
                        }
                        else { // Typeable characters (I think)
                          filename += (char)fileEvent.text.unicode;
                        }
                        showFilename.setString(filename);
                      }
                    }
                  }
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
            }
        }
    }

    return EXIT_SUCCESS;                        // report our program exited successfully
}