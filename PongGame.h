#ifndef PONGGAME_H
#define PONGGAME_H
#define PONGGAME_NDIM 2

namespace hw4 {
    enum class Wall{TOP, BOTTOM, UNKNOWN};
    enum class PadDirection{UP, DOWN};
    enum class VSpeed{UP, DOWN, STILL};
    enum class HSpeed{LEFT, RIGHT};
    enum class GameState{START, RUNNING, FINISHED};
    enum class Player: int {PLAYER_ONE=0, PLAYER_TWO=1, PLAYER_UNKNOWN=99};//yep for array indexing

    class BallVelocity {
    public:
        BallVelocity():vSpeed(VSpeed::UP), hSpeed(HSpeed::RIGHT){};
        BallVelocity(VSpeed vSpeed, HSpeed hSpeed):vSpeed(vSpeed), hSpeed(hSpeed){};
        VSpeed vSpeed;
        HSpeed hSpeed;
    };

    class BallPosition {
    public:
        BallPosition():vPos(4), hPos(4){};
        BallPosition(int vPos, int hPos):vPos(vPos), hPos(hPos){};
        int vPos;
        int hPos;
    };

    class PongGame{
    public:
        //constuct ponggame with nrow and ncol.
        //ballspeed is amount in ms that the ball should move
        static const int ndim;
        static const int nPlayer;
        PongGame();
        PongGame(unsigned int nrow, unsigned int ncol);

        //reset with ballSpeed and
        //ballPosition(top left is 0,0)
        //bassSpeed is measure in gameTicks(if you use millis on update it's millisecond)
        void reset();
        void reset(BallPosition ballPos, BallVelocity ballDir);

        //move player player in direction direction
        //the user is moved instantly.
        //to get the update board call getBoard again
        void movePad(Player player, PadDirection direction);
        //Set pad position for given player
        //pindex = 0 for player 1 and
        //and 1 for player 2.
        void setPadPos(int pindex, int pos);

        //return true if ball got updated
        //return false if the ball doesn't get updated
        //this could be due to calling update too quick
        //game is not in running mode etc.
        //"now" is the current game ticks. You can use millis for this.
        bool update(unsigned long now);

        //write the board to given buffer
        //the buffer must be of the right size nrow*ncol
        void paint(int* board); //board assumes the correct size(nrow*ncol)
        bool isDirty(){return dirty;}//does the board need repaint?

        //return true if game is ended
        inline bool isGameEnded(){ return gameState==GameState::FINISHED; }

        //return Player that won
        //if game is not ended yet return UNKNOWN
        Player getWinner(){return this->winner;}


        //start the game
        //now is the game ticks(you can use millis)
        void start(unsigned long now);

        void setBallSpeed(int ballSpeed){this->ballSpeed = ballSpeed;}
    private:
        unsigned int nrow, ncol;
        int padPos[2]; //0 and 1
        int ballSpeed;
        BallPosition ballPos; //row and column top left is 0,0
        BallVelocity ballVelo; //ball moving direction
        unsigned long lastUpdate;
        GameState gameState;
        Player winner;
        void setBallPosition(BallPosition pos){ballPos = pos; dirty=true;}

        bool paddleRowCoverBallRow(int paddleRow, int ballRow);
        bool hitPlayerOne();
        bool hitPlayerTwo();
        Player hitPlayer();
        void bouncePlayer();
        bool hitTopWall();
        bool hitBottomWall();
        Wall hitWall();
        void bounceWall();
        void moveBall();
        void checkEndGame();
        inline void writeBuffer(int* buffer, int irow, int icol, int value){buffer[irow*ncol + icol]=value;}
        void resetBuffer(int* buffer);
        void paintPad(int* buffer, int padPos, int col);
        void paintBall(int* buffer);
        bool dirty;
        void resetPadPos();

    };
}

#endif