// Define the base
const unsigned int CLOCK_BASE = 5;
const unsigned long MAX_DIGITS = 4; // I can only fit 4 LEDs (with 3 leads each) on board

const unsigned long PRECISION_FACTOR  = 1000; // one zero per point of precision; 3 --> 1000
const unsigned long SECONDS_PER_MINUTE = 60;
const unsigned long MILLIS_PER_SECOND = 1000;
const unsigned long MICROS_PER_SECOND = 1000000;
const unsigned long PICOS_PER_SECOND  = 1000000000;

const int COLOR_RED = 0;
const int COLOR_GREEN = 1;
const int COLOR_BLUE = 2;


// List the color frequency associated with each digit; same per LED
const short FREQUENCY_R[CLOCK_BASE] = {  0,   0,  0,  31,  31};
const short FREQUENCY_G[CLOCK_BASE] = {  0,  31,  0,   0,   0};
const short FREQUENCY_B[CLOCK_BASE] = {  0,   0,  31,  0,  31};
const short FREQUENCY_IN_Hz = 50;

const int PRINT_MODULUS_AMOUNT = 100;
const int MAX_INTENSITY = 255;

// List the pins associate with different LED output leads
const short PIN_R[MAX_DIGITS] = { 2,  5, 10, 11};
const short PIN_B[MAX_DIGITS] = { 3,  6,  9, 12};
const short PIN_G[MAX_DIGITS] = { 4,  7,  8, 13};

#define ON_DEMAND_COMPUTATION 0
//#define ON_DEMAND_COMPUTATION 1

// Define what means on/off so this can be changed quickly
#define USING_PRO_MINI 1
#if USING_PRO_MINI
  #define ON LOW
  #define OFF HIGH
#else
  #define ON HIGH
  #define OFF LOW
#endif

