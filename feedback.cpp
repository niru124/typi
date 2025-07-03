#include "feedback.h"
#include <iostream>
#include <sstream>

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"

std::pair<int, int> showLiveFeedback2(const std::string &expected,
                                      const std::string &typed) {
  std::cout << "\r";
  int incorrect = 0, count = 0;
  std::stringstream ss_expected(expected), ss_typed(typed);
  std::string word_expected, word_typed;

  while (ss_expected >> word_expected) {
    if (ss_typed >> word_typed) {
      for (size_t i = 0; i < word_expected.size(); ++i) {
        if (i < word_typed.size()) {
          if (word_expected[i] == word_typed[i]) {
            std::cout << GREEN << word_typed[i] << RESET;
            count++;
          } else {
            incorrect++;
            std::cout << RED << word_typed[i] << RESET;
          }
        } else {
          std::cout << "_";
        }
      }
      if (word_typed.size() > word_expected.size()) {
        for (size_t i = word_expected.size(); i < word_typed.size(); ++i) {
          incorrect++;
          std::cout << RED << word_typed[i] << RESET;
        }
      }
    }
    std::cout << " ";
  }

  std::cout.flush();
  return {count, incorrect};
}
