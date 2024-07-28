#include <stdio.h>
#include <termios.h>
#include <unistd.h>

void arrowKeys(struct termios oldt, struct termios newt, int ch) {
    tcgetattr(STDIN_FILENO, &oldt); // Get terminal attributes
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Set new terminal attributes

        ch = getchar();
    
        if (ch == '\033') { // Escape sequence for arrow keys
            ch = getchar(); // Skip '['
            switch(getchar()) {
                case 'A':
                    printf("Up arrow pressed\n");
                    break;
                case 'B':
                    printf("Down arrow pressed\n");
                    break;
                case 'C':
                    printf("Right arrow pressed\n");
                    break;
                case 'D':
                    printf("Left arrow pressed\n");
                    break;
                default:
                    printf("Unknown escape sequence\n");
                    break;
            }
        }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // Restore old terminal attributes
}

int main() {
    struct termios oldt, newt;
    int ch;
    
    arrowKeys(oldt, newt, ch);

    return 0;
}
