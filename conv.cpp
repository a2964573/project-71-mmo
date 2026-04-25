#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <syncstream>
#include <chrono>

std::mutex mtx;
std::condition_variable cv;
std::queue<int> jobQueue;
bool isServerRunning = true;

void workerThread(int id) {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        
        cv.wait(lock, [] { return !jobQueue.empty() || !isServerRunning; });

        if (!isServerRunning && jobQueue.empty()) break;

        int job = jobQueue.front();
        jobQueue.pop();
        
        lock.unlock(); 

        std::osyncstream(std::cout) << "Worker " << id << " 처리 완료: Job [" << job << "]" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // 작업하는 척 (0.5초)
    }
}

int main() {
    std::cout << "--- 서버 기동 ---" << std::endl;
    
    std::jthread w1(workerThread, 1);
    std::jthread w2(workerThread, 2);

    for (int i = 1; i <= 5; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        {
            std::lock_guard<std::mutex> lock(mtx);
            jobQueue.push(i);
            std::osyncstream(std::cout) << "Main: Job [" << i << "] 큐에 추가 및 알림 발송!" << std::endl;
        }
        cv.notify_one();
    }

    {
        std::lock_guard<std::mutex> lock(mtx);
        isServerRunning = false;
    }
    cv.notify_all();

    std::cout << "--- 메인 스레드 종료 대기 ---" << std::endl;
    return 0;
}
