/*
Embedded System Midterm for Classes CDA4630 and CDA6316
Based on ESP32 technology, NodeMCU-32S
Dr. Bassem Alhalabi
Florida Atlantic Univercity, Boca Raton
Update: 2023-0928

Modified by Alain Padron for Embedded Systems Fall 2024
*/

// include files
#include <ESP32Servo.h> // Files downloaded from library manager

// class obects
Servo myservo; // using class servo to create servo object, myservo, to control a servo motor

// ---------- defining pins for inputs
const int servoPin = 23; // servo pin
const int distPin = 35; // ir analog pin
const int lightPin = 36; // light sensor pins
const int touchPin = 34; // touch sensor pin
const int tempPin = 39; // temperature sensor pin

// ---------- defining pins for outputs
const int touchLed = 25; // touch green led
const int lightLed = 32; // ligh blue led
const int lightPwmLed = 22; // light yellow led - pwm
const int tempLed = 33; // temperature red led

// ---------- sensor values
int touchVal = 0, baseTouchVal=0;
int lightVal = 0, baseLightVal = 0, pwmVal=0, mapPwmVal=0;
int tempVal = 0, baseTempVal = 0;
int distVal = 0, baseDistVal=0, mapDistVal =0;
int flag=0;


// ---------- Main functions


// setup function: init all connections
void setup()
{
  // Set up ADC resolution to 10 bits: will get values between 0-1024
  analogReadResolution(10);

  // connect servo myservo to servoPin
  myservo.attach(servoPin);

  // set up analog pin as inputs
  pinMode(distPin, INPUT);
  pinMode(lightPin, INPUT);
  pinMode(touchPin, INPUT);
  pinMode(tempPin, INPUT);

  // set output pins
  pinMode(touchLed, OUTPUT);
  pinMode(lightLed, OUTPUT);
  pinMode(tempLed, OUTPUT);
  pinMode(lightPwmLed, OUTPUT);

  // set up default values
  digitalWrite(touchLed, HIGH);
  digitalWrite(lightLed, HIGH);
  digitalWrite(tempLed, HIGH);
  digitalWrite(lightPwmLed, HIGH);

  delay(1000); // wait for 1000 ms
  myservo.write (180);

  delay(1000); // wait for 1000 ms
  myservo.write (0);

  delay(1000); // wait for 1000 ms
  digitalWrite(touchLed, LOW);
  digitalWrite(lightLed, LOW);
  digitalWrite(tempLed, LOW);
  digitalWrite(lightPwmLed, LOW);

  delay(1000); // wait for 1000 ms
  // start the serial port
  Serial.begin(115200);
  // read sensors values and save them as baseline values
  baseLightVal = analogRead(lightPin); // room light
  baseTempVal = analogRead(tempPin); // room temperature
  baseTouchVal = analogRead(touchPin); // room touch
  baseDistVal = analogRead(distPin); // room distance
}

void loop()
{


  // ---------- distance sensor controls


  // read the distance sensor, set the min/max limits, and map to full range 0-180
  distVal = analogRead(distPin); // 0 - 1023
  distVal = min (distVal, 1000);
  distVal = max (distVal, 250);
  mapDistVal = map (distVal, 250, 1000, 0, 180); // map values between in range (250, 1010) to values in range (0, 180)
  myservo.write (mapDistVal);

  // ********** requirement # 1
  // the code above causes the servo to sweep from 0-90 as the distVal sweeps from 250-1000 as you vertically move your hand up/down over the distance sensor
  // modify the code so that if you move your hand horizantally quickly the servo stops in its last position
  /* enter your comments here:



  */


  // ---------- light sensor controls


  // read light sensor
  lightVal = analogRead(lightPin); // light sensor

  // The following two lines designate a deadzone band for the blue led.
  if ( (lightVal > (baseLightVal - (0.1 * baseLightVal) ) ) )  // if incoming light is above the 90% threshold, turn LED on
    { digitalWrite(lightLed, HIGH); }
  else if ( (lightVal < (baseLightVal - (0.9 * baseLightVal) ) ) )  // if incoming light is below the 10% threshold, turn LED off
    { digitalWrite(lightLed, LOW); }
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
  pwmVal = min(pwmVal, baseLightVal); // allow pwmVal value to be max at room light
  pwmVal = max(pwmVal, 100); // allow pwmVal value to be min is 10
  mapPwmVal = map(pwmVal, 100, baseLightVal, 245, 10); 
  analogWrite(lightPwmLed, mapPwmVal);
  // ********** requirement #3
  // modify the code above to reverse the effect of dimming, that is when you cover the light sensors the yellow led should be at brightes
  /* enter your comments here:

  Swapping the 245 and 10 values will reverse the dimming effect. This is due to how th map function works.
  The map function maps the value of pwmVal from the range of 100 to baseLightVal to the range of 245 to 10, which means
  that the higher the value of pwmVal, the lower the value of mapPwmVal. This will cause the LED to be brighter when the incoming light is lower.

  */


  // ---------- touch sensor controls


  // read the touch sensor, and if touched, turn on the green led, when no touch turn led off
  touchVal = analogRead(touchPin);
  if (touchVal < (0.9 * baseTouchVal) ) // using 90% of the baseline value as the threshold
    { digitalWrite(touchLed, HIGH); } // if touched
  else
    { digitalWrite(touchLed, LOW); }
  // ********** requirement #4
  // the code above uses an absolute value to detect touching which may not be suitable for your skin
  // change that value to a percentage of the baseline value based on the actual reading when you touch
  /* enter your comments here:



  */
  // ********** requirement #5
  // change the code so that every time you touch, the led will toggle, and when you untouch, led stays the same. hit: use a flag
  /* enter your comments here:



  */


  // ---------- temperature sensor controls


  // read the temo sensor, and if temp higher than the baseline, turn red led on, else turn led off
  // you can increase the sensor temo by touching/sqweezing it between your thinp and index fingure
  tempVal = analogRead(tempPin); // tmp sensor
  if (tempVal > 1.05 * baseTempVal) { digitalWrite(tempLed, HIGH); }
  else { digitalWrite(tempLed, LOW); }
  // if you have skin temperature that is less than the room basline temp, the led will not turn on
  // in this case you need to change the code so that if the temperature is higher/ lower than the baseline, the led will turn on
  // ********** requirement #6
  // change the code and apply the deadzone band concept instead od threshold value, so that the led will stop flickering
  /* enter your comments here:



  */


  // ---------- printing on serial monitor


  Serial.print("Dist base value: "); Serial.print(baseDistVal); Serial.print("Dist map value: "); Serial.println(distVal);
  Serial.print("light base value: "); Serial.print(baseLightVal); Serial.print("light Reading: "); Serial.println(lightVal);
  Serial.print("light PWD Value: "); Serial.print(pwmVal); Serial.print("map PWD Value: "); Serial.println(mapPwmVal);
  Serial.print("Touch base value: "); Serial.print(baseTouchVal); Serial.print("Touch Reading: "); Serial.println(touchVal);
  Serial.print("Temp base value: "); Serial.print(baseTempVal); Serial.print("Temp Reading: "); Serial.println(tempVal);


  // ---------- the mian loop dealy


  // here we are usning a loop delay to slow down the update rate of the analoginput
  delay(100); // wait for 100ms, the invinit loop interval
  // ********** requirement #7
  // change the delay value (10, 100, 500 1000), and state you observation on the various loop delays
  /* enter your comments here:


*/
}