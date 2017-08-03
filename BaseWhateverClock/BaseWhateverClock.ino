// This program assumes the use of tri-color LEDs, one per digit.
// One lead provides voltage, (which can be wired externally),
// while the other three leads each, when set LOW allow for red, 
// green, or blue lighting, even simultaneously. 
// 
// While the LED I have technically allows all three colors at the 
// same time, it seems safer to power each color independently 
// (i.e., during separate clock cycles). The frequency can be high
// enough that it will not be a problem for our eyes.

#include "defines.h"

// Maximum unsigned long is 4,294,967,295
unsigned long initialMillis;
unsigned long previousMinute;
unsigned long currentMinute;
unsigned short digits[MAX_DIGITS];
unsigned short currentColor;
unsigned long elapsedMillis;
unsigned long microsThisInterval;
unsigned long currentSecond;
unsigned long frequencyCutoff_R[CLOCK_BASE];
unsigned long frequencyCutoff_G[CLOCK_BASE];
unsigned long frequencyCutoff_B[CLOCK_BASE];
unsigned long microsPerInterval = PRECISION_FACTOR * MICROS_PER_SECOND / FREQUENCY_IN_Hz;

void setup() {
  // put your setup code here, to run once:
  initialMillis = millis();
  setAllDigitalPinsOut();
  setAllLightsOff();
  for(int i = 0; i < CLOCK_BASE; i++) {
    digits[CLOCK_BASE] = 0;
  }
  currentColor = COLOR_BLUE;
  computeIntensityCutoffValues();

  Serial.begin(115200);
}

void loop() {
  // Code within 'loop' runs repeatedly:

//  microsThisInterval = (PRECISION_FACTOR * micros()) / FREQUENCY_IN_Hz;
  microsThisInterval = PRECISION_FACTOR * micros();

  // Determine seconds running; transitions at minute
  elapsedMillis = millis() - initialMillis;
  currentSecond = elapsedMillis / MILLIS_PER_SECOND;
  if(currentSecond > SECONDS_PER_MINUTE){
    currentMinute = currentSecond / SECONDS_PER_MINUTE;
    if(currentMinute > previousMinute) {
      previousMinute = currentMinute;
      determineDigits(currentMinute);  
    }
  } else {
    // Indicate seconds for the first minute (let's me know it's working if zero-digit equals zero-intensity)
    determineDigits(currentSecond);  
  }
  
  shineLights();
}

void setAllLightsOff() {
  for(int i = 0; i < MAX_DIGITS; i++) {
    digitalWrite(PIN_R[i], OFF);
    digitalWrite(PIN_B[i], OFF);
    digitalWrite(PIN_G[i], OFF);
  }
}

void setAllRedOff() {
  for(int i = 0; i < MAX_DIGITS; i++) {
    digitalWrite(PIN_R[i], OFF);
  }
}

void setAllBlueOff() {
  for(int i = 0; i < MAX_DIGITS; i++) {
    digitalWrite(PIN_B[i], OFF);
  }
}

void setAllGreenOff() {
  for(int i = 0; i < MAX_DIGITS; i++) {
    digitalWrite(PIN_G[i], OFF);
  }
}

void setAllDigitalPinsOut() {
  for(int i = 0; i < MAX_DIGITS; i++) {
    pinMode(PIN_R[i], OUTPUT);
    pinMode(PIN_B[i], OUTPUT);
    pinMode(PIN_G[i], OUTPUT);
  }
}

void determineDigits(unsigned long minutes) {
  unsigned long remainingMinutes = minutes;
  for(int i = 0; i < MAX_DIGITS; i++) {
    digits[i] = remainingMinutes % CLOCK_BASE;
    remainingMinutes = remainingMinutes / CLOCK_BASE;
  }
} 

void shineLights() {
  currentColor++;
  currentColor = currentColor % 3;
  // Shine only one color per loop; cycle through colors
  switch(currentColor) {
    case COLOR_RED:
      setAllGreenOff(); // previous was green
      for(int i = 0; i < MAX_DIGITS; i++) {
        digitalWrite(
          PIN_R[i], 
#if ON_DEMAND_COMPUTATION
          shineThisCycle(FREQUENCY_R[digits[i]])
#else
          (microsThisInterval % microsPerInterval) < frequencyCutoff_R[digits[i]]
#endif
          ? ON : OFF
        );
      }
      break;
    case COLOR_BLUE:
      setAllRedOff(); // previous was red
      for(int i = 0; i < MAX_DIGITS; i++) {
        digitalWrite(
          PIN_B[i], 
#if ON_DEMAND_COMPUTATION
          shineThisCycle(FREQUENCY_B[digits[i]])
#else
          (microsThisInterval % microsPerInterval) < frequencyCutoff_B[digits[i]]
#endif
          ? ON : OFF
        );
      }
      break;
    case COLOR_GREEN:
      setAllBlueOff(); // previous was blue
      for(int i = 0; i < MAX_DIGITS; i++) {
        digitalWrite(
          PIN_G[i], 
#if ON_DEMAND_COMPUTATION
          shineThisCycle(FREQUENCY_G[digits[i]])
#else
          (microsThisInterval % microsPerInterval) < frequencyCutoff_G[digits[i]]
#endif
          ? ON : OFF
        );
      }
      break;
  }
}

boolean shineThisCycle(unsigned short intensity){
  boolean bShine = false;  
  if(intensity > 0) {
    bShine = (microsThisInterval % microsPerInterval) < computeOnDemand(intensity);
  }
  return bShine;
}

void computeIntensityCutoffValues() {
  for( int i = 0; i < CLOCK_BASE; i++ ){
    frequencyCutoff_R[i] = computeOnDemand(FREQUENCY_R[i]);
    frequencyCutoff_G[i] = computeOnDemand(FREQUENCY_G[i]);
    frequencyCutoff_B[i] = computeOnDemand(FREQUENCY_B[i]);
  }
}

unsigned long computeOnDemand(unsigned short intensity) {
  return (intensity * microsPerInterval) / MAX_INTENSITY;
}


