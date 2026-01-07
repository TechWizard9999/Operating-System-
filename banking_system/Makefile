CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread
TSAN_FLAGS = -fsanitize=thread -g

SRC_DIR = src
BIN_DIR = bin

.PHONY: all clean level1 level1_tsan level2 level2_tsan level3 level3_tsan

all: level1 level2 level3 level4 level5 level6

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# Level 1: Basic Thread-Safe Bank Account
level1: $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(SRC_DIR)/BankAccount.cpp -o $(BIN_DIR)/level1

level1_tsan: $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(TSAN_FLAGS) $(SRC_DIR)/BankAccount.cpp -o $(BIN_DIR)/level1_tsan

# Level 2: Producer-Consumer Queue
level2: $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(SRC_DIR)/TransactionQueue.cpp -o $(BIN_DIR)/level2

level2_tsan: $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(TSAN_FLAGS) $(SRC_DIR)/TransactionQueue.cpp -o $(BIN_DIR)/level2_tsan

# Level 3: Safe Transfer
level3: $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(SRC_DIR)/SafeTransfer.cpp -o $(BIN_DIR)/level3

level3_tsan: $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(TSAN_FLAGS) $(SRC_DIR)/SafeTransfer.cpp -o $(BIN_DIR)/level3_tsan

# Level 4: Reader-Writer Bank Reports
level4: $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(SRC_DIR)/BankReport.cpp -o $(BIN_DIR)/level4

level4_tsan: $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(TSAN_FLAGS) $(SRC_DIR)/BankReport.cpp -o $(BIN_DIR)/level4_tsan

# Level 5: Complete Banking System Integration
level5: $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(SRC_DIR)/BankSystem.cpp -o $(BIN_DIR)/level5

level5_tsan: $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(TSAN_FLAGS) $(SRC_DIR)/BankSystem.cpp -o $(BIN_DIR)/level5_tsan

# Level 6: Debugging & Race Conditions
level6: $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(SRC_DIR)/RaceCondition.cpp -o $(BIN_DIR)/level6

level6_tsan: $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(TSAN_FLAGS) $(SRC_DIR)/RaceCondition.cpp -o $(BIN_DIR)/level6_tsan

clean:
	rm -rf $(BIN_DIR)
