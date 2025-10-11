#ifndef FEEDBACK_H
#define FEEDBACK_H

#include "terminal_utils.h"
#include <string>
#include <utility>

std::pair<int, int> showLiveFeedback2(const std::string &expected,
                                      const std::string &typed);

#endif