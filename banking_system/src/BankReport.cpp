#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <chrono>

class BankAccount {
private:
    double balance;
    mutable std::shared_mutex mtx;

public:
    BankAccount(double initial_balance) : balance(initial_balance) {}

    // exclusive lock
    void deposit(double amount) {
        std::unique_lock<std::shared_mutex> lock(mtx);
        balance += amount;
    }

    // exclusive lock
    void withdraw(double amount) {
        std::unique_lock<std::shared_mutex> lock(mtx);
        if (balance >= amount) balance -= amount;
    }

    // shared lock
    double getBalance() const {
        std::shared_lock<std::shared_mutex> lock(mtx);
        return balance;
    }
};

void writer(BankAccount& acc) {
    for (int i = 0; i < 1000; ++i) {
        acc.deposit(1.0);
        acc.withdraw(1.0);
    }
}

void reader(BankAccount& acc) {
    for (int i = 0; i < 5000; ++i) {
        acc.getBalance();
    }
}

int main() {
    BankAccount account(1000.0);
    std::vector<std::thread> threads;

    // spawn 4 writers
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back(writer, std::ref(account));
    }

    // spawn 20 readers
    for (int i = 0; i < 20; ++i) {
        threads.emplace_back(reader, std::ref(account));
    }

    for (auto& t : threads) t.join();


    return 0;
}
