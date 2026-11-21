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
	$(BUILD_DIR)/eve_research_stage_b $(BUILD_DIR)/eve_research_stage_b1 \
	$(BUILD_DIR)/eve_research_stage_b2 $(BUILD_DIR)/eve_research_stage_b3 \
	$(BUILD_DIR)/eve_research_stage_b31 $(BUILD_DIR)/eve_research_stage_b4 \
	$(BUILD_DIR)/eve_research_stage_b41 $(BUILD_DIR)/eve_research_stage_b42 \
	$(BUILD_DIR)/eve_research_stage_b43 $(BUILD_DIR)/eve_research_stage_b44 \
	$(BUILD_DIR)/eve_research_stage_b45 $(BUILD_DIR)/eve_research_stage_b46 \
	$(BUILD_DIR)/eve_research_stage_b47 $(BUILD_DIR)/eve_research_stage_b48 \
	$(BUILD_DIR)/eve_research_stage_b49 $(BUILD_DIR)/eve_research_stage_b50 \
	$(BUILD_DIR)/eve_research_stage_b51 $(BUILD_DIR)/eve_research_stage_b52 \
	$(BUILD_DIR)/eve_research_stage_b53 $(BUILD_DIR)/eve_research_stage_b54 \
	$(BUILD_DIR)/eve_research_stage_b55 $(BUILD_DIR)/eve_research_stage_b56 \
	$(BUILD_DIR)/eve_research_stage_b57 $(BUILD_DIR)/eve_research_stage_b58 \
	$(BUILD_DIR)/eve_research_stage_b59 $(BUILD_DIR)/eve_research_stage_b510 \
	$(BUILD_DIR)/eve_research_stage_b511 $(BUILD_DIR)/eve_research_stage_b512 \
	$(BUILD_DIR)/eve_research_stage_b513 $(BUILD_DIR)/eve_research_stage_c0

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

$(BUILD_DIR)/eve_research_stage_b1: $(CORE_OBJECTS) \
		$(BUILD_DIR)/src/research_stage_b1.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_research_stage_b2: $(CORE_OBJECTS) \
		$(BUILD_DIR)/src/research_stage_b2.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_research_stage_b3: $(CORE_OBJECTS) \
		$(BUILD_DIR)/src/research_stage_b3.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_research_stage_b31: $(CORE_OBJECTS) \
		$(BUILD_DIR)/src/research_stage_b31.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_research_stage_b4: $(CORE_OBJECTS) \
		$(BUILD_DIR)/src/research_stage_b4.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_research_stage_b41: $(CORE_OBJECTS) \
		$(BUILD_DIR)/src/research_stage_b41.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_research_stage_b42: $(CORE_OBJECTS) \
		$(BUILD_DIR)/src/research_stage_b42.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_research_stage_b43: $(CORE_OBJECTS) \
		$(BUILD_DIR)/src/research_stage_b43.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_research_stage_b44: $(CORE_OBJECTS) \
		$(BUILD_DIR)/src/research_stage_b44.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_research_stage_b45: $(CORE_OBJECTS) \
		$(BUILD_DIR)/src/research_stage_b45.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_research_stage_b46: $(CORE_OBJECTS) \
		$(BUILD_DIR)/src/research_stage_b46.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_research_stage_b47: $(CORE_OBJECTS) \
		$(BUILD_DIR)/src/research_stage_b47.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_research_stage_b48: $(CORE_OBJECTS) \
		$(BUILD_DIR)/src/research_stage_b48.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_research_stage_b49: $(CORE_OBJECTS) \
		$(BUILD_DIR)/src/research_stage_b49.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_research_stage_b50: $(CORE_OBJECTS) \
		$(BUILD_DIR)/src/research_stage_b50.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_research_stage_b51: $(CORE_OBJECTS) \
		$(BUILD_DIR)/src/research_stage_b51.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_research_stage_b52: $(CORE_OBJECTS) \
		$(BUILD_DIR)/src/research_stage_b52.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_research_stage_b53: $(CORE_OBJECTS) \
		$(BUILD_DIR)/src/research_stage_b53.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_research_stage_b54: $(CORE_OBJECTS) \
		$(BUILD_DIR)/src/research_stage_b54.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_research_stage_b55: $(CORE_OBJECTS) \
		$(BUILD_DIR)/src/research_stage_b55.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_research_stage_b56: $(CORE_OBJECTS) \
		$(BUILD_DIR)/src/research_stage_b56.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_research_stage_b57: $(CORE_OBJECTS) \
		$(BUILD_DIR)/src/research_stage_b57.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_research_stage_b58: $(CORE_OBJECTS) \
		$(BUILD_DIR)/src/research_stage_b58.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_research_stage_b59: $(CORE_OBJECTS) \
		$(BUILD_DIR)/src/research_stage_b59.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_research_stage_b510: $(CORE_OBJECTS) \
		$(BUILD_DIR)/src/research_stage_b510.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_research_stage_b511: $(CORE_OBJECTS) \
		$(BUILD_DIR)/src/research_stage_b511.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_research_stage_b512: $(CORE_OBJECTS) \
		$(BUILD_DIR)/src/research_stage_b512.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_research_stage_b513: $(CORE_OBJECTS) \
		$(BUILD_DIR)/src/research_stage_b513.o
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/eve_research_stage_c0: $(CORE_OBJECTS) \
		$(BUILD_DIR)/src/research_stage_c0.o
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
		$(BUILD_DIR)/eve_research_stage_b1 \
		$(BUILD_DIR)/eve_research_stage_b2 \
		$(BUILD_DIR)/eve_research_stage_b3 \
		$(BUILD_DIR)/eve_research_stage_b31 \
		$(BUILD_DIR)/eve_research_stage_b4 \
		$(BUILD_DIR)/eve_research_stage_b41 \
		$(BUILD_DIR)/eve_research_stage_b42 \
		$(BUILD_DIR)/eve_research_stage_b43 \
		$(BUILD_DIR)/eve_research_stage_b44 \
		$(BUILD_DIR)/eve_research_stage_b45 \
		$(BUILD_DIR)/eve_research_stage_b46 \
		$(BUILD_DIR)/eve_research_stage_b47 \
		$(BUILD_DIR)/eve_research_stage_b48 \
		$(BUILD_DIR)/eve_research_stage_b49 \
		$(BUILD_DIR)/eve_research_stage_b50 \
		$(BUILD_DIR)/eve_research_stage_b51 \
		$(BUILD_DIR)/eve_research_stage_b52 \
		$(BUILD_DIR)/eve_research_stage_b53 \
		$(BUILD_DIR)/eve_research_stage_b54 \
		$(BUILD_DIR)/eve_research_stage_b55 \
		$(BUILD_DIR)/eve_research_stage_b56 \
		$(BUILD_DIR)/eve_research_stage_b57 \
		$(BUILD_DIR)/eve_research_stage_b58 \
		$(BUILD_DIR)/eve_research_stage_b59 \
		$(BUILD_DIR)/eve_research_stage_b510 \
		$(BUILD_DIR)/eve_research_stage_b511 \
		$(BUILD_DIR)/eve_research_stage_b512 \
		$(BUILD_DIR)/eve_research_stage_b513 \
		$(BUILD_DIR)/eve_research_stage_c0 \
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
-include $(BUILD_DIR)/src/research_stage_b1.d
-include $(BUILD_DIR)/src/research_stage_b2.d
-include $(BUILD_DIR)/src/research_stage_b3.d
-include $(BUILD_DIR)/src/research_stage_b31.d
-include $(BUILD_DIR)/src/research_stage_b4.d
-include $(BUILD_DIR)/src/research_stage_b41.d
-include $(BUILD_DIR)/src/research_stage_b42.d
-include $(BUILD_DIR)/src/research_stage_b43.d
-include $(BUILD_DIR)/src/research_stage_b44.d
-include $(BUILD_DIR)/src/research_stage_b45.d
-include $(BUILD_DIR)/src/research_stage_b46.d
-include $(BUILD_DIR)/src/research_stage_b47.d
-include $(BUILD_DIR)/src/research_stage_b48.d
-include $(BUILD_DIR)/src/research_stage_b49.d
-include $(BUILD_DIR)/src/research_stage_b50.d
-include $(BUILD_DIR)/src/research_stage_b51.d
-include $(BUILD_DIR)/src/research_stage_b52.d
-include $(BUILD_DIR)/src/research_stage_b53.d
-include $(BUILD_DIR)/src/research_stage_b54.d
-include $(BUILD_DIR)/src/research_stage_b55.d
-include $(BUILD_DIR)/src/research_stage_b56.d
-include $(BUILD_DIR)/src/research_stage_b57.d
-include $(BUILD_DIR)/src/research_stage_b58.d
-include $(BUILD_DIR)/src/research_stage_b59.d
-include $(BUILD_DIR)/src/research_stage_b510.d
-include $(BUILD_DIR)/src/research_stage_b511.d
-include $(BUILD_DIR)/src/research_stage_b512.d
-include $(BUILD_DIR)/src/research_stage_b513.d
-include $(BUILD_DIR)/src/research_stage_c0.d
-include $(BUILD_DIR)/tests/test_main.d
