#include "feedback.h"
#include "makegraph.h"
#include "terminal_utils.h"
#include "database.h"
#include <chrono>
#include <cstdlib>
#include <fstream> // For file operations
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <unistd.h>
#include <vector>
#include <ctime>
#include <algorithm> // For std::shuffle

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"

const std::string DB_PATH = "typing_results.db";

using namespace std;
using namespace chrono;

// Function to load words from a file
vector<string> loadWordsFromFile(const string &filepath) {
  vector<string> words;
  ifstream file(filepath);
  if (file.is_open()) {
    string word;
    while (file >> word) {
      words.push_back(word);
    }
    file.close();
  } else {
    cerr << "Error: Unable to open words file: " << filepath << endl;
  }
  return words;
}

int main(int argc, char *argv[]) {
  initDatabase(DB_PATH);

  int num_words_to_type = 20; // Default number of words
  bool show_progress = false;
  bool start_test = true;

  // Parse command-line arguments
  for (int i = 1; i < argc; ++i) {
    string arg = argv[i];
    if (arg == "-n" && i + 1 < argc) {
      try {
        num_words_to_type = stoi(argv[++i]);
        if (num_words_to_type <= 0) {
          cerr << "Error: Number of words must be positive." << endl;
          return 1;
        }
      } catch (const invalid_argument &e) {
        cerr << "Error: Invalid number for -n." << endl;
        return 1;
      } catch (const out_of_range &e) {
        cerr << "Error: Number for -n out of range." << endl;
        return 1;
      }
    } else if (arg == "-sp") {
      show_progress = true;
      start_test = false; // If showing progress, don't start a test by default
    } else if (arg == "-h" || arg == "--help") {
      cout << "Usage: typi [-n <number_of_words>] [-sp]\n";
      cout << "  -n <number_of_words> : Specify the number of words for the typing test (default: 20).\n";
      cout << "  -sp                  : Show typing progress.\n";
      return 0;
    }
  }

  if (show_progress) {
    printf("\033[2J\033[H"); // Clear screen
    printBold("Your Typing Progress\n");
    vector<TypingResult> recentResults = getRecentResults(DB_PATH, 10); // Get last 10 results

    if (recentResults.empty()) {
      cout << "No results yet. Start a test to see your progress!\n";
    } else {
      cout << left << setw(20) << "Timestamp"
           << setw(12) << "Gross WPM"
           << setw(10) << "Net WPM"
           << setw(10) << "Correct"
           << setw(12) << "Incorrect"
           << setw(10) << "Time (s)" << endl;
      cout << string(80, '-') << endl;
      for (const auto &r : recentResults) {
        cout << left << setw(20) << r.timestamp
             << setw(12) << r.gross_wpm
             << setw(10) << r.net_wpm
             << setw(10) << r.correct_chars
             << setw(12) << r.incorrect_chars
             << setw(10) << fixed << setprecision(2) << r.time_taken_seconds << endl;
      }
    }
    cout << "\nPress Enter to return to menu...";
    cin.ignore(); // Consume the newline character left by previous cin
    cin.get();    // Wait for user to press Enter
  } else if (start_test) {
    vector<string> all_words = loadWordsFromFile("/home/nirantar/Downloads/Downloads/bashi/cpp/files/typi/src/words.txt");
    if (all_words.empty()) {
      cerr << "No words loaded. Exiting test." << endl;
      cin.ignore();
      cin.get();
      return 1;
    }

    // Shuffle and select words
    random_device rd;
    mt19937 gen(rd());
    shuffle(all_words.begin(), all_words.end(), gen);

    vector<string> words;
    for (int i = 0; i < num_words_to_type && i < all_words.size(); ++i) {
      words.push_back(all_words[i]);
    }

    vector<double> y;

    string typed;
    printf("\033[2J\033[H"); // Clear screen
    printf("\033[?25l");     // Hide cursor

    // Generate random word string
    string expected;
    vector<string> store;
    int n = words.size(); // Use the actual number of words selected
    for (int i = 0; i < n; i++) {
      string temp = words[i];
      expected += temp;
      if (i != n - 1)
        expected += " ";
      store.push_back(temp);
    }

    showLiveFeedback2(expected, typed);
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
    printBold("Time taken: ");
    cout << total_duration.count() << " seconds (" << total_minutes << " minutes)" << endl;
    printBold("Correct: ");
    cout << res.first << ", Incorrect: " << res.second << endl;
    printBold("Gross WPM: ");
    cout << grossWPM << ", Net WPM: " << netWPM << endl;

    // Store results in database
    TypingResult result;
    time_t now = time(0);
    tm *ltm = localtime(&now);
    stringstream ss;
    ss << put_time(ltm, "%Y-%m-%d %H:%M:%S");
    result.timestamp = ss.str();
    result.gross_wpm = grossWPM;
    result.net_wpm = netWPM;
    result.correct_chars = res.first;
    result.incorrect_chars = res.second;
    result.time_taken_seconds = total_duration.count();
    insertResult(DB_PATH, result);

    vector<int> x;

    for (size_t i = 0; i < y.size(); ++i)
      x.push_back(i);

    make_graph(x, y);
    system("xdg-open plot_output.png");
  }

  return 0;
}
