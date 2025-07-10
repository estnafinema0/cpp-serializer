CXX := g++
CXXFLAGS := -std=c++17 -Iinclude -Wall -Wextra -pthread

SRC_DIR := src
INCLUDE_DIR := include
TEST_DIR := tests
BUILD_DIR := build

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

TEST_SRCS := $(wildcard $(TEST_DIR)/test_*.cpp)
TEST_OBJS := $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(TEST_SRCS))
TEST_EXES := $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)/%_test,$(TEST_SRCS))

.PHONY: all test clean

all: $(BUILD_DIR) $(OBJS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%_test: $(BUILD_DIR)/%.o $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -lgtest -lgtest_main -o $@

test: all $(TEST_EXES)
	@for exe in $(TEST_EXES); do \
	  echo "Running $$exe"; \
	  ./$$exe; \
	done

clean:
	rm -rf $(BUILD_DIR)
