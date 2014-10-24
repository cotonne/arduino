// http://www.avrfreaks.net/forum/tut-c-newbies-guide-avr-adc

const int PIN_MEASURE = 5;

const int MAX_STREAM_SIZE = 128;

const int MAX_SAMPLES = MAX_STREAM_SIZE * 4;

int DELAY = 100;

// Store the number of samples made
unsigned int nbSamples[2];
// Store the ADC conversion
char* samples;
// Indicate the current channel used
// We used two, so one is sent while the other one is filled
uint8_t current = 0;

char buf[MAX_STREAM_SIZE * 2];

void setup(){
 Serial.begin(115200);
 
 nbSamples[0] = 0;
 nbSamples[1] = 0;
 
 samples = (char*)malloc(2*MAX_SAMPLES);
 
 pinMode(A5, INPUT);
 
 initPin5FreeRunningMode();
}

void loop(){
  delay(DELAY);
  uint8_t previousCurrent = current;
  // Releasing fastly the channel for new samples
  current = current == 1? 0:1;
  unsigned int nbSampleRead = 0;
     
  while( !(nbSampleRead >= nbSamples[previousCurrent]) ){
    unsigned int nbSampleLength = 0;
    if( (nbSampleRead + MAX_STREAM_SIZE) > nbSamples[previousCurrent] ){
      nbSampleLength = nbSamples[previousCurrent] - nbSampleRead;
    } else {
      nbSampleLength = MAX_STREAM_SIZE;
    }

    for(unsigned int i = 0 ; i < nbSampleLength ; i++){
      utoah(samples[previousCurrent * MAX_SAMPLES + nbSampleRead + i], buf + i*2);
    }

    Serial.write((uint8_t*)buf, 2*nbSampleLength);
    nbSampleRead += MAX_STREAM_SIZE;
  }

  Serial.println();
  nbSamples[previousCurrent] = 0;
}

// 8-bit conversion
void initPin5FreeRunningMode(){ 
 // ADTS2 ADTS1 ADTS0 Trigger Source
 //     0     0     0 Free Running mode
 // p.637 - §40.8.1 Rev D
 // Analog MUX can be turned off when setting ACME bit
 //    If the ACME (Analog Comparator Multiplexer Enabled) bit in ADCSRB is set while MUX3 in ADMUX is '1'
 //   (ADMUX[3:0]=1xxx), all MUXes are turned off until the ACME bit is cleared.
 // Problem Fix/Workaround
 //   Clear the MUX3 bit before setting the ACME bit.
 ADCSRB = 0;
 // ADCSRB = (ADCSRB & ~(1 << MUX5)) | (((PIN_MEASURE >> 3) & 0x01) << MUX5);  
   
 // p. 248, §4.9.1 ADMUX – ADC Multiplexer Selection Register
 // presented left adjusted by setting the ADLAR bit (bit 5) in ADMUX 
 // AV CC with external capacitor at AREF pin
 // REFS1 REFS0 Voltage Reference Selection
 //     0     1 AV CC with external capacitor at AREF pin
   ADMUX |= (1 << REFS0) | (1 << ADLAR) | (PIN_MEASURE & 0x07);
   ADMUX &= ~(1<<3);
   
 // The first conversion must be started by writing a logical one to the ADSC bit in ADCSRA
 // p.239
 // Using the ADC Interrupt Flag as a trigger source makes the ADC start a new conversion as soon as the ongoing
 // conversion has finished. The ADC then operates in Free Running mode, constantly sampling and updating the
 // ADC Data Register.
 // p.249
 // Bit 7 – ADEN: ADC Enable Writing this bit to one enables the ADC
 // Bit 6 – ADSC: ADC Start Conversion. In Free Running mode, write this bit to one to start the first conversion.
 // Bit 5 – ADATE: ADC Auto Trigger Enable. When this bit is written to one, Auto Triggering of the ADC is enabled.
 //               The trigger source is selected by setting the ADC Trigger Select bits,ADTS in ADCSRB
 // Bit 3 – ADIE: ADC Interrupt Enable. When this bit is written to one and the I-bit in SREG is set, 
 //               the ADC Conversion Complete Interrupt is activated.
 // Bits 2:0 – ADPS[2:0]: ADC Prescaler Select Bits 
 //               These bits determine the division factor between the system clock frequency and the input clock to the ADC.
   ADCSRA = (1 << ADEN) | (1 << ADSC) | (1 << ADATE) | (1 << ADIE);
   ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
 
 // p.11
 // Bit 7 – I: Global Interrupt Enable. The Global Interrupt Enable bit must be set for the interrupts to be enabled
   sei();                    // Enable global interrupt
 
 //The I-bit is automatically set when a Return from Interrupt instruction – RETI – is executed.
}

 // p.65
 // VectorNo. Program
 //           Address Source Interrupt Definition
 //       22   0x002A    ADC       ADC Conversion Complete
 // p.14, §7.7
ISR(ADC_vect){
  uint8_t nbSample = nbSamples[current];
  if(nbSample < MAX_SAMPLES){
    int pos = current * MAX_SAMPLES + nbSample;
    // Save the conversion
    samples[pos] = ADCH;
    nbSamples[current] = nbSample + 1;
  }
}

// This function is to do an hex conversion on two char
void utoah( uint8_t val, char *  s) {
   uint8_t lb = val >> 4;
   if( lb < 10 ){
      s[0] = lb +  48;
   } else {
      s[0] = lb + 87; 
   }
   
   uint8_t hb = val & 0x0F;
   if( hb < 10 ){
      s[1] = hb +  48;
   } else {
      s[1] = hb + 87;  // (int)A = 97 so, hb >= 10 => hb + 87 >=  97
   }
}


