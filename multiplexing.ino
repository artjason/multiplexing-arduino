int timeDelay = 3;///;
int analogInputPin = A0;                  //potentiometer analog pin
unsigned int analogValue = 0;             //value for analog input
unsigned short divisor[] = {1, 10, 100, 1000};  //divisor
unsigned long voltageOutput;
long currentMillis, previousMillis;
byte i = 0;
byte segmentPatterns[] = {0x3f, 0x06, 0x5b,0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f}; //patterns for 4 digit 7seg
byte posWeight[] = {0, 0, 0, 0};  
byte digit[] = { 0x07, 0x0b, 0x0d, 0x0e};

// Smoothing variables
const int numReadings = 16; // value to determine the size of the readings array.
int readings[numReadings];  // the readings from the analog input
int readIndex = 0;          // the index of the current reading
int total = 0;              // the running total
int average = 0;            // the average

void setup() {
  DDRD = 0B11111111; //a,b,c,d,e,f,g,decpin configured as outputs
  DDRB = 0B00001111; //d1,d2,d3,d4 configured as outputs
  // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}


void loop() {
  currentMillis = millis(); //get the current time
  if (currentMillis - previousMillis >= timeDelay) {
       previousMillis = currentMillis;

      //smoothing
      // subtract the last reading:
      total = total - readings[readIndex];
      // read from the sensor:
      readings[readIndex] = analogRead(analogInputPin);
      // add the reading to the total:
      total = total + readings[readIndex];
      // advance to the next position in the array:
      readIndex++;

      // if we're at the end of the array...
      if (readIndex >= numReadings) {
      // ...wrap around to the beginning:
      readIndex = 0;
      }
      // calculate the average:
      average = total / numReadings;
    
    voltageOutput = map(average, 0, 1023, 0, 4910); // interpolation to 0-5000
    posWeight[i] =  voltageOutput /(divisor[i]) % 10;
    PORTB = 0xff;
    PORTD = segmentPatterns[posWeight[i]];
    PORTB = digit[i];
    if (i == 3) {
      PORTD |= (1 << 7);
    }
    i = (i < 3) ? ++i : 0;
  }
}
