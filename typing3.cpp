#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <vector>

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
using namespace std;
using namespace chrono;
struct termios orig_termios;

void disableRawMode() { tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios); }

pair<int, int> showLiveFeedback2(const string &expected, const string &typed) {
  cout << "\r"; // Move cursor to beginning
  int incorrect = 0;
  int count = 0;
  stringstream ss_expected(expected);
  stringstream ss_typed(typed);

  string word_expected, word_typed;

  while (ss_expected >>
         word_expected) {         // reads until space is hit (the expected)
    if (ss_typed >> word_typed) { // reads until space is hit (the typed)
      for (size_t i = 0; i < word_expected.size(); ++i) {
        if (i < word_typed.size()) {
          if (word_expected[i] == word_typed[i]) {
            cout << GREEN << word_typed[i] << RESET;
            count++;
          } else {
            incorrect++;
            cout << RED << word_typed[i] << RESET;
          }
        } else {
          cout << "_";
        }
      }
      // Extra letters typed by user
      if (word_typed.size() > word_expected.size()) {
        for (int i = word_expected.size(); i < word_typed.size(); ++i) {
          incorrect++;
          cout << RED << word_typed[i] << RESET;
        }
      }
    } else {
      // No word typed yet
      // for (char ch : word_expected)
      //   cout << "_";
    }
    cout << " ";
  }
  cout.flush();
  return {count, incorrect};
}

// int showLiveFeedback(const string &expected, const string &typed) {
//   cout << "\r"; // return cursor to start of line
//   int count=0;
//   for (size_t i = 0; i < expected.length(); ++i) {
//     if (i < typed.length()) {
//       if (typed[i] == expected[i]) {
//         cout << GREEN << typed[i] << RESET;
//         count++;
//       } else
//         cout << RED << typed[i] << RESET;
//     } else {
//       cout << "_";
//     }
//   }
//   cout << " "; // clear extra trailing characters
//   cout.flush();
//   return count;
// }

void enableRawMode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disableRawMode); // ensure terminal resets on exit

  struct termios raw = orig_termios;
  raw.c_lflag &= ~(ECHO | ICANON); // disable echo and canonical mode
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
  vector<string> words = {
      "umbrealla", "university", "camel", "boy",    "table",    "delivery",
      "apple",     "banana",     "cat",   "dog",    "elephant", "fish",
      "grape",     "hat",        "ice",   "jungle", "kite",     "lemon",
      "mountain",  "notebook",   "orange"};

  string typed;

  printf("\033[2J\033[H");
  printf("\033[?25l");
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, words.size() - 1);

  // Generate 5-word string
  string expected;
  vector<string> store;
  int lenthi = 0;
  int n = 20;
  for (int i = 0; i < n; i++) {
    string temp = words[dis(gen)];
    expected.append(temp);
    if (i != n - 1) {
      expected += " ";
    }
    store.push_back(temp);
    lenthi += temp.length();
  }

  cout << GREEN << expected << endl;
  pair<int, int> res;
  enableRawMode();
  char c;
  chrono::high_resolution_clock::time_point start_time;
  bool flg = false;
  while (true) {
    read(STDIN_FILENO, &c, 1);
    if (c && !flg) {
      // cout << "hi" << endl; // perfectly working
      start_time = chrono::high_resolution_clock::now();
      flg = true;
    }
    if (c == 127 || c == 8) { // handle backspace
      if (!typed.empty())
        typed.pop_back();
    } else if (c == '\n' || c == '\r') {
      break;
    } else if (isprint(c)) {
      typed += c;
    }

    res = showLiveFeedback2(expected, typed);
  }
  auto end_time = chrono::high_resolution_clock::now();

  duration<double> duration_seconds =
      duration_cast<duration<double>>(end_time - start_time);

  // Now convert to minutes
  duration<double, ratio<60>> duration_minutes =
      duration_cast<duration<double, ratio<60>>>(end_time - start_time);

  cout << fixed << setprecision(4);
  cout << endl;
  cout << "Time taken: " << duration_seconds.count() << " seconds" << " or "
       << duration_minutes.count() << " minutes" << endl;
  cout << "correct: " << res.first << " incorrect: " << res.second << endl;
  int total = expected.length() - n + 1;
  int grossWPM = ((res.first / 5.0) / duration_minutes.count());
  cout << "Your gross WPM is " << grossWPM;
  int netWPM = (grossWPM - (res.second / duration_minutes.count()));
  cout << "Your net WPM is " << netWPM << endl;

  disableRawMode(); // restore terminal settings

  cout << "\n\nFinal input: " << typed << " which is " << res.first
       << " out of " << total << endl;
  if (typed == expected) {
    cout << GREEN << "Correct!" << RESET << endl;
  } else {
    cout << RED << "Try again." << RESET << endl;
  }

  return 0;
}
