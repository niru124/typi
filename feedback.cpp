#include "feedback.h"
#include <iostream>
#include <sstream>
#include <vector>

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"

std::pair<int, int> showLiveFeedback2(const std::string &expected,
                                      const std::string &typed) {
  std::cout << "\r\033[K"; // Clear the current line using carriage return and erase in display (CSI K) escape sequence

  int incorrect = 0, count = 0;

  // --- Calculate total count and incorrect first (logic for return values) ---
  std::stringstream ss_expected_calc(expected), ss_typed_calc(typed);
  std::string word_expected_calc, word_typed_calc;
  while (ss_expected_calc >> word_expected_calc) {
    if (ss_typed_calc >> word_typed_calc) {
      for (size_t i = 0; i < word_expected_calc.size(); ++i) {
        if (i < word_typed_calc.size()) {
          if (word_expected_calc[i] == word_typed_calc[i]) {
            count++;
          } else {
            incorrect++;
          }
        }
      }
      if (word_typed_calc.size() > word_expected_calc.size()) {
        incorrect += (word_typed_calc.size() - word_expected_calc.size());
      }
    }
  }
  // --- End of calculation logic ---

  // --- Display logic: Only show the current word with live feedback ---
  std::stringstream ss_expected_display(expected);
  std::vector<std::string> expected_words_vec;
  std::string temp_expected_word;
  while (ss_expected_display >> temp_expected_word) {
    expected_words_vec.push_back(temp_expected_word);
  }

  std::stringstream ss_typed_display(typed);
  std::vector<std::string> typed_words_vec;
  std::string temp_typed_word;
  while (ss_typed_display >> temp_typed_word) {
    typed_words_vec.push_back(temp_typed_word);
  }

  if (!typed_words_vec.empty()) {
    size_t current_word_index = typed_words_vec.size() - 1; // Index of the last typed word

    if (current_word_index < expected_words_vec.size()) {
      std::string current_expected_word = expected_words_vec[current_word_index];
      std::string current_typed_word = typed_words_vec[current_word_index];

      for (size_t i = 0; i < current_expected_word.size(); ++i) {
        if (i < current_typed_word.size()) {
          if (current_expected_word[i] == current_typed_word[i]) {
            std::cout << GREEN << current_typed_word[i] << RESET;
          } else {
            std::cout << RED << current_typed_word[i] << RESET;
          }
        } else {
          std::cout << current_expected_word[i]; // Untyped part of current word
        }
      }
      // Handle extra characters typed for the current word
      if (current_typed_word.size() > current_expected_word.size()) {
        for (size_t i = current_expected_word.size(); i < current_typed_word.size(); ++i) {
          std::cout << RED << current_typed_word[i] << RESET;
        }
      }
      // Add a space after the current word for better readability if more words are expected
      std::cout << " ";
    }
  }
  // --- End of display logic ---

  std::cout.flush();
  return {count, incorrect};
}
