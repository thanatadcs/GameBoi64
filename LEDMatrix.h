#ifndef LEDMATRIX_H
#define LEDMATRIX_H
#include "HC595.h"
class LEDMatrix{
  
  public:
    LEDMatrix(int ser, int srClk, int rClk, int srClr):
      shreg(ser,srClk,rClk,srClr),
      currentRow(0){};
  
    void writeRow(unsigned int rowNo, unsigned int colData){
      unsigned int rowsel = ~(1<<(8-1-rowNo));
      unsigned int data = colData | rowsel << 8;
      shreg.pushn(data, 16);
      shreg.latch();
    }
    
    void writeData(int *data){
      for(int i=0; i<8; i++){
        buffer[i] = data[i];
      }
    }
  
    void update(){
      currentRow = (currentRow+1)%8;
      writeRow(currentRow,buffer[currentRow]);
    }
  
  private:
    HC595 shreg;
    int buffer[8];
    int currentRow;
};
#endif
