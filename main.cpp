/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include "keypad.h"
#include "SLCD.h"
#include <stdio.h>

//Blinking rate in milliseconds
#define BLINKING_RATE     1000ms

//Initialise the digital pin LED1 as an output
DigitalOut led(LED1);

//blink green LED to show device is functioning
void SwitchLED(void)
{
    while (true) {
        led = !led;
        ThisThread::sleep_for(BLINKING_RATE);
    }   
}

//check the entered password
bool checkPassword(char input[4], char check[4]) {

    //counter is always 0 when this function is called
    int counter = 0;

    //loop through and compare input password with the password that it's being checked against
    for (int i=0; i<4; i++) {
        //increase counter by 1 when a match is made
        if (input[i] == check[i]) {
            counter++;
        }
    }

    //returns true if all 4 values are correct
    //returns false for all other reasons
    if (counter == 4) {
        return true;
    } else {
        return false;
    }

}

int main() {

    //initialise variables
    int counter = 0;
    char cr;

    //passwords
    //input is the user entry
    //password is the stored password (set to 1234 here)
    //admin is the admin password (set to **** - this can't be changed)
    char input[4];
    char password[4] = {'1', '2', '3', '4'};
    char admin[4] = {'*', '*', '*', '*'};

    //boolean variables used to compare the input value with the user and admin passwords
    bool userCheck;
    bool adminCheck;
    
    //Initialise the digital pin LED1 as an output (green)
    DigitalOut led(LED1);
    //Initialise the digital pin LED2 as an output (red)
    DigitalOut led2(LED2);

    led2 = 1; //set led2 (red) to 1, which is off

    //Initialise the keypad object
    Keypad kpad(D0,D1,D2,D3,D4,D5,D6);

    //this is a thread object
    //UpdateLED() is spawned as a separate thread and sleeps between LED changes
    //Because it is a separate thread the rest of the code continues to run
    Thread LEDUpdate;
        
    //sets the LED update function as a thread
    LEDUpdate.start(SwitchLED);

    //Initialise the display object
    SLCD disp;

    //ask user to enter password
    disp.printf("entr");

    while (true) {
        //read keypad entry - this happens every 10ms
        cr = kpad.ReadKey();

        //checks to see if the return value from the keypad is a viable input
        //(10 is the value returned by NO_KEY i.e. no input detected)
        //saves the entered value into an input array
        if ((cr != 10) && (cr != '#')) {
            input[counter] = cr;
            disp.printf("%c", input[counter]);
            counter++;
        } else if (cr == '#') {
            //clear display when # is entered
            disp.clear();
            //add more clear spaces so the display loops back to the first digit
            for (int i=0; i<(4-counter); i++) {
                disp.printf(" ");
            }
            counter = 0; //set counter to 0 so 4 new values can be entered
        }

        //check password once the counter reaches 4
        if (counter==4){
            //2 checks: admin password and user password
            adminCheck = checkPassword(input, admin);
            userCheck = checkPassword(input, password);

            //if the password is the user password
            if (userCheck == true && adminCheck == false) {
                //correct password so output true and hold LED on
                disp.printf("true");
                for (int i=0; i<2; i++) {
                    led2 = !led2;
                    wait_us(5000000);
                }
                //turn off the output so the door locks again

            //if the password is neither the user or the admin passwords
            } else if (userCheck == false && adminCheck == false) {               
                //incorrect password to output fals and flashes a few times
                for (int i=0; i<3; i++) {
                    disp.printf("fals");
                    led2 = !led2;     
                    wait_us(200000);
                    disp.printf("    ");
                    led2 = !led2;
                    wait_us(200000);
                }

                //no output here - simply loop back to the start and wait for a new password entry
            
            //if the input password is the same as the admin password
            } else if (adminCheck == true) {
                //show that its the admin password
                disp.printf("admn");
                wait_us(1000000);
                disp.printf("    "); //show that the user should now input their new password
                counter = 0; //set counter back to 0 for a new password

                //same loop as before to get an input
                while(true) {
                    //read a keypad input - this uses the same code as before
                    cr = kpad.ReadKey();
                    if ((cr != 10) && (cr != '#')) {        
                        password[counter] = cr;
                        disp.printf("%c", password[counter]);
                        counter++;
                    } else if (cr == '#') {
                        //clear display when # is entered
                        disp.clear();
                        //add more clear spaces so the display loops back to the first digit
                        for (int i=0; i<(4-counter); i++) {
                            disp.printf(" ");
                        }
                        counter = 0; //set counter to 0 so 4 new values can be entered
                    }

                    //once the counter reaches 4, break out of the while loop
                    if (counter == 4){   
                        break;
                    }    
                }

                //outputs the new password on the screen and flashes
                for (int i=0; i<3; i++) {
                    disp.printf("%c%c%c%c", password[0], password[1], password[2], password[3]);     
                    wait_us(500000);
                    disp.printf("    ");
                    wait_us(500000);
                }

            }

            //this only happens once a password has been read
            //NOT every time the code is looped through

            //reset all input values back to 0000
            for(int i=0; i<4; i++) {
                input[i] = 0;
            }

            counter = 0; //set counter back to 0 ready for next entry
            disp.printf("entr"); //display a new enter so the user knows to enter a new value

        }

    }
}   