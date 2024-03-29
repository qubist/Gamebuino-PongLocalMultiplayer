#include <SPI.h>
#include <Gamebuino.h>
Gamebuino gb = Gamebuino();

boolean paused = false;
//player variables
int player_score = 0;
int player_h = 16;
int player_w = 3;
int player_x = 0;
int player_y = (LCDHEIGHT-player_h)/2;
int player_vy = 2;
//oponent variables
int oponent_score = 0;
int oponent_h = 16;
int oponent_w = 3;
int oponent_x = LCDWIDTH-oponent_w;
int oponent_y = (LCDHEIGHT-oponent_h)/2;
int oponent_vy = 2;
//ball variables
int ball_size = 6;
int ball_x = LCDWIDTH - ball_size - oponent_w - 1;
int ball_y = (LCDHEIGHT-ball_size)/2;
int ball_vx = 3;
int ball_vy = 3;

extern const byte font3x5[]; //get the default small font
extern const byte font5x7[]; //get the default large font

///////////////////////////////////// SETUP
void setup() {
  gb.begin();
  gb.display.setFont(font5x7); //change the font to the large one
  gb.titleScreen(F("PongLocalMulti"));
  gb.pickRandomSeed(); //pick a different random seed each time for games to be different
  gb.battery.show = false; //hide the battery indicator
}

///////////////////////////////////// LOOP
void loop() {
  if(gb.update()){
    //pause the game if C is pressed
    if(gb.buttons.pressed(BTN_C)){
      gb.titleScreen(F("Pong - PAUSED"));
      gb.battery.show = false;
      gb.display.fontSize = 2;
    }
    //move the player
    if(gb.buttons.repeat(BTN_UP, 1)){
      player_y = max(0, player_y - player_vy);
    }
    if(gb.buttons.repeat(BTN_DOWN, 1)){
      player_y = min(LCDHEIGHT - player_h, player_y + player_vy);
    }

    //move the ball
    ball_x = ball_x + ball_vx;
    ball_y = ball_y + ball_vy;

    //check for ball collisions
    //collision with the top border
    if(ball_y < 0){
      ball_y = 0;
      ball_vy = -ball_vy;
      gb.sound.playTick();
    }
    //collision with the bottom border
    if((ball_y + ball_size) > LCDHEIGHT){
      ball_y = LCDHEIGHT - ball_size;
      ball_vy = -ball_vy;
      gb.sound.playTick();
    }
    //collision with the player
    if(gb.collideRectRect(ball_x, ball_y, ball_size, ball_size, player_x, player_y, player_w, player_h)){
      ball_x = player_x + player_w;
      ball_vx = -ball_vx;
      gb.sound.playTick();
    }
    //collision with the oponent
    if(gb.collideRectRect(ball_x, ball_y, ball_size, ball_size, oponent_x, oponent_y, oponent_w, oponent_h)){
      ball_x = oponent_x - ball_size;
      ball_vx = -ball_vx;
      gb.sound.playTick();
    }
    //collision with the left side
    if(ball_x < 0){
      oponent_score = oponent_score + 1;
      gb.sound.playCancel();
      ball_x = 0 + player_w + 16; //place the ball on the player side
      ball_vx = abs(ball_vx);
      ball_y = random(0,LCDHEIGHT-ball_size);
    }
    //collision with the right side
    if((ball_x + ball_size) > LCDWIDTH){
      player_score = player_score + 1;
      gb.sound.playCancel();
      ball_x = LCDWIDTH - ball_size - oponent_w - 16; //place the ball on the oponent side
      ball_vx = -abs(ball_vx);
      ball_y = random(0,LCDHEIGHT-ball_size);

    }
    //reset score when 10 is reached
    if((player_score == 10) || (oponent_score == 10)){
      player_score = 0;
      oponent_score = 0;
    }

    //move the oponent
    if(gb.buttons.repeat(BTN_B, 1)){
      oponent_y = max(0, oponent_y - oponent_vy);
    }
    if(gb.buttons.repeat(BTN_A, 1)){
      oponent_y = min(LCDHEIGHT - oponent_h, oponent_y + oponent_vy);
    }
  }

  //draw the score
  gb.display.setFont(font3x5); //change the font to the small one
  gb.display.fontSize = 2;
  gb.display.cursorX = 15;
  gb.display.cursorY = 16;
  gb.display.print(player_score);
  
  gb.display.cursorX = 57;
  gb.display.cursorY = 16;
  gb.display.print(oponent_score);
  gb.display.setFont(font5x7); //change the font back to the large one
  
  //draw the ball
  gb.display.fillRect(ball_x, ball_y, ball_size, ball_size);
  //draw the player
  gb.display.fillRect(player_x, player_y, player_w, player_h);
  //draw the oponent
  gb.display.fillRect(oponent_x,oponent_y, oponent_w, oponent_h);

}
