#include <avr/io.h>
#include <avr/interrupt.h>


// Masks for pins 12 and 11
#define PIN12MASK   0b00010000
#define PIN11MASK   0b00001000

void sendData(const char data)
{
  // Poll UDRE bit in UCSR0A until it is 1
  // Send data
  while((UCSR0A & 0b00100000) == 0);
  UDR0 = data;
}

char recvData()
{
  // If RXC0 bit is 0, return 0
  // Otherwise return contents of UDR0 - '0'
  if((UCSR0A & 0b10000000) == 0) return 0;
  unsigned char data = UDR0;
  return data - '0';
}

void setupUART()
{
  // Set up for 57600 8N1
  UBRR0L = 16;
  UBRR0H = 0;
  UCSR0C = 0b00000110;
  UCSR0A = 0;
 // UCSR0A = 0b0;
}

void startUART()
{
  // Start the transmitter and receiver, but disable
  // all interrupts.
  UCSR0B = 0b00011000;
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
  sendData('0' + 1);
}

ISR(INT1_vect)
{
  sendData('0'+ 2);
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
  setupUART();
  startUART();
  sei();
}

void loop() {
  // put your main code here, to run repeatedly:

  char data = recvData();
    
  if(data == 1)
    flashRed();
  else if(data == 2)
    flashGreen();

}
