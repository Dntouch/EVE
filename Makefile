CXX ?= c++

CPPFLAGS := -Iinclude
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Wpedantic -Wshadow
BUILD_DIR := _build

CORE_SOURCES := \
	src/neural/network.cpp \
	src/brainstem/brainstem.cpp \
	src/bios/runtime.cpp
CORE_OBJECTS := $(CORE_SOURCES:%.cpp=$(BUILD_DIR)/%.o)

.PHONY: all test clean

all: $(BUILD_DIR)/eve_experiment $(BUILD_DIR)/eve_sequence_experiment

test: $(BUILD_DIR)/eve_tests
	$(BUILD_DIR)/eve_tests

$(BUILD_DIR)/eve_experiment: $(CORE_OBJECTS) $(BUILD_DIR)/src/main.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_sequence_experiment: $(CORE_OBJECTS) $(BUILD_DIR)/src/sequence_experiment.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_tests: $(CORE_OBJECTS) $(BUILD_DIR)/tests/test_main.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)/src $(BUILD_DIR)/tests $(BUILD_DIR)/eve_experiment \
		$(BUILD_DIR)/eve_sequence_experiment $(BUILD_DIR)/eve_tests

-include $(CORE_OBJECTS:.o=.d)
-include $(BUILD_DIR)/src/main.d
-include $(BUILD_DIR)/src/sequence_experiment.d
-include $(BUILD_DIR)/tests/test_main.d
