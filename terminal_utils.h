#ifndef TERMINAL_UTILS_H
#define TERMINAL_UTILS_H

#include <string>
#include <utility> // For std::pair

using namespace std;

void disableRawMode();
void enableRawMode();
pair<int, int> showLiveFeedback2(const string &expected, const string &typed);

#endif // TERMINAL_UTILS_H