#include "terminal_utils.h"
#include <iostream>
#include <string>
#include <termios.h>
#include <unistd.h>

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"

static struct termios orig_termios;

void disableRawMode() { tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios); }

void enableRawMode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disableRawMode); // ensure terminal resets on exit

  struct termios raw = orig_termios;
  raw.c_lflag &= ~(ECHO | ICANON); // disable echo and canonical mode
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

pair<int, int> showLiveFeedback2(const string &expected, const string &typed) {
  int correct_chars = 0;
  int incorrect_chars = 0;

  printf("\033[H"); // Move cursor to home

  for (size_t i = 0; i < expected.length(); ++i) {
    if (i < typed.length()) {
      if (typed[i] == expected[i]) {
        cout << GREEN << expected[i];
        correct_chars++;
      } else {
        cout << RED << expected[i];
        incorrect_chars++;
      }
    } else {
      cout << RESET << expected[i];
    }
  }
  cout << RESET; // Reset color after printing
  fflush(stdout); // Flush output to ensure it's displayed immediately

  return {correct_chars, incorrect_chars};
}
