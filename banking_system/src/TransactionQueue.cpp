#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>

enum TransactionType {
    DEPOSIT,
    WITHDRAW
};

struct Transaction {
    int id;
    TransactionType type;
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
    // Initializes queue
    TransactionQueue(size_t cap) : capacity(cap) {}

    // Adds transaction
    void push(Transaction t) {
        std::unique_lock<std::mutex> lock(mtx);
        cv_full.wait(lock, [this]() { return queue.size() < capacity; });
        queue.push(t);
        cv_empty.notify_one();
    }

    // Removes transaction
    Transaction pop() {
        std::unique_lock<std::mutex> lock(mtx);
        cv_empty.wait(lock, [this]() { return !queue.empty(); });
        Transaction t = queue.front();
        queue.pop();
        cv_full.notify_one();
        return t;
    }
};

// Producer thread
void customer(TransactionQueue& q, int start_id, int count) {
    for (int i = 0; i < count; ++i) {
        q.push({start_id + i, DEPOSIT, 100.0});
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

// Consumer thread
void teller(TransactionQueue& q, int count) {
    for (int i = 0; i < count; ++i) {
        q.pop();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

// Main entry
int main() {
    const int num_customers = 5;
    const int tx_per_customer = 1000;
    const int num_tellers = 3;
    const int total_tx = num_customers * tx_per_customer;
    
    TransactionQueue bank_queue(50);

    std::vector<std::thread> customers;
    for (int i = 0; i < num_customers; ++i) {
        customers.emplace_back(customer, std::ref(bank_queue), i * tx_per_customer, tx_per_customer);
    }

    std::vector<std::thread> tellers;
    int tx_per_teller = total_tx / num_tellers;
    for (int i = 0; i < num_tellers; ++i) {
        int count = (i == num_tellers - 1) ? (total_tx - (tx_per_teller * i)) : tx_per_teller;
        tellers.emplace_back(teller, std::ref(bank_queue), count);
    }

    for (auto& c : customers) {
        c.join();
    }
    for (auto& t : tellers) {
        t.join();
    }

    std::cout << "SUCCESS: All " << total_tx << " transactions processed!" << std::endl;

    return 0;
}
