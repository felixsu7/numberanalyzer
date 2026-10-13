EXE_NAME = numtheory
CXX_STD = c++17
MODE = debug

CXX = clang++
WARN  = -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion \
        -Wformat=2 -Wswitch-enum -Wnon-virtual-dtor -Woverloaded-virtual -Wold-style-cast

ifeq ($(MODE),debug)
  OPTS = -g3 -O0 -fsanitize=address,undefined -fno-sanitize-recover=undefined
else ifeq ($(MODE),release)
  OPTS = -O2 -DNDEBUG
else
  $(error MODE must be either debug or release)
endif

LDFLAGS = $(filter -fsanitize=%,$(OPTS))

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
BIN_DIR = bin
TARGET = $(BIN_DIR)/$(EXE_NAME)_$(MODE)

CXXFLAGS = -std=$(CXX_STD) $(WARN) $(INC) $(OPTS) -MMD -MP
INCLUDES = -I$(INCLUDE_DIR)

SOURCES := $(shell find $(SRC_DIR) -name '*.cpp')
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))
DEPS := $(OBJECTS:.o=.d)

.PHONY: all clean get_deps

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $(TARGET)
	@echo "Built target: $@"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "Cleaned"

-include $(DEPS)

