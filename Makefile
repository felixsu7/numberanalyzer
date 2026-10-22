EXE_NAME = numtheory
CXX_STD = c++17
MODE = debug

ifeq ($(MODE),debug)
  ifeq ($(OS),Windows_NT)
    OPTS = /Zi /Od
  else
    OPTS = -g3 -O0 -fsanitize=address,undefined -fno-sanitize-recover=undefined
  endif
else ifeq ($(MODE),release)
  ifeq ($(OS),Windows_NT)
    OPTS = /O2 /DNDEBUG
  else
    OPTS = -O2 -DNDEBUG
  endif
else
  $(error MODE must be either debug or release)
endif

SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
BIN_DIR = bin

ifeq ($(OS),Windows_NT)
CXX = cl.exe
WARN = /W4
TARGET = $(BIN_DIR)/$(EXE_NAME)_$(MODE).exe
SOURCES := $(wildcard $(SRC_DIR)/*.cpp)
HEADERS := $(wildcard $(INCLUDE_DIR)/*.hpp)

CXXFLAGS = /nologo /std:$(CXX_STD) /EHsc $(WARN) /I$(INCLUDE_DIR) $(OPTS)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS)
	@cmd.exe /C "if not exist $(BIN_DIR) mkdir $(BIN_DIR)"
	$(CXX) $(CXXFLAGS) $(SOURCES) /Fe$@
	@echo "Built target: $@"

clean:
	@cmd.exe /C "if exist $(BUILD_DIR) rmdir /S /Q $(BUILD_DIR)"
	@cmd.exe /C "if exist $(BIN_DIR) rmdir /S /Q $(BIN_DIR)"
	@echo "Cleaned"

else
CXX = clang++
WARN = -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion \
       -Wformat=2 -Wswitch-enum -Wnon-virtual-dtor -Woverloaded-virtual -Wold-style-cast
TARGET = $(BIN_DIR)/$(EXE_NAME)_$(MODE)
LDFLAGS = $(filter -fsanitize=%,$(OPTS))
CXXFLAGS = -std=$(CXX_STD) $(WARN) $(OPTS) -MMD -MP

SOURCES := $(shell find $(SRC_DIR) -name '*.cpp')
OBJECTS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SOURCES))
DEPS := $(OBJECTS:.o=.d)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $(TARGET)
	@echo "Built target: $@"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

clean:
	@rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "Cleaned"

-include $(DEPS)
endif
