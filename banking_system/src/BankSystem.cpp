#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <queue>
#include <map>
#include <chrono>
#include <random>

class BankAccount {
public:
    int id;
    double balance;
    mutable std::shared_mutex mtx;

    BankAccount(int id, double balance) : id(id), balance(balance) {}

    void deposit(double amount) {
        std::unique_lock<std::shared_mutex> lock(mtx);
        balance += amount;
    }
    void withdraw(double amount) {
        std::unique_lock<std::shared_mutex> lock(mtx);
        balance -= amount;
    }
    double getBalance() const {
        std::shared_lock<std::shared_mutex> lock(mtx);
        return balance;
    }
};

struct Transaction {
    int fromId;
    int toId;
    double amount;
};

class TransactionQueue {
private:
    std::queue<Transaction> queue;
    size_t capacity;
    std::mutex mtx;
    std::condition_variable cv_full;
    std::condition_variable cv_empty;

public:
    TransactionQueue(size_t cap) : capacity(cap) {}

    void push(Transaction t) {
        std::unique_lock<std::mutex> lock(mtx);
        while (queue.size() >= capacity) cv_full.wait(lock);
        queue.push(t);
        cv_empty.notify_one();
    }

    Transaction pop() {
        std::unique_lock<std::mutex> lock(mtx);
        while (queue.empty()) cv_empty.wait(lock);
        Transaction t = queue.front();
        queue.pop();
        cv_full.notify_one();
        return t;
    }
};

class Bank {
private:
    std::map<int, BankAccount*> accounts;
    TransactionQueue& queue;

public:
    Bank(TransactionQueue& q) : queue(q) {}

    void addAccount(int id, double initial_balance) {
        accounts[id] = new BankAccount(id, initial_balance);
    }

    void processTransfer(int fromId, int toId, double amount) {
        
    }
    void runAudit() const {
        
    }
};

void customer(TransactionQueue& q) {
    for(int i = 0; i < 100; ++i) {
        int u1 = (rand() % 5) + 1;
        int u2 = (rand() % 5) + 1;
        if (u1 != u2) {
            q.push({ u1, u2, 10.0 });
        }
    }
}

void bankWorker(Bank& bank, TransactionQueue& q) {
    for (int i = 0; i < 100; ++i) {
        Transaction t = q.pop();
        bank.processTransfer(t.fromId, t.toId, t.amount);
    }
}

int main() {
    srand(time(0));
    TransactionQueue central_queue(50);
    Bank myBank(central_queue);

    for (int i = 1; i <= 5; ++i) myBank.addAccount(i, 1000.0);

    std::vector<std::thread> threads;
    
    for(int i=0; i<2; ++i) threads.emplace_back(customer, std::ref(central_queue));

    for(int i=0; i<2; ++i) threads.emplace_back(bankWorker, std::ref(myBank), std::ref(central_queue));

    threads.emplace_back([&]() {
        for(int i=0; i<5; ++i) {
            myBank.runAudit();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    });

    for (auto& t : threads) t.join();

    return 0;
}
