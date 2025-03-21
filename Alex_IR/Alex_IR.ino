#include <serialize.h>

#include "packet.h"
#include "constants.h"
#include <math.h>
#include <avr/sleep.h>
#define PRR_TWI_MASK 0b10000000
#define PRR_SPI_MASK 0b00000100
#define ADCSRA_ADC_MASK 0b10000000
#define PRR_ADC_MASK 0b00000001
#define PRR_TIMER2_MASK 0b01000000
#define PRR_TIMER0_MASK 0b00100000
#define PRR_TIMER1_MASK 0b00001000
#define SMCR_SLEEP_ENABLE_MASK 0b00000001
#define SMCR_IDLE_MODE_MASK 0b11110001
void WDT_off(void)
{
  /* Global interrupt should be turned OFF here if not
  already done so */
  /* Clear WDRF in MCUSR */
  MCUSR &= ~(1<<WDRF);
  
  /* Write logical one to WDCE and WDE */
  /* Keep old prescaler setting to prevent unintentional
  time-out */
  WDTCSR |= (1<<WDCE) | (1<<WDE);
  
  /* Turn off WDT */
  WDTCSR = 0x00;
  
  /* Global interrupt should be turned ON here if
  subsequent operations after calling this function do
  not require turning off global interrupt */
}

void setupPowerSaving()
{
  // Turn off the WDT
  WDT_off();

  PRR |= PRR_TWI_MASK;
  PRR |= PRR_SPI_MASK;
  ADCSRA &= ~ADCSRA_ADC_MASK;
  PRR |= PRR_ADC_MASK;

  SMCR &= SMCR_IDLE_MODE_MASK;

  //DDRB |= 0b00100000;
  //PORTB &= 0b11011111;
  
}

void putArduinoToIdle()
{
  PRR |= PRR_TIMER0_MASK;
  PRR |= PRR_TIMER1_MASK;
  PRR |= PRR_TIMER2_MASK;

  SMCR |= SMCR_SLEEP_ENABLE_MASK;

  sleep_cpu();

  SMCR &= ~(SMCR_SLEEP_ENABLE_MASK);

  PRR &= ~(PRR_TIMER0_MASK);
  PRR &= ~(PRR_TIMER1_MASK);
  PRR &= ~(PRR_TIMER2_MASK);
}
typedef enum
{
  STOP = 0,
  FORWARD = 1,
  BACKWARD = 2,
  LEFT = 3,
  RIGHT = 4
} TDirection;
volatile TDirection dir = STOP;

/*
 * Alex's configuration constants
 */

// Number of ticks per revolution from the 
// wheel encoder.

#define COUNTS_PER_REV      192

// Wheel circumference in cm.
// We will use this to calculate forward/backward distance traveled 
// by taking revs * WHEEL_CIRC

#define WHEEL_CIRC          20.1

// Motor control pins. You need to adjust these till
// Alex moves in the correct direction

#define LF                  0b01000000   // Left reverse pin 5 port d
#define LR                  0b00100000   // Left forward pin 6 port d
#define RF                  0b00000100  // Right reverse pin 9 port b
#define RR                  0b00000010  // Right forward pin 10 port b
// Pi, for calculating turn circumference 
#define PI 3.141592654

// Alex's length and breadth in cm

#define ALEX_LENGTH 16.5
#define ALEX_BREADTH 12.5

float AlexDiagonal= 0.0;
float AlexCirc = 0.0;
/*
 *    Alex's State Variables
 */

// Store the ticks from Alex's left and
// right encoders.
volatile unsigned long leftForwardTicks; 
volatile unsigned long rightForwardTicks;
volatile unsigned long leftReverseTicks; 
volatile unsigned long rightReverseTicks;

volatile unsigned long leftForwardTicksTurns; 
volatile unsigned long rightForwardTicksTurns;
volatile unsigned long leftReverseTicksTurns; 
volatile unsigned long rightReverseTicksTurns;
// Store the revolutions on Alex's left
// and right wheels
volatile unsigned long leftRevs;
volatile unsigned long rightRevs;

// Forward and backward distance traveled
volatile unsigned long forwardDist;
volatile unsigned long reverseDist;

//Variables to keep track of whether we have moved a commanded distance
unsigned long deltaDist;
unsigned long newDist;

unsigned long deltaTicks;
unsigned long targetTicks;

/*
 * 
 * Alex Communication Routines.
 * 
 */
 
TResult readPacket(TPacket *packet)
{
    // Reads in data from the serial port and
    // deserializes it.Returns deserialized
    // data in "packet".
    
    char buffer[PACKET_SIZE];
    int len;

    len = readSerial(buffer);

    if(len == 0)
      return PACKET_INCOMPLETE;
    else
      return deserialize(buffer, len, packet);
    
}

void sendStatus()
{
  // Implement code to send back a packet containing key
  // information like leftTicks, rightTicks, leftRevs, rightRevs
  // forwardDist and reverseDist
  // Use the params array to store this information, and set the
  // packetType and command files accordingly, then use sendResponse
  // to send out the packet. See sendMessage on how to use sendResponse.
  //
  TPacket statusPacket;
  statusPacket.packetType=PACKET_TYPE_RESPONSE;
  statusPacket.command = RESP_STATUS;
  statusPacket.params[0] = leftForwardTicks;
  statusPacket.params[1] = rightForwardTicks;
  statusPacket.params[2] = leftReverseTicks;
  statusPacket.params[3] = rightReverseTicks;
  statusPacket.params[4] = leftForwardTicksTurns;
  statusPacket.params[5] = rightForwardTicksTurns;
  statusPacket.params[6] = leftReverseTicksTurns;
  statusPacket.params[7] = rightReverseTicksTurns;
  statusPacket.params[8] = forwardDist;
  statusPacket.params[9] = reverseDist;
  sendResponse(&statusPacket);
  
}
void sendStop(char dir){
  TPacket stopPacket;
  stopPacket.packetType=PACKET_TYPE_RESPONSE;
  stopPacket.command = RESP_STOP;
  stopPacket.data[0] = dir;
  sendResponse(&stopPacket);
}
void sendMessage(const char *message)
{
  // Sends text messages back to the Pi. Useful
  // for debugging.
  
  TPacket messagePacket;
  messagePacket.packetType=PACKET_TYPE_MESSAGE;
  strncpy(messagePacket.data, message, MAX_STR_LEN);
  sendResponse(&messagePacket);
}

void sendBadPacket()
{
  // Tell the Pi that it sent us a packet with a bad
  // magic number.
  
  TPacket badPacket;
  badPacket.packetType = PACKET_TYPE_ERROR;
  badPacket.command = RESP_BAD_PACKET;
  sendResponse(&badPacket);
  
}

void sendBadChecksum()
{
  // Tell the Pi that it sent us a packet with a bad
  // checksum.
  
  TPacket badChecksum;
  badChecksum.packetType = PACKET_TYPE_ERROR;
  badChecksum.command = RESP_BAD_CHECKSUM;
  sendResponse(&badChecksum);  
}

void sendBadCommand()
{
  // Tell the Pi that we don't understand its
  // command sent to us.
  
  TPacket badCommand;
  badCommand.packetType=PACKET_TYPE_ERROR;
  badCommand.command=RESP_BAD_COMMAND;
  sendResponse(&badCommand);

}

void sendBadResponse()
{
  TPacket badResponse;
  badResponse.packetType = PACKET_TYPE_ERROR;
  badResponse.command = RESP_BAD_RESPONSE;
  sendResponse(&badResponse);
}

void sendOK()
{
  TPacket okPacket;
  okPacket.packetType = PACKET_TYPE_RESPONSE;
  okPacket.command = RESP_OK;
  sendResponse(&okPacket);  
}

void sendResponse(TPacket *packet)
{
  // Takes a packet, serializes it then sends it out
  // over the serial port.
  char buffer[PACKET_SIZE];
  int len;

  len = serialize(buffer, packet, sizeof(TPacket));
  writeSerial(buffer, len);
}


/*
 * Setup and start codes for external interrupts and 
 * pullup resistors.
 * 
 */
// Enable pull up resistors on pins 2 and 3
void enablePullups()
{
  // Use bare-metal to enable the pull-up resistors on pins
  // 2 and 3. These are pins PD2 and PD3 respectively.
  // We set bits 2 and 3 in DDRD to 0 to make them inputs. 

  DDRD  &= 0b11110011;
  PORTD |= 0b00001100;
}

// Functions to be called by INT0 and INT1 ISRs.
void leftISR()
{
  
  if(dir == FORWARD){
    leftForwardTicks++;
    forwardDist = (unsigned long) ((float) leftForwardTicks / COUNTS_PER_REV * WHEEL_CIRC);
  }else if(dir == BACKWARD){
    leftReverseTicks++;
        reverseDist = (unsigned long) ((float) leftReverseTicks / COUNTS_PER_REV * WHEEL_CIRC);
  }else if(dir == LEFT){
    leftReverseTicksTurns++;
  }else if(dir == RIGHT){
    leftForwardTicksTurns++;
  }
  //leftRevs = leftTicks / COUNTS_PER_REV;

  // We calculate forwardDist only in leftISR because we
  // assume that the left and right wheels move at the same
  // time.
  //forwardDist = leftRevs * WHEEL_CIRC;
  
  //Serial.print("LEFT: ");
//  Serial.println(leftTicks);
}

void rightISR()
{

  if(dir == FORWARD){
    rightForwardTicks++;
  }else if(dir == BACKWARD){
    rightReverseTicks++;
  }else if(dir == LEFT){
    rightForwardTicksTurns++;
  }else if(dir == RIGHT){
    rightReverseTicksTurns++;
  }

  //rightRevs = rightTicks / COUNTS_PER_REV;
  //Serial.print("RIGHT: ");
 // Serial.println(rightTicks);
}

// Set up the external interrupt pins INT0 and INT1
// for falling edge triggered. Use bare-metal.
void setupEINT()
{
  // Use bare-metal to configure pins 2 and 3 to be
  // falling edge triggered. Remember to enable
  // the INT0 and INT1 interrupts.

  EICRA = 0b00001010;
  EIMSK = 0b00000011;
}

// Implement the external interrupt ISRs below.
// INT0 ISR should call leftISR while INT1 ISR
// should call rightISR.

ISR(INT0_vect)
{
  leftISR();
}

ISR(INT1_vect)
{
  rightISR();
}


// Implement INT0 and INT1 ISRs above.

/*
 * Setup and start codes for serial communications
 * 
 */
// Set up the serial connection. For now we are using 
// Arduino Wiring, you will replace this later
// with bare-metal code.
void setupSerial()
{
  // To replace later with bare-metal.
  Serial.begin(57600);
  /*unsigned int b;
  //b = (unsigned int) round(16000000/(16 * 57600)) – 1;
  b =  (unsigned int) round(16000000/(16*57600)) - 1;
  UBRR0H = (unsigned char) (b >> 8);
  UBRR0L = (unsigned char) b;
  UCSR0C = 0b00000110;
  UCSR0A = 0;*/
}

// Start the serial connection. For now we are using
// Arduino wiring and this function is empty. We will
// replace this later with bare-metal code.

void startSerial()
{
  // Empty for now. To be replaced with bare-metal code
  // later on.
  //UCSR0B = 0b00011000;
}

// Read the serial port. Returns the read character in
// ch if available. Also returns TRUE if ch is valid. 
// This will be replaced later with bare-metal code.

int readSerial(char *buffer)
{

  int count=0;
  while(Serial.available())
    buffer[count++] = Serial.read();
  //while((UCSR0A & 0b10000000) == 1)
    //buffer[count++] = UDR0;

  return count;
}

// Write to the serial port. Replaced later with
// bare-metal code

void writeSerial(const char *buffer, int len)
{
  Serial.write(buffer, len);
  /*for(int count = 0 ; count < sizeof(buffer) ; count++){
  while( (UCSR0A & 0b00100000) == 0)
    UDR0 = buffer[count] ;
  }*/
}

/*
 * Alex's motor drivers.
 * 
 */

// Set up Alex's motors. Right now this is empty, but
// later you will replace it with code to set up the PWMs
// to drive the motors.
void setupMotors()
{
  DDRB |= (RF | RR);
  DDRD |= (LF | LR);
  TCNT0 = 0;
  OCR0A = 0;
  OCR0B = 0;
  TIMSK0 |= 0b110; // OCIEA = 1 OCIEB = 1
  TCCR0B = 0b00000011;
  TCNT1 = 0;
  OCR1A = 0;
  OCR1B = 0;
  TIMSK1 |= 0b110; // OCIEA = 1 OCIEB = 1
  TCCR1B = 0b00000011;
}
static volatile int LFval;
static volatile int LRval;
static volatile int RFval;
static volatile int RRval;
static volatile int LFvalinit;
static volatile int LRvalinit;
static volatile int RFvalinit;
static volatile int RRvalinit;
 ISR(TIMER0_COMPA_vect)
{
OCR0A = LRval;
}
ISR(TIMER0_COMPB_vect)
{
OCR0B = LFval;
}
 ISR(TIMER1_COMPA_vect)
{
OCR1A = RFval;
}
ISR(TIMER1_COMPB_vect)
{
OCR1B = RRval;
}

// Set up Alex's motors. Right now this is empty, but
// later you will replace it with code to set up the PWMs
// to drive the motors.


// Start the PWM for Alex's motors.
// We will implement this later. For now it is
// blank.
void startMotors()
{
  
}

// Convert percentages to PWM values

int pwmVal(float speed)
{
  if(speed < 0.0)
    speed = 0;

  if(speed > 100.0)
    speed = 100.0;

  return (int) ( (speed / 100.0) * 255.0 );
}

// Move Alex forward "dist" cm at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// move forward at half speed.
// Specifying a distance of 0 means Alex will
// continue moving forward indefinitely.
void forward(float dist, float speed)
{
  if(dist > 0)
    deltaDist = dist;
  else
    deltaDist = 9999999;

  newDist = forwardDist + deltaDist;
  
  dir = FORWARD;
  int val = pwmVal(speed);

  // For now we will ignore dist and move
  // forward indefinitely. We will fix this
  // in Week 9.

  // LF = Left forward pin, LR = Left reverse pin
  // RF = Right forward pin, RR = Right reverse pin
  // This will be replaced later with bare-metal code.
  TCCR0A = 0b00100001;
  PORTD &= ~LR; //off LR
  LFval = val;
  LFvalinit = LFval;
  
  TCCR1A = 0b10000001;
  PORTB &= ~RR; //off RR
  RFval = val;
  RFvalinit = RFval;
  
}

// Reverse Alex "dist" cm at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// reverse at half speed.
// Specifying a distance of 0 means Alex will
// continue reversing indefinitely.
void reverse(float dist, float speed)
{
  if(dist > 0)
    deltaDist = dist;
  else
    deltaDist = 9999999;

  newDist = reverseDist + deltaDist;
  dir = BACKWARD;
  int val = pwmVal(speed);

  // For now we will ignore dist and 
  // reverse indefinitely. We will fix this
  // in Week 9.

  // LF = Left forward pin, LR = Left reverse pin
  // RF = Right forward pin, RR = Right reverse pin
  // This will be replaced later with bare-metal code.
  TCCR0A = 0b10000001;
  PORTD &= ~LF; //off LF
  LRval = val;
  LRvalinit = LRval;

  TCCR1A = 0b00100001;
  PORTB &= ~RF; //off RF
  RRval = val;
  RRvalinit = RRval;
}

// Turn Alex left "ang" degrees at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// turn left at half speed.
// Specifying an angle of 0 degrees will cause Alex to
// turn left indefinitely.
void left(float ang, float speed)
{
  if (ang == 0)
    deltaTicks = 99999999;
  else
    deltaTicks = computeDeltaTicks(ang);

  targetTicks = leftReverseTicksTurns + deltaTicks;
  dir = LEFT;
  int val = pwmVal(speed);

  // For now we will ignore ang. We will fix this in Week 9.
  // We will also replace this code with bare-metal later.
  // To turn left we reverse the left wheel and move
  // the right wheel forward.
  TCCR0A = 0b00100001;
  PORTD &= ~LR; //off LR
  LFval = val;
  LFvalinit = LFval;

  TCCR1A = 0b00100001;
  PORTB &= ~RF; //off RF
  RRval = val;
  RRvalinit = RRval;
}

// Turn Alex right "ang" degrees at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// turn left at half speed.
// Specifying an angle of 0 degrees will cause Alex to
// turn right indefinitely.
void right(float ang, float speed)
{
  if (ang == 0)
    deltaTicks = 99999999;
  else
    deltaTicks = computeDeltaTicks(ang);

  targetTicks = rightReverseTicksTurns + deltaTicks;
  dir = RIGHT;
  int val = pwmVal(speed);

  // For now we will ignore ang. We will fix this in Week 9.
  // We will also replace this code with bare-metal later.
  // To turn right we reverse the right wheel and move
  // the left wheel forward.
  TCCR0A = 0b10000001;
  PORTD &= ~LF; //off LF
  LRval = val;
  LRvalinit = LRval;

  TCCR1A = 0b10000001;
  PORTB &= ~RR; //off RR
  RFval = val;
  LRvalinit = LRval;

}

// Stop Alex. To replace with bare-metal code later.
void stop()
{
  dir = STOP;
  TCCR0A = 0b00000001;
  PORTD &= (~LR & ~LF); //off LR & LF

  TCCR1A = 0b00000001;
  PORTB &= ( ~RR & ~ RF ); //off RF & RF

}


// Clears all our counters
void clearCounters()
{
  leftForwardTicks=0;
  rightForwardTicks=0;
  leftReverseTicks = 0;
  rightReverseTicks=0;
  leftForwardTicksTurns=0;
  rightForwardTicksTurns=0;
  leftReverseTicksTurns=0;
  rightReverseTicksTurns=0;
  leftRevs=0;
  rightRevs=0;
  forwardDist=0;
  reverseDist=0; 
}

// Clears one particular counter
void clearOneCounter(int which)
{
  clearCounters();
}
// Intialize Vincet's internal states

void initializeState()
{
  clearCounters();
}

void handleCommand(TPacket *command)
{
  switch(command->command)
  {
    // For movement commands, param[0] = distance, param[1] = speed.
    case COMMAND_FORWARD:
        sendOK();
        forward((float) command->params[0], (float) command->params[1]);
      break;
    case COMMAND_REVERSE:
        sendOK();
        reverse((float) command->params[0], (float) command->params[1]);
        break;
    case COMMAND_TURN_LEFT:
        sendOK();
        left((float) command->params[0], (float) command->params[1]);
        break;
    case COMMAND_TURN_RIGHT:
        sendOK();
        right((float) command->params[0], (float) command->params[1]);
        break;
    case COMMAND_STOP:
        sendOK();
        stop();
        break;
    case COMMAND_GET_STATS:
        sendOK();
        sendStatus();
        break;
    case COMMAND_CLEAR_STATS:
        sendOK();
        clearOneCounter(command->params[0]);
        break;
        
    default:
      sendBadCommand();
  }
}

void waitForHello()
{
  int exit=0;

  while(!exit)
  {
    TPacket hello;
    TResult result;
    
    do
    {
      result = readPacket(&hello);
    } while (result == PACKET_INCOMPLETE);

    if(result == PACKET_OK)
    {
      if(hello.packetType == PACKET_TYPE_HELLO)
      {
     

        sendOK();
        exit=1;
      }
      else
        sendBadResponse();
    }
    else
      if(result == PACKET_BAD)
      {
        sendBadPacket();
      }
      else
        if(result == PACKET_CHECKSUM_BAD)
          sendBadChecksum();
  } // !exit
}

void IR(TPacket *command) {
  
  if((PINB & 0b00001000)&&(PINB & 0b00010000)){
    if (dir == FORWARD){
      if(!(PINB & 0b00100000)){
        stop();
        sendStop('f');
      }
      LFval = LFvalinit;
      RFval = RFvalinit;
        
    }
    else if (dir == BACKWARD){
      LRval = LRvalinit;
      RRval = RRvalinit;
    }
  }
  else if((PINB & 0b00001000)){
    if (dir == FORWARD){
      stop();
      sendStop('r');
      //RFval = RFvalinit/2;
      //RFval = 0;
     
    }
  }

  else if((PINB & 0b00010000)){
    if (dir == FORWARD){
      stop();
      sendStop('l');
      //LFval = LFvalinit/2;
      //LFval = 0;
    }
  }
  //delay(200);
}

void setup() {
  // put your setup code here, to run once:
  AlexDiagonal = sqrt((ALEX_LENGTH * ALEX_LENGTH) + (ALEX_BREADTH * ALEX_BREADTH));
  AlexCirc = PI * AlexDiagonal;
  cli();
  setupEINT();
  setupSerial();
  startSerial();
  setupMotors();
  startMotors();
  enablePullups();
  initializeState();
  DDRB &= 0b11000111; //using pin 11,12,13
  sei();
  setupPowerSaving();
}

void handlePacket(TPacket *packet)
{
  switch(packet->packetType)
  {
    case PACKET_TYPE_COMMAND:
      handleCommand(packet);
      break;

    case PACKET_TYPE_RESPONSE:
      break;

    case PACKET_TYPE_ERROR:
      break;

    case PACKET_TYPE_MESSAGE:
    
      break;

    case PACKET_TYPE_HELLO:
      break;
  }
}
unsigned long computeDeltaTicks(float ang)
{
  unsigned long ticks = (unsigned long) ((ang * AlexCirc * COUNTS_PER_REV) / (360.0 * WHEEL_CIRC));
  return ticks;
}
void loop() {

// Uncomment the code below for Step 2 of Activity 3 in Week 8 Studio 2

// forward(0, 100);

// Uncomment the code below for Week 9 Studio 2


 // put your main code here, to run repeatedly:
  TPacket recvPacket; // This holds commands from the Pi
  if (dir == STOP)
  {
    putArduinoToIdle();
  }
  
  TResult result = readPacket(&recvPacket);

    
  
  if(result == PACKET_OK)
    handlePacket(&recvPacket);
  else{
    if(result == PACKET_BAD)
    {
      sendBadPacket();
    }
    else
      if(result == PACKET_CHECKSUM_BAD)
      {
        sendBadChecksum();
      } 
  
  }
  if (deltaDist > 0)
  {
    if(dir == FORWARD)
    {
      /*if(leftForwardTicks > rightForwardTicks + 50){
        RFvalinit += 2;
        LFvalinit -= 2;
      }else if(rightForwardTicks > leftForwardTicks + 50){
        LFvalinit += 2;
        RFvalinit -= 2;
      }*/
      IR(&recvPacket);
      if (forwardDist >= newDist)
      {
        deltaDist = 0;
        newDist = 0;
        stop();
      }
    }

    else
      if (dir == BACKWARD)
      { 
        /*if(leftReverseTicks > rightReverseTicks + 50){
        RFvalinit += 2;
        LFvalinit -= 2;
      }else if(rightReverseTicks > leftReverseTicks + 50){
        LFvalinit += 2;
        RFvalinit -= 2;
      }*/
        IR(&recvPacket);
        if(reverseDist >= newDist)
        {
          deltaDist=0;
          newDist=0;
          stop();
        }
      }
      else
        if (dir == STOP)
        {
            deltaDist = 0;
            newDist = 0;
            stop();
        }
      
  }

  if (deltaTicks > 0)
  {
    if (dir == LEFT)
    {
      if (leftReverseTicksTurns >= targetTicks)
      {
        deltaTicks=0;
        targetTicks = 0;
        stop();
      }
    }
    else
      if (dir == RIGHT)
      {
        if(rightReverseTicksTurns >= targetTicks)
        {
          deltaTicks=0;
          targetTicks=0;
          stop();
        }
      }
      else
        if(dir == STOP)
        {
          deltaTicks=0;
          targetTicks=0;
          stop();
        }
  }
     
}
