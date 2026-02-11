/*** includes ***/
#include <asm-generic/errno-base.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

/*** data ***/

struct termios originalTermios;

/*** terminal ***/

void die(const char *s){
  perror(s);
  exit(1);
}

void disableRawMode(){
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &originalTermios) == -1)
  die("tcsetattr");
}


void enableRawMode() {
  if (tcgetattr(STDIN_FILENO, &originalTermios) == -1) die("tcgetattr");

  struct termios raw = originalTermios;
  //disable specific terminal flags
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_iflag &= ~(OPOST); // turns off all processing
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

/*** init ***/

int main() {

    enableRawMode();
    while(1){

      // this variable stores a character when typed
      char c = '\0';

      if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN) die("read");

      if(iscntrl(c)){
        printf("%d\r\n",c);
      } else {
        printf("%d('%c')\r\n", c, c);        
      }

      if (c == 'q') break;
    }
    

    return 0;
}