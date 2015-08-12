
/*

    Rotary encoder 1 is on pins 2 and 3
    Rotary encoder 2 is on pins 4 and 5
    Rotary encoder 3 is on pins 6 and 7
    Rotary encoder 4 is on pins 8 and 9
    Rotary encoder 5 is on pins 10 and 11
    The "push" switch on each one is connected to a "Keys" input pin and just seen as a switch.
    Line 29 ,, change "70"  to "40" for SBA card use right side inputs only
    Line 117 ,, change "70"  to  "40"for SBA card use right side inputs only
    That will make all the pins from pin 8 upwards on a Uno, as "Keys" input pins. (Except pin 13)

*/
#include "math.h"
#include "Quadrature.h"
#include "LedControl.h"
Quadrature quad1(2, 3);  //OK
Quadrature quad2(4, 5); //OK
Quadrature quad3(6, 7); //OK
Quadrature quad4(8, 9); //OK
Quadrature quad5(10, 11); //OK

LedControl led_Display_1 = LedControl (52, 50, 51, 8); // OK MAX7219 LED TUBE 7 segment display
// Pin 52 is the DataOut
// Pin 51 is Load or CS RCLK (aka Latch/load/CS/SS...)
//Pin 50 is clock  SRCLK (aka Clock)

int CodeIn;// used on all serial reads
int KpinNo;  //OK
int Koutpin;  //OK
int KoutpinA; //OK

String Digit;    // Variable as a string to take from getChar()
int Count;       // This variable used in loops in the EQUALS() function
int AP_alt_set[5];   // AP_alt_set[5] is an array of the 5 digits that are the Auto Pilot Altitude Set
int AP_vs_minus;
int AP_vs_set[4];
int AP_hdg_set[3];
int AP_crs_set[3];
int AP_ias_set[3];

String gearN, gearL, gearR, autopilotON, autothrotON, N1ON, airspeedON, headingON, approachON, vorlocON, altilocON;
int gearNi, gearLi, gearRi, autopilotONi, autothrotONi, N1ONi, airspeedONi, headingONi, approachONi, vorlocONi, altilocONi;


String KoldpinStateSTR, KpinStateSTR, Kstringnewstate, Kstringoldstate; //OK

int R;// a variable //OK
int Rold;// the old reading
int Rdif;// the difference since last loop
int R2;// a second test variable
int Rold2;// a second loop old reading
int Rdif2; // the second test difference
int R3;// a second test variable
int Rold3;// a second loop old reading
int Rdif3; // the second test difference
int R4;// a second test variable
int Rold4;// a second loop old reading
int Rdif4; // the second test difference
int R5;// a second test variable
int Rold5;// a second loop old reading
int Rdif5; // the second test difference

void setup()
{
  Kstringoldstate = "111111111111111111111111111111111111111111111111111111111111111111111"; //OK

  for (int KoutPin = 13; KoutPin < 40; KoutPin++)// Get all these pins ready for "Keys inputs"  //OK
  {
    pinMode(KoutPin, INPUT); //OK
    digitalWrite(KoutPin, HIGH);  //OK
  }
  for (int KoutPinA = A0; KoutPinA < A15; KoutPinA++)// Get all these ANALOG pins ready for "Keys ouputs" to. OK
  {
    pinMode(KoutPinA , OUTPUT);
    digitalWrite(KoutPinA , LOW);
  }

  //The MAX72XX is in power-saving mode on startup, we have to do a wakeup call
  delay (500);
  led_Display_1.shutdown(0, false);     // I have 4 displays, these start them up
  delay (500);
  led_Display_1.shutdown(1, false);     // I have 4 displays, these start them up
  delay (500);
  led_Display_1.shutdown(2, false);     // I have 4 displays, these start them up
  delay (500);
  led_Display_1.shutdown(3, false);     // I have 4 displays, these start them up
  delay (500);
  led_Display_1.shutdown(4, false);     // I have 4 displays, these start them up

  led_Display_1.setIntensity(0, 8);
  led_Display_1.setIntensity(1, 12);
  led_Display_1.setIntensity(2, 12);
  led_Display_1.setIntensity(3, 12);
  led_Display_1.setIntensity(4, 12);

  Serial.begin(115200);

}

void loop() {
  {
    KEYS(); //Check the "keys" section
  }
  {
    ENCODER(); //Check the Rotary Encoders
  }
  if (Serial.available()) {// Check if serial data has arrived from PC
    CodeIn = getChar();
    if (CodeIn == '=') {
      EQUALS(); // The first identifier is "="
    }
    if (CodeIn == '<') {
      LESSTHAN(); // The first identifier is "<"
    }
    if (CodeIn == '?') {
      QUESTION(); // The first identifier is "?"
    }
    if (CodeIn == '/') {
      SLASH(); // The first identifier is "/" (Annunciators)
    }
  }

}

char getChar()// Get a character from the serial buffer
{
  while (Serial.available() == 0); // wait for data
  return ((char)Serial.read()); // Thanks Doug
}

void EQUALS() {     // The first identifier was "="
  CodeIn = getChar(); // Get another character
  switch (CodeIn) { // Now lets find what to do with it
  
      case 't'://Found the second identifier
      autothrotON = "";
      autothrotON += getChar();
      //autothrotONi = autothrotON.toInt(); // N1 on off led
      if (autothrotON == "1" ){
        digitalWrite(A15, HIGH);
      } else {
        digitalWrite(A15, LOW);
      }
   break;
   
    case 'u'://Found the second identifier
      N1ON = "";
      N1ON += getChar();
      //N1ONi = N1ON.toInt(); // N1 on off led
      if (N1ON == "1" ){
        digitalWrite(A14, HIGH);
      } else {
        digitalWrite(A14, LOW);
      }
 break;
 
    case 's'://Found the second identifier
      airspeedON = "";
      airspeedON += getChar();
      //airspeedONi = airspeedON.toInt(); // airpseed on led
      if (airspeedON == "1") {
        digitalWrite(A13, HIGH);
      } else {
        digitalWrite(A13, LOW);
      }
 break;
 
    case 'j'://Found the second identifier
      headingON = "";
      headingON += getChar();
      //headingONi = headingON.toInt(); // heading on led
      if (headingON == "1") {
        digitalWrite(A12, HIGH);
      } else {
        digitalWrite(A12, LOW);
      }
 break;
 
    case 'r'://Found the second identifier
      approachON = "";
      approachON += getChar();
   //   approachONi = approachON.toInt(); // approach hold on led
      if (approachON == "1") {
        digitalWrite(A11, HIGH);
      } else {
        digitalWrite(A11, LOW);
      }
 break;
 
    case 'o'://Found the second identifier
      vorlocON = "";
      vorlocON += getChar();
      //vorlocONi = vorlocON.toInt(); // vor loc nav1  hold on led
      if (vorlocON == "1") {
        digitalWrite(A10, HIGH);
      } else {
        digitalWrite(A10, LOW);
      }
 break;
 
    case 'k'://Found the second identifier
      altilocON = "";
      altilocON += getChar();
      //altilocONi = altilocON.toInt(); // altittude lock  hold on led
      if (altilocON == "1") {
        digitalWrite(A9, HIGH);
      } else {
        digitalWrite(A9, LOW);
      }
 break;
 
    case 'a'://Found the second identifier
      autopilotON = "";
      autopilotON += getChar();
      //autopilotONi = autopilotON.toInt(); // autopilot active  hold on led
      if (autopilotON == "1") {
        digitalWrite(A8, HIGH);
      } else {
        digitalWrite(A8, LOW);
      }
 break;
 
    case 'e':    //  AP course set screen adres 0
      Count = 0;
      while (Count < 3)
      {
        Digit = "";
        Digit += getChar();

        AP_crs_set[Count] = Digit.toInt();
        Count++;
      }
      if (AP_crs_set[0] == 0 and AP_crs_set[1] == 0 and AP_crs_set[2] == 0)
      {
        AP_crs_set[0] = 3; AP_crs_set[1] = 6;
      }
      led_Display_1.setDigit(0, 7, AP_crs_set[0], false);
      led_Display_1.setDigit(0, 6, AP_crs_set[1], false);
      led_Display_1.setDigit(0, 5, AP_crs_set[2], false);
      //                 led_Display_1.setDigit(5,6,AP_crs_set[0],false); IF co pilot course display is connected add these
      //                 led_Display_1.setDigit(5,5,AP_crs_set[1],false);
      //                 led_Display_1.setDigit(5,4,AP_crs_set[2],false);

      break;

    case 'f':    //  IAS MACH SPEED screen adres 1
      Count = 0;
      while (Count < 3)
      {
        Digit = "";
        Digit += getChar();

        AP_ias_set[Count] = Digit.toInt();
        Count++;
      }
      if (AP_ias_set[0] == 0 and AP_ias_set[1] == 0 and AP_ias_set[2] == 0)
      {
        AP_ias_set[0] = 3; AP_ias_set[1] = 6;
      }

      led_Display_1.setDigit(0, 2, AP_ias_set[0], false);
      led_Display_1.setDigit(0, 1, AP_ias_set[1], false);
      led_Display_1.setDigit(0, 0, AP_ias_set[2], false);
      break;

    case 'd':    //  AP heading set Screen adres 2
      Count = 0;
      while (Count < 3)
      {
        Digit = "";
        Digit += getChar();

        AP_hdg_set[Count] = Digit.toInt();
        Count++;
      }
      if (AP_hdg_set[0] == 0 and AP_hdg_set[1] == 0 and AP_hdg_set[2] == 0)
      {
        AP_hdg_set[0] = 3; AP_hdg_set[1] = 6;
      }
      led_Display_1.setDigit(1, 7, AP_hdg_set[0], false);
      led_Display_1.setDigit(1, 6, AP_hdg_set[1], false);
      led_Display_1.setDigit(1, 5, AP_hdg_set[2], false);
      break;

    case 'b':           //  AP altitude set screen adres 3
      Count = 0;               // clear the Count
      while (Count < 5)        // we will count to 5 from 0 to 4
      {
        Digit = "";          // clears the string variable Digit
        Digit += getChar();  // Makes the string Digit what ever getChar() is holding

        AP_alt_set[Count] = Digit.toInt(); // Turns the string Digit into an Integer and
        // then stores it in the Array AP_alt_set[] 0,1,2,3,4
        Count++;
      }

      led_Display_1.setDigit(1, 4, AP_alt_set[0], false); // First digit of Alt Set is displayed
      led_Display_1.setDigit(1, 3, AP_alt_set[1], false); // Second digit of Alt Set is displayed
      led_Display_1.setDigit(1, 2, AP_alt_set[2], false); // Third digit of Alt Set is displayed
      led_Display_1.setDigit(1, 1, AP_alt_set[3], false); // Fourth digit of Alt Set is displayed
      led_Display_1.setDigit(1, 0, AP_alt_set[4], false); // Fifth digit of Alt Set is displayed
      // false denotes NO decimal point
      break;

    case 'c':    //  AP vertical speed set screen 4
      Count = 0;
      while (Count < 4)
      {
        Digit = "";
        Digit += getChar();

        if (Count == 0)
        {
          if (Digit == "-")
          {
            AP_vs_minus = '-';
          }
          else
          {
            AP_vs_minus = ' ';
          }
          Digit = "";
          Digit += getChar();
        }

        AP_vs_set[Count] = Digit.toInt();
        Count++;
      }
      led_Display_1.setChar(2, 4, AP_vs_minus, false);
      led_Display_1.setDigit(2, 3, AP_vs_set[0], false);
      led_Display_1.setDigit(2, 2, AP_vs_set[1], false);
      led_Display_1.setDigit(2, 1, AP_vs_set[2], false);
      led_Display_1.setDigit(2, 0, AP_vs_set[3], false);
      break;

  }
}
//
//
//
//
void LESSTHAN() {   // The first identifier was "<"
  CodeIn = getChar(); // Get the second identifier
  switch (CodeIn) { // Now lets find what to do with it
    case 'A'://Found the second identifier "GearN"
      gearN = "";
      gearN += getChar();
      gearN += getChar();
      gearN += getChar();
      gearNi = gearN.toInt(); // convert it to an integer (Thanks Phill)
      if (gearNi > 99) {
        digitalWrite(A1, HIGH);
      } else {
        digitalWrite(A1, LOW);  //GEAR GREEN OK
        if (gearNi >= 1 && gearNi <= 98) {
          digitalWrite(A0, HIGH); //GEAR RED OK
        } else {
          digitalWrite(A0, LOW);
        }
      }
      // See "OTHER" for more uses of gearni
      break;

    case 'B':
      gearL = "";
      gearL += getChar();
      gearL += getChar();
      gearL += getChar();
      gearLi = gearL.toInt(); // convert it to an integer (Thanks Phill)
      if (gearNi > 99) {
        digitalWrite(A3, HIGH);
      } else {
        digitalWrite(A3, LOW);  //GEAR GREEN OK
        if (gearNi >= 1 && gearNi <= 98) {
          digitalWrite(A2, HIGH); //GEAR RED OK
        } else {
          digitalWrite(A2, LOW);
        }
      }
      // See "OTHER" for more uses of gearLi
      break;

    case 'C':
      gearR = "";
      gearR += getChar();
      gearR += getChar();
      gearR += getChar();
      gearRi = gearR.toInt(); // convert it to an integer (Thanks Phill)
      if (gearNi > 99) {
        digitalWrite(A5, HIGH);
      } else {
        digitalWrite(A5, LOW);  //GEAR GREEN OK
        if (gearNi >= 1 && gearNi <= 98) {
          digitalWrite(A4, HIGH); //GEAR RED OK
        } else {
          digitalWrite(A4, LOW);
        }
      }
      // See "OTHER" for more uses of gearRi
      break;

      //etc etc etc
  }
}

void QUESTION() {   // The first identifier was "?"
  CodeIn = getChar(); // Get another character
  switch (CodeIn) { // Now lets find what to do with it
    case 'A'://Found the second identifier
      //Do something
      break;

    case 'B':
      //Do something
      break;

    case 'C':
      //Do something
      break;

      //etc etc etc
  }
}
void SLASH() {   // The first identifier was "/" (Annunciator)
  //Do something
}
void KEYS()
{
  Kstringnewstate = "";
  for (int KpinNo = 13; KpinNo < 40; KpinNo++) {
    KpinStateSTR = String(digitalRead(KpinNo));
    KoldpinStateSTR = String(Kstringoldstate.charAt(KpinNo - 13));
    if (KpinStateSTR != KoldpinStateSTR)
    {
      if (KpinNo != 13) {
        if (KpinNo == 14 && KpinStateSTR == "0") {
          Serial.println ("B340"); // autopilot off  OK
        }
        if (KpinNo == 15 && KpinStateSTR == "0") {
          Serial.println ("B341"); // autopilot on   OK
        }
        if (KpinNo == 16 && KpinStateSTR == "0") {
          Serial.println ("B300"); // flightdirector off
        }
        if (KpinNo == 17 && KpinStateSTR == "0") {
          Serial.println ("B301"); // flightd on
        }
        if (KpinNo == 18 && KpinStateSTR == "0") {
          Serial.println ("B31"); // AP N1 on off
        }
        if (KpinNo == 19 && KpinStateSTR == "0") {
          Serial.println ("B26"); // airspeed hold same as B20 colde mach
        }
        if (KpinNo == 20 && KpinStateSTR == "0") {
          Serial.println ("B04"); // heading on off
        }
        if (KpinNo == 21 && KpinStateSTR == "0") {
          Serial.println ("B08"); // APP ON OFF
        }
        if (KpinNo == 22 && KpinStateSTR == "0") {
          Serial.println ("B10"); // nav or gps switch  nav hold mode
        }
        if (KpinNo == 24 && KpinStateSTR == "0") {
          Serial.println ("B05"); // alltitude hold on off
        }
        if (KpinNo == 25 && KpinStateSTR == "0") {
          Serial.println ("B01"); // CMD A autopilot on off
        }
        if (KpinNo == 26 && KpinStateSTR == "0") {
          Serial.println ("B11"); //
        }
        if (KpinNo == 27 && KpinStateSTR == "0") {
          Serial.println ("B11"); //
        }
        if (KpinNo == 28 && KpinStateSTR == "0") {
          Serial.println ("B11"); //
        }
        if (KpinNo == 29 && KpinStateSTR == "0") {
          Serial.println ("B11"); //
        }
        if (KpinNo == 30 && KpinStateSTR == "0") {
          Serial.println ("B11"); //
        }
        if (KpinNo == 31 && KpinStateSTR == "0") {
          Serial.println ("B11"); //
        }
        if (KpinNo == 32 && KpinStateSTR == "0") {
          Serial.println ("B11"); //
        }
        if (KpinNo == 33 && KpinStateSTR == "0") {
          Serial.println ("B11"); //
        }
        if (KpinNo == 34 && KpinStateSTR == "0") {
          Serial.println ("B11"); //
        }
        if (KpinNo == 35 && KpinStateSTR == "0") {
          Serial.println ("B11"); //
        }
        if (KpinNo == 36 && KpinStateSTR == "0") {
          Serial.println ("B11"); //
        }
        if (KpinNo == 37 && KpinStateSTR == "0") {
          Serial.println ("B11"); //
        }
        if (KpinNo == 38 && KpinStateSTR == "0") {
          Serial.println ("C01"); // LandingGEAR UP   OK
        }
        if (KpinNo == 39 && KpinStateSTR == "0") {
          Serial.println ("C02"); // LandingGEAR DOWN  OK
        }
        if (KpinNo == 40 && KpinStateSTR == "0") {
           Serial.println ("B11"); //
        }

        //  Add more here but remember to change the figure in the next line down.
        if (KpinNo > 40) { //Change pinNo number to same as the highest one used for simconnect codes
          Serial.print ("D");
          if (KpinNo < 40) Serial.print ("0");
          Serial.print (KpinNo);
          Serial.println (KpinStateSTR);
        }
      }
    }
    Kstringnewstate += KpinStateSTR;
  }
  Kstringoldstate = Kstringnewstate;
}
void ENCODER() {                                                           //ALL ENCODERS OK !
  R = (quad1.position() / 2); //The /2 is to suit the encoder(See my website)
  if (R != Rold) { // checks to see if it different
    (Rdif = (R - Rold)); // finds out the difference
    if (Rdif == 1) Serial.println ("B13");//
    if (Rdif == -1) Serial.println ("B14"); //
    Rold = R; // overwrites the old reading with the new one.
  }
  R2 = (quad2.position());
  if (R2 != Rold2) {
    (Rdif2 = (R2 - Rold2));
    if (Rdif2 == 1) Serial.println ("B11"); //
    if (Rdif2 == -1) Serial.println ("B12");//
    Rold2 = R2;
  }
  R3 = (quad3.position());
  if (R3 != Rold3) {
    (Rdif3 = (R3 - Rold3));
    if (Rdif3 == 1) Serial.println ("A57"); //
    if (Rdif3 == -1) Serial.println ("A58"); //
    Rold3 = R3;
  }
  R4 = (quad4.position());
  if (R4 != Rold4) {
    (Rdif4 = (R4 - Rold4));
    if (Rdif4 == 1) Serial.println ("B15"); // MACH
    //    if (Rdif4 == 1) Serial.println ("B18"); //IAS
    if (Rdif4 == -1) Serial.println ("B16");// MACH
    //    if (Rdif4 == -1) Serial.println ("B19");//IAS
    Rold4 = R4;
  }
  R5 = (quad5.position());
  if (R5 != Rold5) {
    (Rdif5 = (R5 - Rold5));
    if (Rdif5 == 1) Serial.println ("A55"); //
    if (Rdif5 == -1) Serial.println ("A56");//
    Rold5 = R5;
  }
}



