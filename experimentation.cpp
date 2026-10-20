#include <chrono>
#include <iostream>
#include <thread>
#include <queue>
#include <mutex>



int main(int argc, char** argv) {
    int N = 100;
    std::queue<const char*> q;
    std::mutex m;
    bool finished[2]{};

    std::thread t1([N, &m, &q, &finished]() {
        for (int i{}; i < N; i++) {
            const char* str = "|cat|";
            {
            std::lock_guard<std::mutex> lock(m);
            q.push(str);
            }
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
        finished[0] = true;
    });
    std::thread t2([N, &m, &q, &finished]() {
        for (int i{}; i < N; i++) {
            const char* str = "|dog|";
            {
            std::lock_guard<std::mutex> lock(m);
            q.push(str);
            }
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
        finished[1] = true;
    });
    std::thread printer([&m, &q, &finished]() {
        while (true){
            {
            std::lock_guard lock(m);
            if (q.empty() && finished[0] && finished[1]) break;
            const char* first = q.front();
            q.pop();
            std::cout << first;
            }
            std::this_thread::sleep_for(std::chrono::microseconds(1));
        }
    });
    t1.join();
    t2.join();
    printer.join();
    return 0;
}
