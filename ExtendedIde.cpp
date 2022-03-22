
#include "ExtendedIde.h"


//Constructor para PWM
ExtendedIde::ExtendedIde(int pin, float freq, int mode, int nbits=0 ) {
  pin_ = pin;
  freq_ = freq;
  mode_ = mode;
  
  SelectTimerWithPin();
  timers_.GetAproximateFreq(freq_, timer_);
  if ( mode_ == NORMAL) {
    ModifyRegisterNormalMode(timers_.GetBitPrescaler()); 
    PrintParameters();
  } else if ( mode_ == FPWM_MAX) {
    
      switch (timer_) {
        case 0: ModifyRegFPWM_Timer0(); break;
        case 1: ModifyRegFPWM_Timer1(0); break;
        case 2: ModifyRegFPWM_Timer2(); break;
      }
  } else if ( mode_ == FPWM_OCR) {//La variación del registro OCR esta solo en el Timer1
      ModifyRegFPWM_Timer1(nbits);
  }  
}

ExtendedIde::ExtendedIde() {}

void ExtendedIde::ChangedAnalogRead(int prescaler) {
  byte temp;
  switch(prescaler) {
    case    8: temp = 0b011; break;
    case   16: temp = 0b100; break; 
    case   32: temp = 0b101; break;
    case   64: temp = 0b110; break;
    case  128: temp = 0b111; break;
  }

  ADCSRA = ADCSRA & 0b11111000 | temp; 
}

void ExtendedIde::PrintParameters() {
  Serial.print("Modo: ");
  switch (mode_) {
    case 0: Serial.println("Normal"); break;
    case 1: Serial.println("Fast PWM MAX"); break;
    case 2: Serial.println("Fast PWM OCR"); break;
  }
  Serial.print("Pin: ");
  Serial.println(pin_);
  Serial.print("Frecuencia: ");
  Serial.println(freq_);
  Serial.print("Frecuencia más cercana: ");
  Serial.println(timers_.GetFrequency());
  Serial.print("Prescalar: ");
  Serial.println(timers_.GetPrescaler());
  
}


void ExtendedIde::SelectTimerWithPin() {
  if (pin_ == 5 || pin_ == 6 ) {    
    timer_ = 0;
  }
  else if (pin_ == 9 || pin_ == 10) {
    timer_ = 1;     
  } 
  else if (pin_ == 3 || pin_ == 11) {    
    timer_ = 2;
  } 
  else {
    Serial.println("Error: Pin fuera de rango"); 
  }  
}

void ExtendedIde::ModifyRegisterNormalMode(byte conf) {
  switch (pin_) {
    //Timer0
    case 5: TCCR0B = TCCR0B & 0b11111000 | conf; break;
    case 6: TCCR0B = TCCR0B & 0b11111000 | conf; break;
    //Timer1
    case 9: TCCR1B = TCCR1B & 0b11111000 | conf; break;
    case 10:TCCR1B = TCCR1B & 0b11111000 | conf; break;
    //Timer2
    case 3: TCCR2B = TCCR2B & 0b11111000 | conf; break;
    case 11:TCCR2B = TCCR2B & 0b11111000 | conf; break;
  }
}


void ExtendedIde::ModifyRegFPWM_Timer0() {
  if ( mode_ == FPWM_MAX) {
    //Fast PWM mode in timer 0 with 0xFF as max
    TCCR0A |= (1<<WGM00)|(1<<WGM01);
    TCCR0B &=~ (1<<WGM02);
    //Disable all interrupts
    TIMSK0 &=~ (1<<TOIE0)|(1<<OCIE0B)|(1<<OCIE0A);
  } else if (mode_ == FPWM_OCR) {
    //Fast PWM mode in timer 0 with OCR0A as max
    TCCR0A |= (1<<WGM00)|(1<<WGM01);
    TCCR0B |= (1<<WGM02);
    //Disable all interrupts
    TIMSK0 &=~ (1<<TOIE0)|(1<<OCIE0B)|(1<<OCIE0A);
  }
}
void ExtendedIde::ModifyRegFPWM_Timer1(int nbits) {
  switch(nbits) {
    case 0:
      //Fast PWM mode in timer 1 with OCR1A as top
      TCCR1A |= (1<<WGM10)|(1<<WGM11);
      TCCR1B |= (1<<WGM12)|(1<<WGM13);
      //Disable all interrupts
      TIMSK1 &=~ (1<<TOIE1)|(1<<ICIE1)|(1<<OCIE1B)|(1<<OCIE1A);
      break;
    case 8:
      //Fast PWM mode in timer 1 with 0xFF as top
      TCCR1A |= (1<<WGM10);
      TCCR1A &=~ (1<<WGM11);
      TCCR1B |= (1<<WGM12);
      TCCR1B &=~ (1<<WGM13);
      //Disable all interrupts
      TIMSK1 &=~ (1<<TOIE1)|(1<<ICIE1)|(1<<OCIE1B)|(1<<OCIE1A);
      break;
    case 9:
      //Fast PWM mode in timer 1 with 0x01FF as top
      TCCR1A &=~ (1<<WGM10);
      TCCR1A |= (1<<WGM11);
      TCCR1B |= (1<<WGM12);
      TCCR1B &=~ (1<<WGM13);
      //Disable all interrupts
      TIMSK1 &=~ (1<<TOIE1)|(1<<ICIE1)|(1<<OCIE1B)|(1<<OCIE1A);
      break;
    case 10:
      //Fast PWM mode in timer 1 with 0x03FF as top
      TCCR1A |= (1<<WGM10);
      TCCR1A |= (1<<WGM11);
      TCCR1B |= (1<<WGM12);
      TCCR1B &=~ (1<<WGM13);
      //Disable all interrupts
      TIMSK1 &=~ (1<<TOIE1)|(1<<ICIE1)|(1<<OCIE1B)|(1<<OCIE1A);
      break;
    
  }
}
void ExtendedIde::ModifyRegFPWM_Timer2() {
    if ( mode_ == FPWM_MAX) {
    //Fast PWM mode in timer 2 with 0xFF as max
    TCCR2A |= (1<<WGM20)|(1<<WGM21);
    TCCR2B &=~ (1<<WGM22);
    //Disable all interrupts
    TIMSK2 &=~ (1<<TOIE2)|(1<<OCIE2B)|(1<<OCIE2A);
  } else if (mode_ == FPWM_OCR) {
    //Fast PWM mode in timer 2 with OCR2A as max
    TCCR2A |= (1<<WGM20)|(1<<WGM21);
    TCCR2B |= (1<<WGM22);
    //Disable all interrupts
    TIMSK2 &=~ (1<<TOIE2)|(1<<OCIE2B)|(1<<OCIE2A);
  }
}
