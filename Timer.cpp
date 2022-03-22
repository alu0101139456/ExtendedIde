#include "Timer.h"

Timer::Timer() {
  
 
}

Timer::~Timer() {}

int Timer::abss(int x)  {     
  return x > 0 ? x : (-1 * x);
}


void Timer::SetFrequencies(int timer) {
  long int frec = 8000000;
  if(timer == 0) {
    frec = 16000000;
  }
  
  double aux;
  SetPrescaler(timer);
  for(int i = 0; i < prescalers_->size(); i++) {
    frequencies_.push_back(frec/(prescalers_->at(i))/256.0);
  }

}

void Timer::SetBitPrescaler(int timer) {
  if(timer != 2) {
    switch(presc_) {
      case    1: conf_ = 0b001; break;
      case    8: conf_ = 0b010; break;
      case   64: conf_ = 0b011; break; 
      case  256: conf_ = 0b100; break;
      case 1024: conf_ = 0b101; break;
    }
  } else if(timer == 2) {
      switch(presc_) {
      case    1: conf_ = 0b001; break;
      case    8: conf_ = 0b010; break;
      case   32: conf_ = 0b011; break; 
      case   64: conf_ = 0b100; break; 
      case  128: conf_ = 0b101; break;
      case  256: conf_ = 0b110; break;
      case 1024: conf_ = 0b111; break;
    }
  }
}

void Timer::SetPrescaler(int timer) {
  if (timer != 2) {
    prescalers_ = &presc_timer01_;
  }
  else {
    prescalers_ = &presc_timer2_;    
  }
}
byte Timer::GetBitPrescaler() {
  return conf_;
}
float Timer::GetAproximateFreq(float aprox, int timer) {
  SetFrequencies(timer);
  
  int a, b, bestFreq;
  for (size_t i = 1; i < frequencies_.size()-1; i++) {
    
    if (aprox > frequencies_[i]) {
      a = abss(aprox - frequencies_[i]); 
      b = abss(aprox - frequencies_[i-1]); 
      bestFreq = a < b ? i : i-1;

      presc_ = prescalers_->at(bestFreq);
      freq_ = frequencies_[bestFreq];
      SetBitPrescaler(timer);
      return freq_;
    } 
  }
  presc_ = prescalers_->at(prescalers_->size() - 1); 
  freq_ = frequencies_[frequencies_.size()-1];
  SetBitPrescaler(timer);
  return freq_;
}

int Timer::GetPrescaler() {
  return presc_;
}

float Timer::GetFrequency() {
  return freq_;
}

void Timer::PrintFrequencies() {
  for (int i = 0; i < frequencies_.size(); i++) {
    Serial.print("Presc:");
    Serial.println(prescalers_->at(i));
    Serial.print("Freq:");
    Serial.println(frequencies_[i]);     
  }
}
