#include <fstream>
#include <chrono>
#include <ctime>
#include <string>

inline std::ofstream logfile("game.log");

inline std::string getTimeString() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm localtime =  *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&localtime, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

#define LOG(...) do {\
    logfile << __FILE__ << ":" << __LINE__ << ": " << getTimeString() << " ";\
    logfile << __VA_ARGS__ << "\n";\
    logfile.flush(); \
} while (0);