#include <Arduino_AVRSTL.h>


#pragma once

#define NORMAL 0
#define FPWD_MAX 1
#define FPWM_OCR 2

class Timer {
  private:
    std::vector<float> frequencies_;
    int presc_;
    float freq_;
    byte conf_;
    std::vector<int>* prescalers_; 

    std::vector<int> presc_timer01_ = {1, 8, 64, 256, 1024};
    std::vector<int> presc_timer2_ = {1, 8, 32, 64, 128, 256, 1024};

  public:
    Timer();
    ~Timer();

    float GetAproximateFreq(float aprox, int timer);
    void SetFrequencies(int timer);
    void SetPrescaler(int timer);
    void SetBitPrescaler(int timer);
    int GetPrescaler();
    byte GetBitPrescaler();
    float GetFrequency();
    void PrintFrequencies();

    int abss(int x);
};
