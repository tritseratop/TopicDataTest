#include "Logger.h"
#include <iomanip> 

using namespace logger;

Logger::Logger(
    const std::string& info, 
    const char* fname = "logger.txt", 
    bool is_console = false)
    : numWarnings(0U)
    , numErrors(0U)
    , name(fname)
    , is_console(is_console)
{
    time(&_now);
    myFile.open(fname, std::ofstream::out | std::ofstream::trunc);

    // Write the first lines
    if (myFile.is_open()) {
        *this << e_logType::LOG_INFO << "Log file created";
        *this << e_logType::LOG_INFO << "Log file info: " + info;
    } // if
    _start = myFile.tellp();
}

Logger::~Logger() {
    if (myFile.is_open()) {
        myFile << std::endl << std::endl;

        // Report number of errors and warnings
        myFile << numWarnings << " warnings" << std::endl;
        myFile << numErrors << " errors" << std::endl;
        if (is_console)
        {
            std::cout << numWarnings << " warnings" << std::endl;
            std::cout << numErrors << " errors" << std::endl;
        }
        myFile.close();
    } // if
}

void Logger::writeTime(const std::string& logType) {
    std::string msg = PrepTime() + logType;
    if (true) {
        std::lock_guard<std::mutex> guard(m_writeFile);
        myFile << msg;
        if (is_console)
        {
            std::cout << msg;
        }
    }
    else {

    }
}

std::string Logger::PrepTime() {
    time(&_now);
    struct tm* t;
    t = localtime(&_now);
    std::string s, m, h, D, M, Y;
    s = std::to_string(t->tm_sec);
    m = std::to_string(t->tm_min);
    h = std::to_string(t->tm_hour);
    D = std::to_string(t->tm_mday);
    M = std::to_string(t->tm_mon + 1);
    Y = std::to_string(t->tm_year + 1900);
    if (t->tm_sec < 10) s = "0" + s;
    if (t->tm_min < 10) m = "0" + m;
    if (t->tm_hour < 10) h = "0" + h;
    if (t->tm_mday < 10) D = "0" + D;
    if (t->tm_mon + 1 < 10) M = "0" + M;

    std::string res =
        Y + "-" + M + "-" + D + " " + h + ":" + m + ":" + s + " ";

    return res;
}

Logger* Logger::fileLogger_ = nullptr;

std::mutex Logger::m_genInstance;

Logger* Logger::getInstance(const std::string& info, const char* fname, bool is_console) {
    {
        std::lock_guard<std::mutex> guard(Logger::m_genInstance);
        if (fileLogger_ == nullptr) {
            fileLogger_ = new Logger(info, fname, is_console);
        }
    }
    return fileLogger_;
}