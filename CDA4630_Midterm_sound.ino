/*
Embedded System Midterm for Classes CDA4630 and CDA6316
Based on ESP32 technology, NodeMCU-32S
Dr. Bassem Alhalabi
Florida Atlantic Univercity, Boca Raton
Update: 2023-0928


Modified by Alain Padron
CDA 4630 Midterm
Fall 2024 
*/

// include files
// Install ESP32Servo from library manager
#include <ESP32Servo.h>

// class obects
Servo myservo;  // using class servo to create servo object, myservo, to control a servo motor


// ---------- defining pins for inputs 
const int servoPin = 23;    // servo pin
const int distPin = 35;       // ir analog pin
const int trigPin = 5;      // trigger pin
const int echoPin = 17;      // echo pin
const int lightPin = 36;    // light sensor pins
const int touchPin = 34;    // touch sensor pin
const int tempPin = 39;     // temperature sensor pin

// ---------- defining pins for outputs 
const int touchLed = 25;      // touch green led
const int lightLed = 32;      // ligh blue led
const int lightPwmLed = 22;   // light yellow led - pwm
const int tempLed = 33;       // temperature red led

//  ---------- sensor values
int touchVal = 0, baseTouchVal = 0;
int lightVal = 0, baseLightVal = 0, pwmVal = 0, mapPwmVal = 0;
int tempVal = 0,  baseTempVal = 0;
int distVal = 0,  baseDistVal = 0,  mapDistVal = 0;

float duration_us = 0;
float distance_cm = 0;
bool irSensor = false; //toggle these two sensor to write values to servo
bool ulSensor = true;

/*

Set ulSensor to true to use the ultrasonic sensor, and irSensor to false since it will not be used.

*/



// ---------- Main functions 
// setup function: init all connections
void setup() 
{
  // Set up ADC resolution to 10 bits: will get values between 0-1024
  analogReadResolution(10);  
  // connect servo myservo to servoPin
  myservo.attach(servoPin);
  
  // set up analog pin as inputs
  // pinMode(distPin, INPUT);
  pinMode(echoPin, INPUT);
  pinMode(lightPin, INPUT);
  pinMode(touchPin, INPUT);
  pinMode(tempPin, INPUT);

  // set output pins
  pinMode(trigPin, OUTPUT);
  pinMode(touchLed, OUTPUT);
  pinMode(lightLed, OUTPUT);
  pinMode(tempLed, OUTPUT);
  pinMode(lightPwmLed, OUTPUT);
 
  // set up default values
  digitalWrite(touchLed, HIGH);
  digitalWrite(lightLed, HIGH);
  digitalWrite(tempLed, HIGH);
  digitalWrite(lightPwmLed, HIGH);
  delay(1000);  // wait for 1000 ms

  myservo.write(180);
  delay(1000);  // wait for 1000 ms
  myservo.write(0);
  delay(1000);  // wait for 1000 ms

  digitalWrite(touchLed, LOW);
  digitalWrite(lightLed, LOW);
  digitalWrite(tempLed, LOW);
  digitalWrite(lightPwmLed, LOW);
  delay(1000);  // wait for 1000 ms


  // start the serial port
  Serial.begin(115200);

  // read sensors values and save them as baseline values
  baseLightVal = analogRead(lightPin);  // room light
  baseTempVal =  analogRead(tempPin);   // room temperature
  baseTouchVal = analogRead(touchPin);  // room touch
  baseDistVal = analogRead(distPin);  // room touch
  
}

// Declare the maxDistance and minDistance values to 25 cm and 5 cm respectively.
// float maxDistance_cm = 100;
// float minDistance_cm = 5;
// float mappedRotation = 0;

// created rotation value for requirement #1
float mappedRotation = 0;

// created flags for requirement #5
bool ledState = LOW;
bool lastTouch = false;

void loop() 
{
  // // ---------- distance sensor controls
  // // read the distance sensor, set the min/max limits, and map to full range 0-180 
  // distVal = analogRead(distPin);  // 0 - 1023
  // distVal = min (distVal, 1000);
  // distVal = max (distVal, 250);
  // mapDistVal = map (distVal, 250, 1000, 0, 180);  // map values between in range (250, 1010) to values in range (0, 180)

  // if (irSensor == true){
  //   myservo.write(mapDistVal);
  // }

  // ---------- ultrasonic sensor controls
  // generate 10-microsecond pulse to TRIG pin
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // measure duration of pulse from ECHO pin
  duration_us = pulseIn(echoPin, HIGH);

  // calculate the distance
  distance_cm = 0.017 * duration_us;

  // Control servo based on distance
  if (ulSensor == true) {
    if (distance_cm <= 30) {
      mappedRotation = map(distance_cm, (float)5, (float)25, 0, (float)180); // turn servo between 0 to 180 degress based on distance from 5 to 25 cm
      myservo.write(mappedRotation);
    } 

  }
  
  // ********** requirement # 1
  // the code above causes the servo to sweep from 0-180 degrees as the distance increases for the ultra sonic sensor
  // modify the code so that if you move your hand horizontally quickly the servo stops in its last position
  /* enter your comments here:

  Commented out the distVal, mapDistVal and irSensor code since it is not being used.

  The code was modified to only move the servo if the distance is less than or equal to 30 cm.
  This means that any distances past 30 will be cause the servo to move. This is so that if an object or hand quickly moves out of frame,
  and the new distance is the ceiling, it wont cause the sensor to move back to 180 degrees.
  
  A mapping function is then used to map a distance from 5cm to 25cm in front of the sensor to a servo rotation from 0 to 180 degrees.
  This is because the sensor can only reliably measure distances from 3cm to around 30 cm.
  Any more or less and the sensor will not reliably measure the distance.

  */


  // ---------- light sensor controls
  // read light sensor
  lightVal = analogRead(lightPin);  // light sensor
  // The following two lines designate a deadzone band for the blue led.
  if ( (lightVal > (baseLightVal - (0.1 * baseLightVal) ) ) ) {  // if incoming light is above the 90% threshold, turn LED on
    digitalWrite(lightLed, HIGH);
  }
  else if ( (lightVal < (baseLightVal - (0.8 * baseLightVal) ) ) ) { // if incoming light is below the 10% threshold, turn LED off
    digitalWrite(lightLed, LOW);
  }
  // observe when the blue led turns off and on as you move your finger up and down above the light sensor
  
  // ********** requirement #2
  // change the code above to expand the range from 0.3-0.5 to the max and explore the changes.
  /* enter your comments here:

    Modified the code to use a high threashold of 90% to turn the LED on and a low threshold of 10% to turn the LED off.
    This means that when the incoming light is greater than 90% of the base light value, the LED will turn on.
    When the incoming light is less than 10% of the base light value, the LED will turn off.

  */

  // ---------- pwm controls for the yellow led, where led dims from 100% to 0% based on the light value
  pwmVal = lightVal; // copy the light val
  pwmVal = min(pwmVal, baseLightVal); // constrain maximum of pwmVal to baseLightVal
  pwmVal = max(pwmVal, 100); // limit minimum of pwmVal to 100
  mapPwmVal = map(pwmVal, 100, baseLightVal, 245, 10); 
  analogWrite(lightPwmLed, mapPwmVal);
  // ********** requirement #3
  // modify the code above to reverse the effect of dimming, that is when you cover the light sensors the yellow led should be at brightes
  /* enter your comments here:

  Swapping the 245 and 10 values will reverse the dimming effect. This is due to how the map function works.
  The map function maps the value of pwmVal from the range of 100 to baseLightVal to the range of 245 to 10, which means
  that the higher the value of pwmVal, the lower the value of mapPwmVal. This will cause the LED to be brighter when the incoming light is lower.

  */


  // ---------- touch sensor controls
  // read the touch sensor, and if touched, turn on the green led, when no touch turn led off
  touchVal = analogRead(touchPin);
  bool beingTouched = touchVal < (0.9 * baseTouchVal); // TRUE = being touched, FALSE = not being touched

  if (beingTouched != lastTouch) { // check if current state is opposite of last state TRUE != FALSE, will run
    if (beingTouched) { // only when its being touched will the led change
      
      ledState = !ledState; // set led to opposite of its state HIGH -> LOW, LOW -> HIGH
      
      digitalWrite(touchLed, ledState);
    }
    lastTouch = beingTouched; // last touch state updates to current
  }
  // ********** requirement #4
  // the code above uses an absolute value to detect touching which may not be suitable for your skin
  // change that value to a percentage of the baseline value based on the actual reading when you touch
  /* enter your comments here:

  The code was modified to have incorporate the baseTouchVal value in the if statement. Essentially, as long as the touchVal is less than 90% of the baseTouchVal, the LED will turn on.

  */
  // ********** requirement #5
  // change the code so that every time you touch, the led will toggle, and when you untouch, led stays the same. hit: use a flag
  /* enter your comments here:

  To achieve this, flags were created outside of the loop to keep track of the LED state and the last touch state.
  These two flags would be used to compared the previous touch state to the current touch state. If the current touch state is different from the last touch state, the LED will toggle.

  */


  // ---------- temperature sensor controls
  // read the temo sensor, and if temp higher than the baseline, turn red led on, else turn led off
  // you can increase the sensor temo by touching/sqweezing it between your thinp and index fingure

  tempVal = analogRead(tempPin);  // tmp sensor
  if (tempVal > ( 1.04 * baseTempVal) ) { // 3% higher than the baseline value
    digitalWrite(tempLed, HIGH); 
  }
  else if (tempVal < ( 1.02 * baseTempVal) ) {
    digitalWrite(tempLed, LOW);
  }
  // if you have skin temperature that is less than the room basline temp, the led will not turn on
  // in this case you need to change the code so that if the temperature is higher/lower than the baseline, the led will turn on

  // ********** requirement #6 
  // change the code and apply the deadzone band concept instead of threshold value, so that the led will stop flickering 
  /* enter your comments here:

  No flickering occurs with RED led. The code was already modified to have a 5% threshold for the temperature sensor.

  */


  // ---------- printing on serial monitor
  Serial.println("Sensor Readings:");
  //Serial.print("IR Distance Sensor Base Value: ");  Serial.println(baseDistVal);
  //Serial.print("IR Value: "); Serial.println(distVal);
  //Serial.print("Run Time: "); Serial.print((float)millis()/1000.00); Serial.print("s"); Serial.print("\t| Lock Time: "); Serial.print((float)lockTime/1000.00); Serial.println("s");
  Serial.print("Distance Value: "); Serial.print((int)round(distance_cm)); Serial.print("cm\t| Servo Rotation: "); Serial.print(myservo.read()); Serial.println("°");
  Serial.print("Light Base Value: "); Serial.print(baseLightVal); Serial.print("\t| Light Reading: "); Serial.println(lightVal);
  Serial.print("Light PWM Value: "); Serial.print(pwmVal); Serial.print("\t| Mapped PWM Reading: "); Serial.println(mapPwmVal);
  Serial.print("Touch Base Value: "); Serial.print(baseTouchVal); Serial.print("\t| Touch Reading: "); Serial.println(touchVal);
  Serial.print("Temp Base Value: "); Serial.print(baseTempVal); Serial.print("\t| Temp Reading: "); Serial.println(tempVal);
  Serial.println("------------------------------------------------------------------");

  /*
  
  Adjusted the print statements to look more organized and easier to read on the Arduino Serial Monitor.
  Also commented out the IR distance values since they are not used.
  
  */



  // ---------- the main loop delay
  // here we are usning a loop delay to slow down the update rate of the analog input
  delay(100);  // wait for 100ms, the invinit loop interval 

  // ********** requirement #7
  // change the delay value (10, 100, 500 1000), and state you observation on the various loop delays
  /* enter your comments here:

  The larger the delay value, the slower the loop will run. This will cause the servo to move slower and the LED's to react slower.
  The opposite is true for smaller delay values. The servo will move faster and the LED's will react faster.
  However, 100ms is an optimal value for this loop as it allows the servo to move at a reasonable speed and the LED's to react quickly to changes in sensor values.

  */

}