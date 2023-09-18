// ============================================
// The Hardware class file
//
// Copyright 2022 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


#include "globals.h"
#include "hardware.h"

// We need to actually instantiate some of the globals for hardwares 
// (i.e. declare them once without the extern keyword).
// That's what this file does!

// Hardware initialization: Instantiate all the things!
uLCD_4DGL uLCD(p9,p10,p11);                 // LCD Screen (tx, rx, reset)
Serial pc(USBTX,USBRX);                     // USB Console (tx, rx)
DigitalIn button1(p21);                     // Invincibility
DigitalIn button2(p22);                     // Pause game
DigitalIn button3(p23);                     // Menu 
DigitalIn button4(p24);                     // Resume game
AnalogOut DACout(p18);                      // Speaker (pin)
PwmOut speaker(p26);                        // ...
wave_player waver(&DACout);                 // Wave player
Nav_Switch navs(p12,p15,p14,p16,p13);       // Nav Switch

/*
 * This initializes the hardware by calling some functions to set them up.
 */
int hardware_init()
{
    // Crank up the speed
    uLCD.baudrate(3000000);
    pc.baud(115200);
        
    //Initialize action pushbuttons
    button1.mode(PullUp); 
    button2.mode(PullUp);
    button3.mode(PullUp);
    button4.mode(PullUp);
    
    
    return ERROR_NONE;
}

/*
 * This handles reading inputs from all possible devices - pushbuttons, nav_switch
 */
GameInputs read_inputs() 
{
    GameInputs in;
    
    
    //1. Read action buttons from in
    in.b1 = button1.read();
    in.b2 = button2.read();
    in.b3 = button3.read();
    in.b4 = button4.read();
    
    //2. Read navigation switch buttons from in
    in.ns_up = navs.up();
    in.ns_down = navs.down();
    in.ns_left = navs.left();
    in.ns_right = navs.right();
    in.ns_center = navs.center();
    
    wait(0.02); //For simple debouncing
    return in;
}
