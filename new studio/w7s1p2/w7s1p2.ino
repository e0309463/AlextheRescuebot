#include <avr/io.h>
#include <avr/interrupt.h>

/*#ifndef F_CPU
#define F_CPU   16000000UL
#endif
*/
#include <util/delay.h>

// Masks for pins 12 and 11
#define PIN12MASK   0b00010000
#define PIN11MASK   0b00001000

// UDRIE mask. Use this to enable/disable
// the UDRE interrupt
#define UDRIEMASK   0b00100000

static volatile char flashWhich=1;
static volatile char buttonVal=1;

char dataRecv, dataSend;

ISR(USART_RX_vect)
{
  // Write received data to dataRecv
  dataRecv = UDR0-'0';
}

ISR(USART_UDRE_vect)
{
  // Write dataSend to UDR0
  UDR0 = dataSend;
  
  // Disable UDRE interrupt
  UCSR0B &= 0b11011111;
  
}

/*
void sendData(const char data)
{
  // Copy data to be sent to dataSend
  dataSend = buttonVal+'0';
  
  // Enable UDRE interrupt below
  while((UCSR0A & 0b00100000) == 0);
  
  UDR0 = dataSend;
}

char recvData()
{
  while((UCSR0A & 0b10000000) == 0);

  unsigned char dataRecv = UDR0;
  return dataRecv - '0';
}*/

void setupSerial()
{
  // Set up the 57600 8N1 

  UBRR0L = 16;
  UBRR0H = 0;
  
  UCSR0C = 0b00000110;
  UCSR0A = 0;
}

void startSerial()
{
  // Start the serial port.
  // Enable RXC interrupt, but NOT UDRIE
  // Remember to enable the receiver
  // and transmitter
  UCSR0B = 0b10111000;
}

// Enable external interrupts 0 and 1
void setupEINT()
{
  // Configure INT0 and INT1 for rising edge triggered.
  // Remember to enable INT0 and INT1 interrupts.
  EICRA |= 0b00001111;
  EIMSK |= 0b00000011;
}

// ISRs for external interrupts
ISR(INT0_vect)
{
  dataSend = '1';
  UCSR0B |= 0b00100000;
}

ISR(INT1_vect)
{
  dataSend = '2';
  UCSR0B |= 0b00100000;
}

// Red is on pin 12
void flashRed()
{
    PORTB |= PIN12MASK;
    delay(100);
    PORTB &= ~PIN12MASK;
    delay(500);
}

// Green is on pin 11
void flashGreen()
{
    PORTB |= PIN11MASK;
    delay(100);
    PORTB &= ~PIN11MASK;
    delay(500);
}

void setup() {

  cli();
  // put your setup code here, to run once:

  DDRB |= (PIN11MASK | PIN12MASK);
  setupEINT();
  setupSerial();
  startSerial();
  sei();
}

void loop() {
  // put your main code here, to run repeatedly:

  flashWhich = dataRecv;
   
  if(flashWhich == 1)
    flashRed();
  else
    flashGreen();
}
