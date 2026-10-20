#include <iostream>
#include <termios.h>
#include <unistd.h>

int main(int argc, char** argv) {
    termios old_settings{};
    tcgetattr(STDIN_FILENO, &old_settings);

    termios new_settings = old_settings;
    new_settings.c_lflag &= ~(ICANON | ECHO);
    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);

    char c;
    while (read(STDIN_FILENO, &c, 1) == 1) {
        if (c == 'q') {
            break;
        }

        std::cout << "You pressed: " << c << '\n';
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);
}