#include "PongGame.h"

namespace hw4 {
    inline int bound(int x, int vmin, int vmax) {
        return x <= vmin ? vmin : (x >= vmax ? vmax: x);
    }

    inline int max(int a, int b){
        return a < b ? b : a;
    }

    inline int min(int a, int b){
        return a < b ? a : b;
    }

    const int PongGame::ndim = 2;
    const int PongGame::nPlayer = 2;

    PongGame::PongGame():
            nrow(8),
            ncol(8),
            gameState(GameState::START),
            ballSpeed(250)
    {
        reset();
        resetPadPos();
    }


    PongGame::PongGame(unsigned int nrow, unsigned int ncol):
            nrow(nrow),
            ncol(ncol),
            gameState(GameState::START),
            ballSpeed(250)
    {
        reset();
        resetPadPos();
    }


    void PongGame::reset(){
        BallPosition bpos(4,1);
        BallVelocity bvelo(VSpeed::UP, HSpeed::RIGHT);
        reset(bpos, bvelo);
    }

    void PongGame::resetPadPos() {
        setPadPos(0, 4);
        setPadPos(1, 4);
    }

    void PongGame::reset(BallPosition ballPos, BallVelocity iniVelo) {
        this->ballVelo = iniVelo;
        gameState = GameState::START;
        setBallPosition(ballPos);
        winner = Player::PLAYER_UNKNOWN;
        dirty=true;
    }


    void PongGame::start(unsigned long now){
        gameState = GameState::RUNNING;
        dirty=true;
    }


    bool PongGame::paddleRowCoverBallRow(int paddleRow, int ballRow){
        return (ballRow >= paddleRow - 1) && (ballRow <= paddleRow + 1);
    }


    bool PongGame::hitPlayerOne() {
        bool ret = ballVelo.hSpeed==HSpeed::LEFT &&
                   ballPos.hPos == 1 &&
                   paddleRowCoverBallRow(padPos[0], ballPos.vPos);
        return ret;
    }


    bool PongGame::hitPlayerTwo() {
        bool ret = ballVelo.hSpeed==HSpeed::RIGHT &&
                   ballPos.hPos == ncol-2 &&
                   paddleRowCoverBallRow(padPos[1], ballPos.vPos);
        return ret;
    }


    Player PongGame::hitPlayer(){
        if(hitPlayerOne()){
            return Player::PLAYER_ONE;
        }else if(hitPlayerTwo()){
            return Player::PLAYER_TWO;
        }
        return Player::PLAYER_UNKNOWN;
    }


    inline VSpeed bounceVspeed(int padpos, int ballpos){
        if(padpos == ballpos){
            return VSpeed::STILL;
        }else if(padpos > ballpos){//pad lower than ball
            return VSpeed::UP;
        }else if(padpos<ballpos){//pad higher than ball;
            return VSpeed::DOWN;
        }
        //shouldn't reach here
        return VSpeed::DOWN; //shut up compiler

    }

    void PongGame::bouncePlayer(){
        switch(hitPlayer()) {
            case Player::PLAYER_ONE:
                ballVelo.hSpeed=HSpeed::RIGHT;
                ballVelo.vSpeed=bounceVspeed(padPos[0], ballPos.vPos);
                break;
            case Player::PLAYER_TWO:
                ballVelo.hSpeed=HSpeed::LEFT;
                ballVelo.vSpeed=bounceVspeed(padPos[1], ballPos.vPos);
                break;
            case Player::PLAYER_UNKNOWN:
                break;
        }
    }


    bool PongGame::hitTopWall(){ return ballPos.vPos <= 0 && ballVelo.vSpeed == VSpeed::UP; }
    bool PongGame::hitBottomWall(){ return ballPos.vPos >= nrow-1 && ballVelo.vSpeed == VSpeed::DOWN;}
    Wall PongGame::hitWall() {
        if(hitTopWall()){
            return Wall::TOP;
        } else if(hitBottomWall()){
            return Wall::BOTTOM;
        } else {
            return Wall::UNKNOWN;
        }
    }


    void PongGame::bounceWall(){
        switch(hitWall()){
            case Wall::TOP:
                ballVelo.vSpeed = VSpeed::DOWN;
                break;
            case Wall::BOTTOM:
                ballVelo.vSpeed = VSpeed::UP;
                break;
            case Wall::UNKNOWN:
                break;
        }
    }


    void PongGame::moveBall(){
        switch(ballVelo.vSpeed){
            case VSpeed::UP:
                ballPos.vPos--;
                break;
            case VSpeed::DOWN:
                ballPos.vPos++;
                break;
            case VSpeed::STILL:
                break;
        }

        switch(ballVelo.hSpeed){
            case HSpeed::LEFT:
                ballPos.hPos--;
                break;
            case HSpeed::RIGHT:
                ballPos.hPos++;
                break;
        }
        dirty=true;
    }


    void PongGame::checkEndGame(){
        if(ballPos.hPos==0){
            winner=Player::PLAYER_TWO;
            gameState = GameState::FINISHED;
        }
        else if(ballPos.hPos==ncol-1){
            winner=Player::PLAYER_ONE;
            gameState = GameState::FINISHED;
        }
    }


    void PongGame::resetBuffer(int* buffer){
        for(int irow=0; irow<nrow; irow++){
            for(int icol=0; icol<ncol; icol++){
                writeBuffer(buffer, irow, icol, 0);
            }
        }
    }


    void PongGame::paintPad(int* buffer, int padPos, int col){
        writeBuffer(buffer, padPos, col, 1);
        writeBuffer(buffer, max(padPos-1, 0), col, 1);
        writeBuffer(buffer, min(padPos+1, nrow-1), col, 1);
        dirty=false;
    }

    inline void PongGame::paintBall(int* buffer){
        writeBuffer(buffer, ballPos.vPos, ballPos.hPos, 1);
    }


    //write 2d array to buffer
    //the first index is row and second index is column
    void PongGame::paint(int* buffer){
        resetBuffer(buffer);
        //player one
        paintPad(buffer, padPos[0], 0);
        //player two
        paintPad(buffer, padPos[1], ncol-1);
        //ball
        paintBall(buffer);
    }

    bool PongGame::update(unsigned long now){
        if(gameState!=GameState::RUNNING){return false;}//game not in running state
        if(now - lastUpdate > ballSpeed){
            bouncePlayer(); //do not switch the order
            bounceWall();
            moveBall();
            checkEndGame();
            lastUpdate = now;
            dirty=true;
            return true;
        } else { //calling it too fast
            return false;
        }
    }

    void PongGame::movePad(Player player, PadDirection dir){
        const int pindex = static_cast<int>(player);
        const int increment = dir==PadDirection::UP?-1:+1;
        const int newPos = padPos[pindex]+increment;
        padPos[pindex] = bound(newPos, 0, nrow-1);
        dirty=true;
    }

    void PongGame::setPadPos(int pindex, int pos){
        padPos[pindex] = bound(pos,0,nrow-1);
        dirty=true;
    }
}
