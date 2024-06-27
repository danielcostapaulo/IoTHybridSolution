# Compiler
CXX = g++
# Compiler flags
CXXFLAGS = -std=c++11 -Wall

# Source files directory
SRC_DIR = src
# Build directory
BUILD_DIR = build
# Output executable name
TARGET = Kmeans_ot

# List of all source files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
# Corresponding object files
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Default target
all: $(TARGET)

# Rule to link the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Rule to compile source files into object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(BUILD_DIR)/fft_handler.h.gch | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Precompiled header for fft_handler.h
$(BUILD_DIR)/fft_handler.h.gch: $(SRC_DIR)/fft_handler.h $(BUILD_DIR)/aprox_fft.h.gch $(BUILD_DIR)/Arduino.h.gch | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

# Precompiled header for approx_fft.h
$(BUILD_DIR)/Arduino.h.gch: $(SRC_DIR)/Arduino.h $(BUILD_DIR)/pgmspace.h.gch  $(BUILD_DIR)/interrupt.h.gch | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/pgmspace.h.gch: $(SRC_DIR)/pgmspace.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/interrupt.h.gch: $(SRC_DIR)/interrupt.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/aprox_fft.h.gch: $(SRC_DIR)/aprox_fft.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

# Create the build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean target to remove all generated files
clean:
	rm -r $(BUILD_DIR)

.PHONY: all clean
