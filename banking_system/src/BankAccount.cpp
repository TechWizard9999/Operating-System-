#include <iostream>
#include <vector>
#include <thread>
#include <iomanip>
#include <mutex>

class BankAccount {
private:
    double balance;
    mutable std::mutex mtx;

public:
    // Initializes balance
    BankAccount() : balance(0.0) {}

    // Deposits money
    void deposit(double amount) {
        std::lock_guard<std::mutex> lock(mtx);
        balance += amount;
    }

    // Returns balance
    double getBalance() const {
        std::lock_guard<std::mutex> lock(mtx);
        return balance;
    }
};

// Thread task
void depositTask(BankAccount& account, int iterations, double amount) {
    for (int i = 0; i < iterations; ++i) {
        account.deposit(amount);
    }
}

// Main entry
int main() {
    BankAccount account;
    const int numThreads = 10;
    const int iterationsPerThread = 1000;
    const double depositAmount = 100.0;
    const double expectedBalance = numThreads * iterationsPerThread * depositAmount;


    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(depositTask, std::ref(account), iterationsPerThread, depositAmount);
    }

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}
