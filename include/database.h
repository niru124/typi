#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <tuple>
#include <chrono>

struct TypingResult {
    std::string timestamp;
    int gross_wpm;
    int net_wpm;
    int correct_chars;
    int incorrect_chars;
    double time_taken_seconds;
};

struct TypingEvent {
    char character;
    std::chrono::high_resolution_clock::time_point timestamp;
};


void initDatabase(const std::string& db_path);
void insertResult(const std::string& db_path, const TypingResult& result);
std::vector<TypingResult> getRecentResults(const std::string& db_path, int limit);

#endif // DATABASE_H
