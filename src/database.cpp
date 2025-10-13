#include "database.h"
#include <iostream>
#include <sqlite3.h>
#include <ctime>
#include <iomanip>
#include <sstream>

static int callback(void *data, int argc, char **argv, char **azColName) {
    return 0;
}

void initDatabase(const std::string& db_path) {
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open(db_path.c_str(), &db);

    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    const char* sql = "CREATE TABLE IF NOT EXISTS results("
                      "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                      "TIMESTAMP TEXT NOT NULL,"
                      "GROSS_WPM INTEGER NOT NULL,"
                      "NET_WPM INTEGER NOT NULL,"
                      "CORRECT_CHARS INTEGER NOT NULL,"
                      "INCORRECT_CHARS INTEGER NOT NULL,"
                      "TIME_TAKEN_SECONDS REAL NOT NULL);";

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
}

void insertResult(const std::string& db_path, const TypingResult& result) {
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open(db_path.c_str(), &db);

    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    std::string sql = "INSERT INTO results (TIMESTAMP, GROSS_WPM, NET_WPM, CORRECT_CHARS, INCORRECT_CHARS, TIME_TAKEN_SECONDS) VALUES ('";
    sql += result.timestamp + "', ";
    sql += std::to_string(result.gross_wpm) + ", ";
    sql += std::to_string(result.net_wpm) + ", ";
    sql += std::to_string(result.correct_chars) + ", ";
    sql += std::to_string(result.incorrect_chars) + ", ";
    sql += std::to_string(result.time_taken_seconds) + ");";

    rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    }
    sqlite3_close(db);
}

std::vector<TypingResult> getRecentResults(const std::string& db_path, int limit) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    std::vector<TypingResult> results;

    rc = sqlite3_open(db_path.c_str(), &db);
    if (rc) {
        std::cerr << "Can't open database: " << sqlite3_errmsg(db) << std::endl;
        return results;
    }

    std::string sql = "SELECT TIMESTAMP, GROSS_WPM, NET_WPM, CORRECT_CHARS, INCORRECT_CHARS, TIME_TAKEN_SECONDS FROM results ORDER BY ID DESC LIMIT " + std::to_string(limit) + ";";

    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);
    if (rc != SQLITE_OK) {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return results;
    }

    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        TypingResult res;
        res.timestamp = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        res.gross_wpm = sqlite3_column_int(stmt, 1);
        res.net_wpm = sqlite3_column_int(stmt, 2);
        res.correct_chars = sqlite3_column_int(stmt, 3);
        res.incorrect_chars = sqlite3_column_int(stmt, 4);
        res.time_taken_seconds = sqlite3_column_double(stmt, 5);
        results.push_back(res);
    }

    if (rc != SQLITE_DONE) {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return results;
}
