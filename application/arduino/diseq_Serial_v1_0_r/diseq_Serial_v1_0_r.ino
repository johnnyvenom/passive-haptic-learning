#define __DEBUG_SOFTPWM__ 0
#include <SoftPWM.h>
#include <SPI.h>
#include <Wire.h>
#include <SFE_LSM9DS0.h>

/* TO TEST THE MOTORS THE MESSAGE IS:
/* [ motor(0-5) attack(ms) sustain(ms) amplitude(0-255) decay(ms) 
/* [0 1000 2000 255 1000]
/* motors are arranged counter-clockwise
 
/*
diseq_v6_2

v6_3 all imu code is commented out

v6_2
added serial messages for testing IMUs. Serial messages '1'-'6' will trigger individual motors, '7'-'9' will trigger patterns utilizing all motors.

v6
Added serial communication flags - 
serial messages to enable serial print:
'p' enables SPI
'a' enables accel
'm' enables PWM
's' enables SR
'o' disables all 
'i' enables raw spi printing

Based on rev_v4D

Motors:
PWM Pins 6, 7, 3, 4, 2, 5
Driver Pin Bs: SR 1, 2, 7, 6, 5, 4
Driver Pin A: SR 0

LEDs:
Pins 8, 9

Shift Register:
14 SD
15 SR_RST
16 SCK1
17 LATCH

Maximum length of envelope sections is 5000ms.
Maximum value for amplitude is 255.

*/


///////////////////////
// Example I2C Setup //

#define LSM9DS0_XM  0x1D // Would be 0x1E if SDO_XM is LOW
#define LSM9DS0_G   0x6B // Would be 0x6A if SDO_G is LOW
LSM9DS0 dof(MODE_I2C, LSM9DS0_G, LSM9DS0_XM);

#define PRINT_RAW
#define PRINT_SPEED 500 // 500 ms between prints

#define TESTING 0
/******************IMU Variables***********************************************/

// Timers
unsigned long sensorTimer = 0;
unsigned long motorTimer  = 0;
int sensorInterval = 5000; // interval between sensor readings, in microseconds
int motorInterval  = 10;

int accel_val[] = {0, 0, 0};
int gyro_val[]  = {0, 0, 0};
int mag_val[]   = {0, 0, 0};

byte all_imu_data[50] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

int cycleCount = 0;
float IMU_scalar = 0.95; // lowpass filter for IMU values - value 0f 0 = no lpf, val of 1 = new input ignored.

/*****************************************************************

*****************************************************************/

unsigned long PWMwriteTimer=0;
int PWMwriteInterval = 10;

SOFTPWM_DEFINE_CHANNEL( 2, DDRD, PORTD, PORTD2 );
SOFTPWM_DEFINE_CHANNEL( 3, DDRD, PORTD, PORTD3 );
SOFTPWM_DEFINE_CHANNEL( 4, DDRD, PORTD, PORTD4 );
SOFTPWM_DEFINE_CHANNEL( 5, DDRD, PORTD, PORTD5 );
SOFTPWM_DEFINE_CHANNEL( 6, DDRD, PORTD, PORTD6 );
SOFTPWM_DEFINE_CHANNEL( 7, DDRD, PORTD, PORTD7 );

SOFTPWM_DEFINE_OBJECT( 10 );
SOFTPWM_DEFINE_EXTERN_OBJECT( 10 );

int act_index = 0; //only used for assinging values to arrays
unsigned int a_t [17]; //attack
unsigned int attacks[17];
unsigned int s_t [17]; //sustain
unsigned int s_i[17];  //intensity / amplitude
unsigned int act_flag [17]; //actuator state flag
unsigned int d_t[17]; //decay

long onset[17]; //beginning of actuator envelope
float cur_position[17]; //current location in actuator envelope
int cur_pwm[17]; //current output value
int prev_pwm[17];

int i, ii, jj; //var i may only be used in process_actuators function, unless you want to go through and rename all of those indexes!

//timers
unsigned int printInterval = 500;
unsigned long printTimer=0;
unsigned long timer = 0;
int count = 0;

//power led variables
unsigned long pwrTimer=0;
int pwrInterval = 500;
int pwrLED = 8;

//communication LED variables
unsigned long commTimer = 0;
int commInterval = 25;
int commLED = 9;
boolean commLEDflag = 0;
boolean prev_commLEDflag = 0;

long pwmInterval = 25;
long pwmTimer = 0;

//SPI Variables
volatile byte buf [500];
volatile byte c = 0;
volatile byte pos;
volatile boolean process_it;
byte junk = 0;

//SLIP VARIABLES
char escapeChar = 101;
char delimiterChar = 100;
char pollChar = 102;
char dummyChar = 103;
char initEnvChar = 104;
char initImuChar=105;
int esc_flag = 0;
int poll_flag = 0;
int poll_set = 0;

int enable_SPIprint = 0;
int enable_accelprint = 0;
int enable_PWMprint = 0;
int enable_SRprint = 0;
int enable_printrawSPI = 0;

int print_accel_flag=0;
int print_SPI_flag=0;

byte POLL_DEFAULT = 254; //imu data isn't ready yet

//PINS
int pins[6] = {7, 6, 5, 2, 4, 3};//right order motor PWM pins
//int pins[3]={3,5,6};
int pins_numb =0; //actuator specified in SPI message
int prev_pins_numb =0;

//error handling variables
int maxEnvTime = 9000;
int errorFlag=0;
unsigned long errorTimer=0;
int errorInterval = 50;

///////////

//Shift register variables
int latchPin = 17;
int clockPin = 16;
int resetPin = 15;
int dataPin = 14;

//int motorPinA[] = {3, 1, 2, 6, 0, 5}; //for motors 0,1,4,5 this indicates shift register pins, for 2&3 arduino digital pins
int motorPinA = 0;
int pinAflag = 0;
//int motorPinB[] = {1, 2, 7, 6, 5, 4}; //shift register pins 0-7
// CORRECT SHIFT REGISTER PINS FROM PREVIOUS VERSION
int motorPinB[] = {2, 1, 4, 5, 6, 7}; //shift register pins 0-7

int brakeInterval = 100;

byte motorSRstate=0;
byte prev_motorSRstate=0;

int motorState[] = {0,0,0,0,0,0}; //0 = stopped, 1 = running, 2 = brake
int prev_motorState[] = {0,0,0,0,0,0};


/******************************************
/
/
/
/
*******************************************/

void setup()
{

  
  Serial.begin(57600);
//  SoftPWMBegin();
    Palatis::SoftPWM.begin( 60 );
// uint16_t status = dof.begin();
   
  //initialize Shift Register
  pinMode(resetPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  digitalWrite(resetPin, HIGH);
  
  //INITIALIZING SPI
  pinMode(MISO, OUTPUT);   // have to send on master in, *slave out*
  SPCR |= _BV(SPE);   // turn on SPI in slave mode
  SPCR |= _BV(SPIE);   // turn on interrupts
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  
          // Clear previous run data
        
    junk = SPSR;
    junk = SPDR;
    delay(10);
    
  
  // get ready for an interrupt 
  pos = 0;   // buffer empty
  process_it = false;
  SPI.attachInterrupt();     // now turn on interrupts
  
    pinMode(pwrLED, OUTPUT);
  pinMode(commLED, OUTPUT);
  digitalWrite(pwrLED, HIGH);
  digitalWrite(commLED, HIGH);
  delay(500);
  //digitalWrite(pwrLED, LOW);
  digitalWrite(commLED, LOW);
  
//  readSensors();
//  print_accel();
}

/******************************************
/
/
/
/
*******************************************/

void loop()
{
  if (enable_accelprint) print_accel();
  if (print_SPI_flag==1) print_SPI();
  
//  readSensors(); //read sensors int data arrays
  //printSensors();  //serial print sensors
  process_actuators(); //generate PWM values for all actuators
  set_actuators(); //set PWM values of all actuators
 // test_motors(); //generate test data
  commLEDctrl(); //turns commLED on for SPI events
  errorLEDctrl();
  //check_serial();
}




/******************************************
///SPI INTERRUPT ROUTINE
/
/ copied from matthews motor_envelopes2.ino file
/
*******************************************/
ISR (SPI_STC_vect)
{
    c = SPDR;                                // grab byte from SPI Data Register
  if (enable_printrawSPI)  Serial.println(c);
    if (pos < sizeof(buf)) {
      
        if (esc_flag == 1) {                 // if prev byte was esc automatically write current byte to buffer
            buf[pos++] = c;
            esc_flag = 0;
        } else if (c == initImuChar) {          // if polling for IMU data
              count=6;
              SPDR = all_imu_data[count];
            //  Serial.println(all_imu_data[count]);
              count++;
        } else if (c==dummyChar)
        {
          //do nothing
        } else if (c == initEnvChar) {          // get ready for new envelope data
              pos=0;
        } else if ( c == pollChar) {
              SPDR = all_imu_data[count];
            //  Serial.println(all_imu_data[count]);
              count++;
        } else if (c == escapeChar) {        // next byte is good data
            esc_flag = 1;
        } else if (c == delimiterChar) {     // ends packet
            process_it = true;
            //process_SPI();
            
    /***process_SPI()****/  
    
      pins_numb = returnInt(buf[0],buf[1]); //WHICH ACTUATOR
      if ((pins_numb > 6) || (pins_numb < 0)) {
        pins_numb = prev_pins_numb;
        errorFlag=1;
      } else prev_pins_numb = pins_numb;
      
      if (enable_SPIprint) print_SPI_flag=1; //print
      
      //ENVELOPE PARAMETERS
      attacks[pins_numb] = returnInt(buf[2],buf[3]); 
      if ((attacks[pins_numb] > maxEnvTime) || (attacks[pins_numb] < 0)) {
        attacks[pins_numb] = attacks[16];
        errorFlag=1;
      } else attacks[16] = attacks[pins_numb];
      
      s_t[pins_numb] = returnInt(buf[4],buf[5]);
      if ((s_t[pins_numb] > maxEnvTime) || (s_t[pins_numb] < 0)) {
        s_t[pins_numb] = s_t[16];
        errorFlag=1;
      } else s_t[16] = s_t[pins_numb];
      
      s_i[pins_numb] = returnInt(buf[6],buf[7]); 
      if ((s_i[pins_numb] > 256) || (s_i[pins_numb] < 0)) {
        s_i[pins_numb] = s_i[16];
        errorFlag=1;
      } else s_i[16] = s_i[pins_numb];
      
      d_t[pins_numb] = returnInt(buf[8],buf[9]); 
      if ((d_t[pins_numb] > maxEnvTime) || (d_t[pins_numb] < 0)) {
        d_t[pins_numb] = d_t[16];
        errorFlag=1;
      } else d_t[16] = d_t[pins_numb];

      onset[pins_numb] = millis(); //ENVELOPE START TIME
      //actuatorStatus(pins_numb, 1); //FLAG - happens in motorshiftout now
      motor_shiftOut (pins_numb, 1);    
      
 /*******/
        }
       else {                             // writes incoming bytes to the buffer
            buf[pos] = c;
            pos++;
        }
    }
    commLEDflag = 1;
}

void print_accel()
{
  if (millis()>printTimer+printInterval)
  {
  Serial.print("Accelerometer: ");
  Serial.print(accel_val[0]);
  Serial.print(", ");
  Serial.print(accel_val[1]);
  Serial.print(", ");
  Serial.println(accel_val[2]);
  print_accel_flag=0;
  
    Serial.print("Accelerometer: ");
  Serial.print(accel_val[0]);
  Serial.print(", ");
  Serial.print(accel_val[1]);
  Serial.print(", ");
  Serial.println(accel_val[2]);
  print_accel_flag=0;
  
    all_imu_data[6] = topByte(accel_val[0]);
    all_imu_data[7] = bottomByte(accel_val[0]);
    all_imu_data[8] = topByte(accel_val[1]);
    all_imu_data[9] = bottomByte(accel_val[1]);
    all_imu_data[10] = topByte(accel_val[2]);
    all_imu_data[11] = bottomByte(accel_val[2]);
  }
}

void print_SPI()
{
      Serial.print("motor ");
      Serial.print(pins_numb);
      Serial.print(", ");
      Serial.print(attacks[pins_numb]);
      Serial.print(", ");
      Serial.print(s_t[pins_numb]);
      Serial.print(", ");
      Serial.print(s_i[pins_numb]);
      Serial.print(", ");
      Serial.println(d_t[pins_numb]);
      
      print_SPI_flag=0;
  
  
}


/******************************************
BYTE MANIPULATION

// Returns the concatenation of 2 bytes as 1 int.
// Params:
//    - byte a: Most significant byte
//    - byte b: Least significant byte
// Returns: int resulting from concatenation of byte a and b

*******************************************/

int returnInt(byte a, byte b)
{
    int val;
    val = a;
    val <<= 8;
    val |= b;
    return val;
}

// Returns the most significant 8 bits of an int as a byte.
// Params:
//    - int a: integer to decompose
// Returns: byte made from top 8 bits of int a

byte topByte(int a)
{
    return a >> 8;
}

// Returns the least significant 8 bits of an int as a byte.
// Param:
//    - int a: integer to decompose
// Returns: byte made from bottom 8 bits of int a
// Note: Test correct transmission of bytes by commenting first line of this function

byte bottomByte(int a)
{
    a = a << 8;
    return a >> 8;
}


//test functions


