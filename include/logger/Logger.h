#ifndef FILELOGGER_HPP
#define FILELOGGER_HPP

#include <fstream>
#include <iostream>
#include <deque>
#include <string>
#include <chrono>
#include <mutex>
#include <atomic>

namespace logger {


    class Logger {

    protected:
        static Logger* fileLogger_;
        static std::mutex m_genInstance;

        // ctor
        explicit Logger(const std::string& info, const char* fname, bool is_console);

        // dtor
        ~Logger();

    public:
        enum e_logType { LOG_ERROR, LOG_WARNING, LOG_INFO, LOG_MESSAGE };

        static Logger* getInstance(const std::string& info = "Messenger Logger", const char* fname = "logs.txt", bool is_console = false);

        std::string PrepTime();

        // Overload << operator using log type
        friend Logger& operator << (Logger& logger, e_logType l_type) {
            ++logger.lines;
            switch (l_type) {
            case logger::Logger::e_logType::LOG_ERROR:
            { 
                logger.writeTime("[ERR] : ");
                ++logger.numErrors;
            }
                break;

            case logger::Logger::e_logType::LOG_WARNING:
            {
                logger.writeTime("[WAR] : ");
                ++logger.numWarnings;
            }
                break;

            case logger::Logger::e_logType::LOG_MESSAGE:
            {
                logger.writeTime("[MSG] : ");
            }
                break;

            default:
            {
                logger.writeTime("[INF] : ");
            }
                break;
            } // sw
            return logger;
        }

        void writeTime(const std::string& msg);

        friend void operator << (Logger& logger, const std::string& text) {
            {
                std::lock_guard<std::mutex> guard(logger.m_writeFile);
                logger.myFile << text << std::endl;
                if (logger.is_console)
                {
                    std::cout << text << std::endl;
                }
            }
            //return logger;
        }

        // Make it Non Copyable
        Logger(const Logger&) = delete;
        void operator= (const Logger&) = delete;

    private:
        std::atomic<unsigned> lines = 0;
        unsigned _start = 0;
        std::fstream myFile;
        time_t _now;
        std::string name;
        bool is_console;

        unsigned int numWarnings;
        unsigned int numErrors;
        std::mutex m_writeFile;
    }; // class end

}  // namespace

#endif // FILELOGGER_HPP