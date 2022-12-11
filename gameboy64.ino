#include "PongGame.h"
#include "LEDMatrix.h"

using namespace std;
using namespace hw4;

const int nrow = 8;
const int ncol = 8;

PongGame pongGame(nrow, ncol);
int* buffer = new int[nrow*ncol];
short* screen_buffer = new short[nrow];

LEDMatrix ledMat(2, 3, 4, 5);

int pins[] = {8, 9, 10, 11};
int pre_states[] = {1, 1, 1, 1};

void setup(){
    pongGame.start(millis());
    for (int i=0;i<4;i++)
      pinMode(pins[i], INPUT_PULLUP);
}

void drawScreen(int* buffer){
    short row = 0;
    for(int irow=0; irow<nrow; irow++){
        for(int icol=0; icol<ncol; icol++){
            row |= buffer[irow * ncol + icol];
            if (icol != ncol - 1) row = row << 1;
        }
        screen_buffer[irow] = row;
        row = 0;
    }
    ledMat.writeData(screen_buffer);
}

void processInput(unsigned long tick){
  int state;
  for (int i=0;i<4;i++)
  {
    if ((state = digitalRead(pins[i])) == 0 && pre_states[i] == 1)
    {
      switch(i){
          case 1:
              pongGame.movePad(Player::PLAYER_ONE, PadDirection::UP);
              break;
          case 0:
              pongGame.movePad(Player::PLAYER_ONE, PadDirection::DOWN);
              break;
          case 3:
              pongGame.movePad(Player::PLAYER_TWO, PadDirection::UP);
              break;
          case 2:
              pongGame.movePad(Player::PLAYER_TWO, PadDirection::DOWN);
              break;
      }
    }
    pre_states[i] = state;
  }
}

void loop(){
    const unsigned long tick = millis();
    processInput(tick);
    pongGame.update(tick);

    if(pongGame.isDirty()){
        pongGame.paint(buffer);
        drawScreen(buffer);
    }
    ledMat.update();
}
