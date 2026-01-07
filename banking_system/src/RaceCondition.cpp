#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

class Counter {
public:
    int value = 0;
    std::mutex mtx;

    void increment() {
        std::lock_guard<std::mutex> lock(mtx);
        value++; 
    }
};

void worker(Counter& counter) {
    for (int i = 0; i < 1000; ++i) {
        counter.increment();
        std::this_thread::sleep_for(std::chrono::microseconds(1)); 
    }
}

int main() {
    Counter counter;
    std::vector<std::thread> threads;

    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(worker, std::ref(counter));
    }

    for (auto& t : threads) {
        t.join();
    }

    std::cout << "Actual:   " << counter.value << std::endl;

    if (counter.value != 10000) {
        std::cout << "DATA RACE DETECTED! (Result is wrong)" << std::endl;
    } else {
        std::cout << "SUCCESS! Thread-safe execution confirmed." << std::endl;
    }
    return 0;
}
