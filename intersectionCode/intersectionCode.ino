#include <Wire.h>
// #include <Adafruit_PWMServoDriver.h>

// Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
// #define SERVOMIN 150   // This is the 'minimum' pulse length count (out of 4096) - This is the start position of the servo
// #define SERVOMAX 400   // This is the 'maximum' pulse length count (out of 4096) - This is the end position of the servo
// #define USMIN 600      // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
// #define USMAX 2400     // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
// #define SERVO_FREQ 50  // Analog servos run at ~50 Hz updates

//int brightness = 128;  // 50% brightness (range from 0 to 255)

//Seconds to be in each state
//int timeGreen = 10

//Blueprint for a traffic light
typedef struct roadLight {
  int red;
  int yellow;
  int green;
} roadLight;

//Blueprint for a crosswalk light
typedef struct walkLight {
  int wait;
  int walk;
} walkLight;

//Blueprint for an intersection
typedef struct intersection {
  struct roadLight *mainRoadLight;
  struct roadLight *sideRoadLight;
  struct walkLight *walkTopRight;
  struct walkLight *walkTopLeft;
  struct walkLight *walkBottomRight;
  struct walkLight *walkBottomLeft;
} intersection;


//Create the intersections
intersection intersection1;
intersection intersection2;
intersection intersection3;
intersection intersection4;
intersection intersection5;

//Declare intersectionsArray
intersection *intersectionsArray[5] = { &intersection1, &intersection2, &intersection3, &intersection4, &intersection5 };

/*
 Intersection state (for intersections 1-5)
 State:
 0 - Start of new intersection loop
 1 - Yellow light on main st - blink walk lights
 2 - Red light on main st
 3 - Green light on side st
 4 - Yellow light on side st - blink walk lights
 5 - Red light on side st
 */
int intersectionState[5] = { 0, 0, 0, 0, 0 };




//Millis of the start of the current loop (intersections 1-5)
unsigned long previousMillis[5] = { 0, 0, 0, 0, 0 };
//Holds the current milliseconds since the program started
unsigned long currentMillis;


//A buffer/range in milliseconds for checking if an intersection should enter another state
//Needed because not every millisecond will be evaluated for state change, some are skipped.
int millisBuffer;


int intersectionSwitchDelay;


// put your setup code here, to run once:
void setup() {

  for (int i = 0; i < 5; i++) {
    intersectionsArray[i]->mainRoadLight = malloc(sizeof(roadLight));
    intersectionsArray[i]->sideRoadLight = malloc(sizeof(roadLight));
    intersectionsArray[i]->walkTopRight = malloc(sizeof(walkLight));
    intersectionsArray[i]->walkTopLeft = malloc(sizeof(walkLight));
    intersectionsArray[i]->walkBottomRight = malloc(sizeof(walkLight));
    intersectionsArray[i]->walkBottomLeft = malloc(sizeof(walkLight));
  }

  //Tell arduino to talk to serial monitor
  // Serial.begin(9600);

  //Map all pins to their intersection lights. These numbers will likely have to be changed
  //for a replicate setup

  //Map pins to intersection 1
  intersection1.mainRoadLight->red = 22;
  intersection1.mainRoadLight->yellow = 23;
  intersection1.mainRoadLight->green = 24;
  intersection1.sideRoadLight->red = 25;
  intersection1.sideRoadLight->yellow = 26;
  intersection1.sideRoadLight->green = 27;
  intersection1.walkTopRight->wait = 10;
  intersection1.walkTopRight->walk = 11;
  intersection1.walkTopLeft->wait = 13;
  intersection1.walkTopLeft->walk = 12;
  intersection1.walkBottomLeft->wait = 7;
  intersection1.walkBottomLeft->walk = 6;
  intersection1.walkBottomRight->wait = 8;
  intersection1.walkBottomRight->walk = 9;


  //Map pins to intersection 2
  intersection2.mainRoadLight->red = 28;
  intersection2.mainRoadLight->yellow = 29;
  intersection2.mainRoadLight->green = 30;
  intersection2.sideRoadLight->red = 31;
  intersection2.sideRoadLight->yellow = 32;
  intersection2.sideRoadLight->green = 33;
  intersection2.walkTopRight->wait = 14;
  intersection2.walkTopRight->walk = 15;
  intersection2.walkTopLeft->wait = 16;
  intersection2.walkTopLeft->walk = 17;
  intersection2.walkBottomLeft->wait = 18;
  intersection2.walkBottomLeft->walk = 19;
  intersection2.walkBottomRight->wait = 20;
  intersection2.walkBottomRight->walk = 21;



  //Map pins to intersection 3
  intersection3.mainRoadLight->red = 34;
  intersection3.mainRoadLight->yellow = 35;
  intersection3.mainRoadLight->green = 36;
  intersection3.sideRoadLight->red = 37;
  intersection3.sideRoadLight->yellow = 38;
  intersection3.sideRoadLight->green = 39;
  intersection3.walkTopRight->wait = 69;
  intersection3.walkTopRight->walk = 68;
  intersection3.walkTopLeft->wait = 67;
  intersection3.walkTopLeft->walk = 66;
  intersection3.walkBottomLeft->wait = 65;
  intersection3.walkBottomLeft->walk = 64;
  intersection3.walkBottomRight->wait = 63;
  intersection3.walkBottomRight->walk = 62;



  //Map pins to intersection 4
  intersection4.mainRoadLight->red = 40;
  intersection4.mainRoadLight->yellow = 41;
  intersection4.mainRoadLight->green = 42;
  intersection4.sideRoadLight->red = 43;
  intersection4.sideRoadLight->yellow = 44;
  intersection4.sideRoadLight->green = 45;
  intersection4.walkTopRight->wait = 61;
  intersection4.walkTopRight->walk = 60;
  intersection4.walkTopLeft->wait = 59;
  intersection4.walkTopLeft->walk = 58;
  intersection4.walkBottomLeft->wait = 4;
  intersection4.walkBottomLeft->walk = 5;
  intersection4.walkBottomRight->wait = 2;  //not connected?
  intersection4.walkBottomRight->walk = 3;  //not connected?


  //Map pins to intersection 5
  intersection5.mainRoadLight->red = 46;
  intersection5.mainRoadLight->yellow = 47;
  intersection5.mainRoadLight->green = 48;
  intersection5.sideRoadLight->red = 49;
  intersection5.sideRoadLight->yellow = 50;
  intersection5.sideRoadLight->green = 51;
  intersection5.walkTopRight->wait = 57;
  intersection5.walkTopRight->walk = 56;
  // intersection5.walkTopLeft->wait =
  // intersection5.walkTopLeft->walk =
  intersection5.walkBottomLeft->wait = 55;
  intersection5.walkBottomLeft->walk = 54;
  // intersection5.walkBottomRight->wait =
  // intersection5.walkBottomRight->walk =

  //Set all pin modes to OUTPUT
  pinMode(intersection1.mainRoadLight->red, OUTPUT);
  pinMode(intersection1.mainRoadLight->yellow, OUTPUT);
  pinMode(intersection1.mainRoadLight->green, OUTPUT);
  pinMode(intersection2.mainRoadLight->red, OUTPUT);
  pinMode(intersection2.mainRoadLight->yellow, OUTPUT);
  pinMode(intersection2.mainRoadLight->green, OUTPUT);
  pinMode(intersection3.mainRoadLight->red, OUTPUT);
  pinMode(intersection3.mainRoadLight->yellow, OUTPUT);
  pinMode(intersection3.mainRoadLight->green, OUTPUT);
  pinMode(intersection4.mainRoadLight->red, OUTPUT);
  pinMode(intersection4.mainRoadLight->yellow, OUTPUT);
  pinMode(intersection4.mainRoadLight->green, OUTPUT);
  pinMode(intersection5.mainRoadLight->red, OUTPUT);
  pinMode(intersection5.mainRoadLight->yellow, OUTPUT);
  pinMode(intersection5.mainRoadLight->green, OUTPUT);
  pinMode(intersection1.sideRoadLight->red, OUTPUT);
  pinMode(intersection1.sideRoadLight->yellow, OUTPUT);
  pinMode(intersection1.sideRoadLight->green, OUTPUT);
  pinMode(intersection2.sideRoadLight->red, OUTPUT);
  pinMode(intersection2.sideRoadLight->yellow, OUTPUT);
  pinMode(intersection2.sideRoadLight->green, OUTPUT);
  pinMode(intersection3.sideRoadLight->red, OUTPUT);
  pinMode(intersection3.sideRoadLight->yellow, OUTPUT);
  pinMode(intersection3.sideRoadLight->green, OUTPUT);
  pinMode(intersection4.sideRoadLight->red, OUTPUT);
  pinMode(intersection4.sideRoadLight->yellow, OUTPUT);
  pinMode(intersection4.sideRoadLight->green, OUTPUT);
  pinMode(intersection5.sideRoadLight->red, OUTPUT);
  pinMode(intersection5.sideRoadLight->yellow, OUTPUT);
  pinMode(intersection5.sideRoadLight->green, OUTPUT);




  pinMode(intersection1.walkTopRight->walk, OUTPUT);
  pinMode(intersection1.walkTopRight->wait, OUTPUT);
  pinMode(intersection1.walkTopLeft->walk, OUTPUT);
  pinMode(intersection1.walkTopLeft->wait, OUTPUT);
  pinMode(intersection1.walkBottomLeft->walk, OUTPUT);
  pinMode(intersection1.walkBottomLeft->wait, OUTPUT);
  pinMode(intersection1.walkBottomRight->walk, OUTPUT);
  pinMode(intersection1.walkBottomRight->wait, OUTPUT);


  pinMode(intersection2.walkTopRight->walk, OUTPUT);
  pinMode(intersection2.walkTopRight->wait, OUTPUT);
  pinMode(intersection2.walkTopLeft->walk, OUTPUT);
  pinMode(intersection2.walkTopLeft->wait, OUTPUT);
  pinMode(intersection2.walkBottomLeft->walk, OUTPUT);
  pinMode(intersection2.walkBottomLeft->wait, OUTPUT);
  pinMode(intersection2.walkBottomRight->walk, OUTPUT);
  pinMode(intersection2.walkBottomRight->wait, OUTPUT);


  pinMode(intersection3.walkTopRight->walk, OUTPUT);
  pinMode(intersection3.walkTopRight->wait, OUTPUT);
  pinMode(intersection3.walkTopLeft->walk, OUTPUT);
  pinMode(intersection3.walkTopLeft->wait, OUTPUT);
  pinMode(intersection3.walkBottomLeft->walk, OUTPUT);
  pinMode(intersection3.walkBottomLeft->wait, OUTPUT);
  pinMode(intersection3.walkBottomRight->walk, OUTPUT);
  pinMode(intersection3.walkBottomRight->wait, OUTPUT);


  pinMode(intersection4.walkTopRight->walk, OUTPUT);
  pinMode(intersection4.walkTopRight->wait, OUTPUT);
  pinMode(intersection4.walkTopLeft->walk, OUTPUT);
  pinMode(intersection4.walkTopLeft->wait, OUTPUT);
  pinMode(intersection4.walkBottomLeft->walk, OUTPUT);
  pinMode(intersection4.walkBottomLeft->wait, OUTPUT);
  pinMode(intersection4.walkBottomRight->walk, OUTPUT);
  pinMode(intersection4.walkBottomRight->wait, OUTPUT);


  pinMode(intersection5.walkTopRight->walk, OUTPUT);
  pinMode(intersection5.walkTopRight->wait, OUTPUT);
  pinMode(intersection5.walkTopLeft->walk, OUTPUT);
  pinMode(intersection5.walkTopLeft->wait, OUTPUT);
  pinMode(intersection5.walkBottomLeft->walk, OUTPUT);
  pinMode(intersection5.walkBottomLeft->wait, OUTPUT);
  pinMode(intersection5.walkBottomRight->walk, OUTPUT);
  pinMode(intersection5.walkBottomRight->wait, OUTPUT);


  //Initialize lights (Main St to green, side streets to red, walk lights accordingly)
  for (int i = 0; i < 5; i++) {
    // Serial.println("Initializing lights");
    changeToGreen(intersectionsArray[i]->mainRoadLight);
    changeToRed(intersectionsArray[i]->sideRoadLight);
    changeToWalk(intersectionsArray[i]->walkTopRight);
    changeToWalk(intersectionsArray[i]->walkBottomLeft);
    changeToWait(intersectionsArray[i]->walkTopLeft);
    changeToWait(intersectionsArray[i]->walkBottomRight);
  }


  millisBuffer = 50; //Millisecond window for the currentMillis to land in to trigger a state change
  intersectionSwitchDelay = 2000;  //2 second delay between intersections
  currentMillis = millis();
  for (int i = 0; i < 5; i++) {
    previousMillis[i] = currentMillis;
  }
  // Serial.println("Setup complete");
}

void loop() {
  //Serial.print("Pin of intersection 1 main road light green: ");
  //Serial.println(intersectionsArray[0].mainRoadLight.green);
  for (int i = 0; i < 5; i++) {
    // Serial.print("Intersection: ");
    // Serial.println(i);
    intersectionController(i);
  }
  //changeToGreen(intersectionsArray[4].mainRoadLight);
  //Serial.println(intersectionsArray[0].mainRoadLight.red);
  //changeToGreen(intersection1.mainRoadLight);
  //Serial.println(intersection1.mainRoadLight.green);
}


//Functions /////////////////////////////////////////////////////////////////

/*
Change roadLight to red
@param light - The roadLight you want to turn red
*/
void changeToRed(roadLight *light) {
  digitalWrite(light->red, HIGH);
  digitalWrite(light->yellow, LOW);
  digitalWrite(light->green, LOW);
}

/*
Change roadLight to yellow
@param light - The roadLight you want to turn yellow
*/
void changeToYellow(roadLight *light) {
  digitalWrite(light->red, LOW);
  digitalWrite(light->yellow, HIGH);
  digitalWrite(light->green, LOW);
}

/*
Change roadLight to green
@param light - The roadLight you want to turn green
*/
void changeToGreen(roadLight *light) {
  digitalWrite(light->red, LOW);
  digitalWrite(light->yellow, LOW);
  digitalWrite(light->green, HIGH);
}

/*
Change walkLight light to walk
@param light - The walkLight you want to turn to walk
*/
void changeToWalk(walkLight *light) {
  digitalWrite(light->walk, HIGH);
  digitalWrite(light->wait, LOW);
}

/*
Change walkLight to wait
@param light - The walkLight you want to turn wait
*/
void changeToWait(walkLight *light) {
  digitalWrite(light->walk, LOW);
  digitalWrite(light->wait, HIGH);
}


// Sequence of intersection states and how many milliseconds they corespond to
// 0- Start: main st is green, side streets red, top left & bottom right wait, top right & bottom left walk
// Wait 10 seconds (10000)
// 1- Intersection 1 main light turns yellow
// Top right & bottom left blink red
// Wait 5 seconds (5000) 15000ms
// 2- Intersection 1 main light turns red
// Top right & bottom left wait
// Wait 1 second (1000) 16000ms
// 3- Intersection 1 side light turns green
// Top left & bottom right walk
// Wait 10 seconds (10000) 26000ms
// 4- Intersection 1 side light turns yellow
// Top left & bottom right blink red
// Wait 5 seconds (5000) 31000ms
// 5- Intersection 1 side light turns red
// Top left & bottom right wait
// Wait 1 second (1000) 32000ms
// Restart
/*
Query an intersection for state change. If the given intersection state change condition is met, the intersection will change state
@param i - An intersection to check
@param delay - The delay in milliseconds between intersections
*/
void intersectionController(int i) {
  //Starts at state 0

  // How long in milliseconds the program has been running
  currentMillis = millis();

  // Serial.print("Current millis: ");
  // Serial.println(currentMillis);
  // Serial.print("Previous millis: ");
  // Serial.println(previousMillis[i]);
  //Millis of each intersection since the loop began for that intersection began
  int millisAfterDelay = ((currentMillis - previousMillis[i]) - (i * intersectionSwitchDelay));
  // Serial.print("Millis after delay: ");
  // Serial.println(millisAfterDelay);
  if (millisAfterDelay >= 0) {

    //Blink walk light
    if (intersectionState[i] == 1 && ((millisAfterDelay % 1000) <= millisBuffer)) {
      changeToWalk(intersectionsArray[i]->walkTopRight);
      changeToWalk(intersectionsArray[i]->walkBottomLeft);
    } else if (intersectionState[i] == 1 && (millisAfterDelay % 500 <= millisBuffer) && (millisAfterDelay % 1000 > millisBuffer)) {
      changeToWait(intersectionsArray[i]->walkTopRight);
      changeToWait(intersectionsArray[i]->walkBottomLeft);
    }

    //Blink second walk light
    if (intersectionState[i] == 4 && ((millisAfterDelay % 1000) <= millisBuffer)) {
      changeToWalk(intersectionsArray[i]->walkTopLeft);
      changeToWalk(intersectionsArray[i]->walkBottomRight);
    } else if (intersectionState[i] == 4 && (millisAfterDelay % 500 <= millisBuffer) && (millisAfterDelay % 1000 > millisBuffer)) {
      changeToWait(intersectionsArray[i]->walkTopLeft);
      changeToWait(intersectionsArray[i]->walkBottomRight);
    }

    //State 1
    if (intersectionState[i] == 0 && millisAfterDelay > millisBuffer && ((millisAfterDelay % 10000) <= millisBuffer)) {
      // Serial.println("State 1 ============================");
      // Serial.print("Current millis: ");
      // Serial.println(currentMillis, DEC);
      intersectionState[i] = 1;
      changeToYellow(intersectionsArray[i]->mainRoadLight);
      return;
    }

    //State 2
    else if (intersectionState[i] == 1 && ((millisAfterDelay % 15000) <= millisBuffer)) {
      // Serial.println("State 2 ============================");
      // Serial.print("Current millis: ");
      // Serial.println(currentMillis, DEC);
      intersectionState[i] = 2;
      changeToRed(intersectionsArray[i]->mainRoadLight);
      changeToWait(intersectionsArray[i]->walkTopRight);
      changeToWait(intersectionsArray[i]->walkBottomLeft);
      return;

    }

    //State 3
    else if (intersectionState[i] == 2 && ((millisAfterDelay % 16000) <= millisBuffer)) {
      // Serial.println("State 3 ============================");
      // Serial.print("Current millis: ");
      // Serial.println(currentMillis, DEC);
      intersectionState[i] = 3;
      changeToGreen(intersectionsArray[i]->sideRoadLight);
      changeToWalk(intersectionsArray[i]->walkTopLeft);
      changeToWalk(intersectionsArray[i]->walkBottomRight);
      return;

    }

    //State 4
    else if (intersectionState[i] == 3 && ((millisAfterDelay % 26000) <= millisBuffer)) {
      // Serial.println("State 4 ============================");
      // Serial.print("Current millis: ");
      // Serial.println(currentMillis, DEC);
      intersectionState[i] = 4;
      changeToYellow(intersectionsArray[i]->sideRoadLight);
      return;
    }

    //State 5
    else if (intersectionState[i] == 4 && ((millisAfterDelay % 31000) <= millisBuffer)) {
      // Serial.println("State 5 ============================");
      // Serial.print("Current millis: ");
      // Serial.println(currentMillis, DEC);
      intersectionState[i] = 5;
      changeToRed(intersectionsArray[i]->sideRoadLight);
      changeToWait(intersectionsArray[i]->walkTopLeft);
      changeToWait(intersectionsArray[i]->walkBottomRight);
      return;
    }

    //State 0
    else if (intersectionState[i] == 5 && (millisAfterDelay % 32000) <= millisBuffer) {
      // Serial.println("State 0 ============================");
      // Serial.print("Current millis: ");
      // Serial.println(currentMillis, DEC);
      intersectionState[i] = 0;
      //Only set previousMillis when the state restarts to 0
      previousMillis[i] = currentMillis;
      changeToGreen(intersectionsArray[i]->mainRoadLight);
      changeToWalk(intersectionsArray[i]->walkTopRight);
      changeToWalk(intersectionsArray[i]->walkBottomLeft);
      return;
    }
  }
}


// Ignore below /////////////////////
/*
Slide servo motor "out" (clockwise)
@param servoNum - the servo number you want to move
*/
// uint8_t slideOut(int servoNum) {
//   for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
//     pwm.setPWM(servoNum, 0, pulselen);
//   }
// }




/*
Slide servo motor "in" (counter-clockwise)
@param servoNum - the servo number you want to move
*/
// uint8_t slideIn(int servoNum) {
//   for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
//     pwm.setPWM(servoNum, 0, pulselen);
//   }
// }
