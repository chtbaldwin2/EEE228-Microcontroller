#include "keypad.h"
#include "ThisThread.h"


// keypad constructor - allows the pins for the rows/columns to be specified
Keypad::Keypad(PinName row0, 
               PinName row1, 
               PinName row2, 
               PinName row3, 
               PinName col0, 
               PinName col1, 
               PinName col2): _row0(row0), _row1(row1), _row2(row2), _row3(row3), _col0(col0), _col1(col1), _col2(col2)
{

    // create a 10ms scanning facility using a thread
    keyscan.start(callback(this, &Keypad::KeyScanner));
    key = NO_KEY;
    key_p = NO_KEY;
   


}
  int x = 0;
  int temp = 0;


// key scanning thread
void Keypad::KeyScanner(void)
{
    static  int time10MsCnt = 0;
    // do forever
    while (1==1) {
        // left column
        _col0 = 0; _col1 = 1; _col2 = 1;
        // read rows
        if (_row0 == 0) {key = mapping[0][0]; x = x + 1;}
        if (_row1 == 0) {key = mapping[0][1]; x = x + 1;}
        if (_row2 == 0) {key = mapping[0][2]; x = x + 1;}
        if (_row3 == 0) {key = mapping[0][3]; x = x + 1;}

        // middle column
        _col0 = 1; _col1 = 0; _col2 = 1;
        // read rows
        if (_row0 == 0) {key = mapping[1][0]; x = x + 1;}
        if (_row1 == 0) {key = mapping[1][1]; x = x + 1;}
        if (_row2 == 0) {key = mapping[1][2]; x = x + 1;}
        if (_row3 == 0) {key = mapping[1][3]; x = x + 1;}

        // right column
        _col0 = 1; _col1 = 1; _col2 = 0;
        // read rows
        if (_row0 == 0) {key = mapping[2][0]; x = x + 1;}
        if (_row1 == 0) {key = mapping[2][1]; x = x + 1;}
        if (_row2 == 0) {key = mapping[2][2]; x = x + 1;}
        if (_row3 == 0) {key = mapping[2][3]; x = x + 1;}

        // read again in 10ms
        ThisThread::sleep_for(10ms);
        // while(key != 0)
        // {
        //     ThisThread::sleep_for(10ms);
        //     if(time10MsCnt++ > 100)
        //     {
        //         time10MsCnt = 0;
        //         break;
        //     }
        // }
    }
}


// uses current value of key to determine if key pressed

char Keypad::ReadKey(void)
{
    // ThisThread::sleep_for(50ms);
    key = 0;
    ThisThread::sleep_for(100ms);
    return key;
}