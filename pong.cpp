#include <unistd.h>
#include <ncurses.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <string>
#include <ctype.h>

void display(char board[][80], int score[], int ball_pos[]){
  usleep(10000);
  int i;
  clear();
  for(i=0; i<24; i++){
    mvprintw(i, 0, board[i]);
  }
  mvprintw(1, 37, std::to_string(score[0]).c_str());
  mvprintw(1, 43, std::to_string(score[1]).c_str());
  mvprintw(ball_pos[0], ball_pos[1], "o");
  //mvprintw(1, 10, std::to_string(paddle_pos[0]).c_str());
  //mvprintw(1, 50, std::to_string(paddle_pos[1]).c_str());
}

void move_paddle(int side, int dir, char board[][80], int *paddle){
  int i;
  if(side==2){
    side = 76;
  }
  if(*paddle<2 && dir){
    return;
  }
  if(*paddle>20 && dir==0){
    return;
  }
  if(!dir){
    for(i=*paddle; i<24; i++){
      board[i][side] = ' ';
    }
    *paddle+=1;
    for(i=*paddle; i<*paddle+3; i++){
      board[i][side] = '#';
    }
  }else{
    for(i=*paddle; i<24; i++){
      board[i][side] = ' ';
    }
    *paddle-=1;
    for(i=*paddle; i<*paddle+3; i++){
      board[i][side] = '#';
    }
  }
  for(i=0; i<79; i++){
    board[0][i] = '*';
    board[23][i] = '*';
  }
}

int get_dir(int dir){
  switch(dir){
  case 1:
    return 7;
  case 2:
    return 8;
  case 3:
    return 9;
  case 4:
    return 6;
  case 6:
    return 4;
  case 7:
    return 1;
  case 8:
    return 2;
  case 9:
    return 3;
  default:
    return 0;
    break;
  }
}

void move_ball(char board[][80], int score[], int ball_pos[], int *dir, int paddle_pos[]){
  int next_pos[2] = {12, 40};
  if(board[ball_pos[0] - 1][ball_pos[1]] == '|'){
    board[ball_pos[0]][ball_pos[1]] = '|';
  }else{
    board[ball_pos[0]][ball_pos[1]] = ' ';
  }

  switch(*dir){
  case 1:
    next_pos[1] = ball_pos[1]-1;
    next_pos[0] = ball_pos[0]+1;
    break;
  case 2:
    next_pos[1] = ball_pos[1];
    next_pos[0] = ball_pos[0]+1;
    break;
  case 3:
    next_pos[1] = ball_pos[1]+1;
    next_pos[0] = ball_pos[0]+1;
    break;
  case 4:
    next_pos[1] = ball_pos[1]-1;
    next_pos[0] = ball_pos[0];
    break;
  case 6:
    next_pos[1] = ball_pos[1]+1;
    next_pos[0] = ball_pos[0];
    break;
  case 7:
    next_pos[1] = ball_pos[1]-1;
    next_pos[0] = ball_pos[0]-1;
    break;
  case 8:
    next_pos[1] = ball_pos[1];
    next_pos[0] = ball_pos[0]-1;
    break;
  case 9:
    next_pos[1] = ball_pos[1]+1;
    next_pos[0] = ball_pos[0]-1;
    break;
  default:
    break;
  }

  if(board[next_pos[0]][next_pos[1]] == '#'){
    if(next_pos[1]>40){
      if(next_pos[0] == paddle_pos[1]){
	*dir = 7;
      }else if(next_pos[0] == paddle_pos[1]+1){
	*dir = 4;
      }else if(next_pos[0] == paddle_pos[1]+2){
	*dir = 1;
      }
    }else{
      if(next_pos[0] == paddle_pos[0]){
	*dir = 9;
      }else if(next_pos[0] == paddle_pos[0]+1){
	*dir = 6;
      }else if(next_pos[0] == paddle_pos[0]+2){
	*dir = 3;
      }
    }
    return;
  }else if(board[next_pos[0]][next_pos[1]] == '*'){
    *dir = get_dir(*dir);
    return;
  }else if(board[next_pos[0]][next_pos[1]] == '/'){
    score[1]+=1;
    next_pos[0] = 12;
    next_pos[1] = 40;
    do{
      *dir = rand()%9 + 1;
    }while(*dir==8 || *dir==2 || *dir==5);
  }else if(board[next_pos[0]][next_pos[1]] == '\\'){
    score[0]+=1;
    next_pos[0] = 12;
    next_pos[1] = 40;
    do{
      *dir = rand()%9 + 1;
    }while(*dir==8 || *dir==2 || *dir==5);
  }

  //printw(std::to_string(next_pos[1]).c_str());
  refresh();
  usleep(50000);
  ball_pos[0] = next_pos[0];
  ball_pos[1] = next_pos[1];
}

void place_paddles(char board[][80]){
  int i;
  for(i=0;i<3;i++){
    board[i+11][1] = '#';
    board[i+11][76] = '#';
  }
}

int main(int argc, char *argv[]){
  srand(time(0));
  int score[2] = {0, 0};
  int paddle_pos[2] = {11, 11};
  int ball_pos[2] = {12, 40};
  int i, j, in;
  int dir = 6;
  bool playing = true;
  char board[24][80];
  char line[80] = "/                                       |                                     \\";
  for(i=0; i<24; i++){
    for(j=0; j<=79; j++){
      board[i][j] = line[j];
      if(j>=79){
	continue;
      }
      if(i==0 || i==23){
	board[i][j] = '*';
      }
    }
  }
  board[ball_pos[0]][ball_pos[1]] = 'o';
  std::string art[8] = {"88888b.  .d88b.  88888b.  .d88b.  \n",
			"888 \"88bd88""88b88888 \"88bd88P\"88b \n",
			"888  888888  888888  888888  888 \n",
			"888 d88PY88..88P888  888Y88b 888 \n",
			"88888P\"  \"Y88P\" 888  888 \"Y88888 \n",
			"888                          888 \n",
			"888                     Y8b d88P \n",
			"888                      \"Y88P\"  "};

  initscr();
  raw();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  for(i=0; i<8; i++){
    mvprintw(6+i, 24, art[i].c_str());
  }
  mvprintw(8+i, 22, "Press any key to start, ESC to quit");
  in = getch();
  if(in == 27){
    endwin();
    return 0;
  }
  nodelay(stdscr, TRUE);
  place_paddles(board);
  while(playing){
    in = getch();
    if(in == 27){
      playing = false;
    }

    //display(board, score, ball_pos);

    switch (in){
    case KEY_UP:
      move_paddle(2, 1, board, &paddle_pos[1]);
      break;
    case KEY_DOWN:
      move_paddle(2, 0, board, &paddle_pos[1]);
      break;
    case 'q':
      move_paddle(1, 1, board, &paddle_pos[0]);
      break;
    case 'a':
      move_paddle(1, 0, board, &paddle_pos[0]);
      break;
    default:
      break;
    }

    move_ball(board, score, ball_pos, &dir, paddle_pos);
    display(board, score, ball_pos);
    refresh();
  }
  
  endwin();
  return 0;
}
