#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <iomanip>

class BankAccount {
public:
    int id;
    double balance;
    mutable std::recursive_mutex mtx;

    BankAccount(int id, double balance) : id(id), balance(balance) {
    }

    // Deposits funds
    void deposit(double amount) {
        std::lock_guard<std::recursive_mutex> lock(mtx);
        balance += amount;
    }

    // Withdraws funds
    void withdraw(double amount) {
        std::lock_guard<std::recursive_mutex> lock(mtx);
        if(balance >= amount ) balance -= amount;
    }
};

// Safe transfer
void transfer(BankAccount& from, BankAccount& to, double amount) {
    if(from.id == to.id) return ;
    BankAccount& first = from.id < to.id ? from : to;
    BankAccount& second = from.id < to.id ? to : from;
    std::scoped_lock lock(first.mtx, second.mtx);
    if(from.balance >= amount) {
        from.withdraw(amount);
        to.deposit(amount);
    }
}

// Main entry
int main() {
    BankAccount acc1(1, 1000.0);
    BankAccount acc2(2, 1000.0);
    std::thread t1(transfer, std::ref(acc1), std::ref(acc2), 100.0);
    std::thread t2(transfer, std::ref(acc2), std::ref(acc1), 100.0);

    t1.join();
    t2.join();

    std::cout << "Account 1 balance: " << acc1.balance << std::endl;
    std::cout << "Account 2 balance: " << acc2.balance << std::endl;

    return 0;
}
