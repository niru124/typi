#include "feedback.h"
#include "terminal_utils.h"
#include <iostream>
#include <sstream>
#include <vector>

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define BG_RED "\033[41m"
#define UNDERLINE "\033[4m"
#define LAVENDER "\033[95m"
#define BLUE "\033[34m"
#define NO_UNDERLINE "\033[24m"

// Splits the string by whitespace into a vector of words.
std::vector<std::string> split_string(const std::string &s) {
  std::vector<std::string> words;
  std::stringstream ss(s);
  std::string word;
  while (ss >> word) {
    words.push_back(word);
  }
  return words;
}

std::pair<int, int> showLiveFeedback2(const std::string &expected,
                                      const std::string &typed) {
  static int lines_printed = 0;

  if (lines_printed > 0) {
    std::cout << "\033[" << lines_printed << "A";
  }
  std::cout << "\r\033[J";

  int term_width = getTerminalWidth();
  lines_printed = (expected.length() / term_width) + 1;

  int total_correct = 0;
  int total_incorrect = 0;

  std::vector<std::string> expected_words = split_string(expected);
  std::vector<std::string> typed_words = split_string(typed);
  bool trailing_space = !typed.empty() && isspace(typed.back());

  size_t typed_char_index = 0;

  for (size_t i = 0; i < expected_words.size(); ++i) {
    const std::string &expected_word = expected_words[i];

    if (i < typed_words.size()) {
      const std::string &typed_word = typed_words[i];

      // Render characters, comparing typed vs expected
      for (size_t j = 0; j < expected_word.length(); ++j) {
        bool is_cursor_pos =
            typed_char_index + j == typed.length() && !trailing_space;
        if (is_cursor_pos)
          std::cout << UNDERLINE;

        if (j < typed_word.length()) {
          if (typed_word[j] == expected_word[j]) {
            std::cout << GREEN << "\033[1m" << expected_word[j] << RESET;
            total_correct++;
          } else {
            std::cout << RED << "\033[1m" << expected_word[j] << RESET;
            total_incorrect++;
          }
        } else {
          if (i == typed_words.size() - 1 && !trailing_space) {
            std::cout << BLUE << "\033[1m" << expected_word[j] << RESET;
          } else {
            std::cout << "\033[1m" << expected_word[j] << RESET;
          }
        }
        if (is_cursor_pos)
          std::cout << NO_UNDERLINE;
      }

      // Render extra (over-typed) characters
      if (typed_word.length() > expected_word.length()) {
        std::cout << LAVENDER;
        for (size_t j = expected_word.length(); j < typed_word.length(); ++j) {
          bool is_cursor_pos =
              typed_char_index + j == typed.length() && !trailing_space;
          if (is_cursor_pos)
            std::cout << UNDERLINE;
          std::cout << typed_word[j];
          if (is_cursor_pos)
            std::cout << NO_UNDERLINE;
        }
        std::cout << RESET;
        total_incorrect += (typed_word.length() - expected_word.length());
      }
    } else {
      // This is an untyped word. Place cursor at the beginning if it belongs
      // here.
      bool is_cursor_pos =
          typed_char_index == typed.length() && !trailing_space;
      if (is_cursor_pos)
        std::cout << UNDERLINE;
      if (i == typed_words.size()) {
        std::cout << BLUE << "\033[1m" << expected_word << RESET;
      } else {
        std::cout << "\033[1m" << expected_word << RESET;
      }
      if (is_cursor_pos)
        std::cout << NO_UNDERLINE;
    }

    // Correctly advance the character counter and render the space
    if (i < expected_words.size() - 1) {
      if (i < typed_words.size()) {
        typed_char_index += typed_words[i].length() + 1;
      } else {
        typed_char_index += expected_words[i].length() + 1;
      }

      bool is_cursor_pos =
          (typed_char_index - 1 == typed.length()) ||
          (trailing_space && typed_char_index - 1 == typed.length() - 1);

      if (is_cursor_pos) {
        std::cout << UNDERLINE << " " << NO_UNDERLINE;
      } else {
        std::cout << " ";
      }
    }
  }

  // Final check for cursor at the very end of the text
  if (typed.length() > 0 &&
      typed.length() == typed_char_index + typed_words.back().length()) {
    std::cout << UNDERLINE << " " << NO_UNDERLINE;
  }

  std::cout << std::endl;
  std::cout.flush();

  return {total_correct, total_incorrect};
}
