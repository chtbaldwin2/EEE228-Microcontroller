#pragma once
#include "mbed.h"


class Keypad
{
    public:

    // constructor for the keypad class - you need to supply the pin names to which the rows/
    // and columns are connected
    Keypad(PinName row0, 
           PinName row1, 
           PinName row2, 
           PinName row3, 
           PinName col0, 
           PinName col1, 
           PinName col2);

    // function that reads a key - returns mmediately with the key pressed or NO_KEY
    // only one instance of a key value will be returned - coincident with the onset of 
    // the key being pressed.
    char ReadKey(void);

    bool ifPressed(void);

   #define NO_KEY 10;

    protected:

    private:

    // this key scan function does most of the heavy lifting it is intended to be spawned as 
    // an independent thread that runs in the background
    void KeyScanner(void);

    // thread to hold the key scanner
    Thread keyscan;

    // class definitions for the rwos (inputs) and columns (outputs)
    DigitalIn _row0, _row1, _row2, _row3;
    DigitalOut _col0, _col1, _col2;

    // keypad mapping ket position to value
    char mapping[3][4] = {{'1','4','7','*'},{'2','5','8','0'},{'3','6','9','#'}};

    // variables shared between keyscanner and read key
    char key, key_p;

};