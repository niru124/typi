#include "feedback.h"
#include "makegraph.h"
#include "terminal_utils.h"
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <unistd.h>
#include <vector>

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"

using namespace std;
using namespace chrono;

int main() {

  vector<string> words = {
      "umbrealla", "university", "camel", "boy",    "table",    "delivery",
      "apple",     "banana",     "cat",   "dog",    "elephant", "fish",
      "grape",     "hat",        "ice",   "jungle", "kite",     "lemon",
      "mountain",  "notebook",   "orange"};

  vector<double> y;

  string typed;
  printf("\033[2J\033[H"); // Clear screen
  printf("\033[?25l");     // Hide cursor

  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis(0, words.size() - 1);

  // Generate random word string
  string expected;
  vector<string> store;
  int n = 20;
  for (int i = 0; i < n; i++) {
    string temp = words[dis(gen)];
    expected += temp;
    if (i != n - 1)
      expected += " ";
    store.push_back(temp);
  }

  cout << GREEN << expected << endl;
  pair<int, int> res;

  enableRawMode();

  char c;
  high_resolution_clock::time_point total_start_time, total_end_time;
  high_resolution_clock::time_point word_start_time;

  bool started = false;
  string current_word;
  int correct_chars_total = 0;

  total_start_time = high_resolution_clock::now();
  word_start_time = total_start_time;

  while (true) {
    read(STDIN_FILENO, &c, 1);

    if (!started && isprint(c)) {
      total_start_time = high_resolution_clock::now();
      word_start_time = total_start_time;
      started = true;
    }

    if (c == 127 || c == 8) { // backspace
      if (!typed.empty())
        typed.pop_back();
      if (!current_word.empty())
        current_word.pop_back();
    } else if (c == '\n' || c == '\r') {
      // Handle last word
      if (!current_word.empty() && y.size() < store.size()) {
        auto word_end_time = high_resolution_clock::now();
        double duration_min =
            duration<double>(word_end_time - word_start_time).count() / 60.0;

        int correct = 0;
        string &expected_word = store[y.size()];
        size_t len = min(current_word.length(), expected_word.length());
        for (size_t i = 0; i < len; ++i) {
          if (current_word[i] == expected_word[i])
            correct++;
        }

        if (duration_min > 0.001) {
          correct_chars_total += correct;
          double wpm = (correct / 5.0) / duration_min;
          y.push_back(wpm);
        }
      }
      break;
    } else if (isprint(c)) {
      typed += c;

      if (c == ' ') {
        if (!current_word.empty() && y.size() < store.size()) {
          auto word_end_time = high_resolution_clock::now();
          double duration_min =
              duration<double>(word_end_time - word_start_time).count() / 60.0;

          int correct = 0;
          string &expected_word = store[y.size()];
          size_t len = min(current_word.length(), expected_word.length());
          for (size_t i = 0; i < len; ++i) {
            if (current_word[i] == expected_word[i])
              correct++;
          }

          if (duration_min > 0.001) {
            correct_chars_total += correct;
            double wpm = (correct / 5.0) / duration_min;
            y.push_back(wpm);
          }
        }
        current_word.clear();
        word_start_time = high_resolution_clock::now();
      } else {
        current_word += c;
      }
    }

    res = showLiveFeedback2(expected, typed);
  }

  total_end_time = high_resolution_clock::now();
  disableRawMode(); // restore terminal settings
  cout << "\n\n";

  duration<double> total_duration = total_end_time - total_start_time;
  double total_minutes = total_duration.count() / 60.0;

  int total_chars = expected.length() - (n - 1); // remove n-1 spaces
  int grossWPM = ((res.first / 5.0) / total_minutes);
  int netWPM = (grossWPM - (res.second / total_minutes));

  cout << fixed << setprecision(2);
  cout << "Time taken: " << total_duration.count() << " seconds ("
       << total_minutes << " minutes)" << endl;
  cout << "Correct: " << res.first << ", Incorrect: " << res.second << endl;
  cout << "Gross WPM: " << grossWPM << ", Net WPM: " << netWPM << endl;

  // if (typed == expected) {
  //   cout << GREEN << "Correct!" << RESET << endl;
  // } else {
  //   cout << RED << "Try again." << RESET << endl;
  // }

  // cout << "\nPer-word WPM values:\n";
  // for (size_t i = 0; i < y.size(); ++i) {
  //   cout << "Word " << i + 1 << ": " << y[i] << " WPM\n";
  // }
  vector<int> x;

  for (size_t i = 0; i < y.size(); ++i)
    x.push_back(i);

  make_graph(x, y);
  system("xdg-open plot_output.png");
  return 0;
}
