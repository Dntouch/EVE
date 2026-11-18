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

all: $(BUILD_DIR)/eve_experiment $(BUILD_DIR)/eve_sequence_experiment \
	$(BUILD_DIR)/eve_performance_benchmark $(BUILD_DIR)/eve_research_002 \
	$(BUILD_DIR)/eve_experiment_002 $(BUILD_DIR)/eve_experiment_003 \
	$(BUILD_DIR)/eve_experiment_003_benchmark $(BUILD_DIR)/eve_research_stage_a \
	$(BUILD_DIR)/eve_research_stage_a1 $(BUILD_DIR)/eve_research_stage_a2 \
	$(BUILD_DIR)/eve_research_stage_b

test: $(BUILD_DIR)/eve_tests
	$(BUILD_DIR)/eve_tests

$(BUILD_DIR)/eve_experiment: $(CORE_OBJECTS) $(BUILD_DIR)/src/main.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_sequence_experiment: $(CORE_OBJECTS) $(BUILD_DIR)/src/sequence_experiment.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_performance_benchmark: $(CORE_OBJECTS) $(BUILD_DIR)/src/performance_benchmark.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_research_002: $(CORE_OBJECTS) $(BUILD_DIR)/src/research_002.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_experiment_002: $(CORE_OBJECTS) $(BUILD_DIR)/src/experiment_002.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_experiment_003: $(CORE_OBJECTS) $(BUILD_DIR)/src/experiment_003.o
	$(CXX) $(CXXFLAGS) -pthread $^ -o $@

$(BUILD_DIR)/eve_experiment_003_benchmark: $(CORE_OBJECTS) \
		$(BUILD_DIR)/src/experiment_003_benchmark.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_research_stage_a: $(CORE_OBJECTS) \
		$(BUILD_DIR)/src/research_stage_a.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_research_stage_a1: $(CORE_OBJECTS) \
		$(BUILD_DIR)/src/research_stage_a1.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_research_stage_a2: $(CORE_OBJECTS) \
		$(BUILD_DIR)/src/research_stage_a2.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_research_stage_b: $(CORE_OBJECTS) \
		$(BUILD_DIR)/src/research_stage_b.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_tests: $(CORE_OBJECTS) $(BUILD_DIR)/tests/test_main.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)/src $(BUILD_DIR)/tests $(BUILD_DIR)/eve_experiment \
		$(BUILD_DIR)/eve_sequence_experiment $(BUILD_DIR)/eve_performance_benchmark \
		$(BUILD_DIR)/eve_research_002 $(BUILD_DIR)/eve_experiment_002 \
		$(BUILD_DIR)/eve_experiment_003 $(BUILD_DIR)/eve_experiment_003_benchmark \
		$(BUILD_DIR)/eve_research_stage_a $(BUILD_DIR)/eve_research_stage_a1 \
		$(BUILD_DIR)/eve_research_stage_a2 \
		$(BUILD_DIR)/eve_research_stage_b \
		$(BUILD_DIR)/eve_tests

-include $(CORE_OBJECTS:.o=.d)
-include $(BUILD_DIR)/src/main.d
-include $(BUILD_DIR)/src/sequence_experiment.d
-include $(BUILD_DIR)/src/performance_benchmark.d
-include $(BUILD_DIR)/src/research_002.d
-include $(BUILD_DIR)/src/experiment_002.d
-include $(BUILD_DIR)/src/experiment_003.d
-include $(BUILD_DIR)/src/experiment_003_benchmark.d
-include $(BUILD_DIR)/src/research_stage_a.d
-include $(BUILD_DIR)/src/research_stage_a1.d
-include $(BUILD_DIR)/src/research_stage_a2.d
-include $(BUILD_DIR)/src/research_stage_b.d
-include $(BUILD_DIR)/tests/test_main.d
