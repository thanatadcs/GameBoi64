#ifndef HC595_H
#define HC595_H
class HC595 {
  public:
    HC595(int ser, int srClk, int rClk, int srClr);
    void reset();
    void pushOne(int data);
    void latch();
    void pushn(unsigned long data, int nbits);
  private:
    int ser;
    int srClk;
    int rClk;
    int srClr;
};
#endif