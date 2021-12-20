NAME       = neo_pixel_uart

UART_BAUD  = 2400000
PORT       = /dev/ttyUSB0

TOP_DIR    = ./
BIN_DIR    = $(TOP_DIR)/bin
OBJ_DIR    = $(BIN_DIR)/obj

CXX        = g++
OPT_FLAGS  =
CXXFLAGS   = -c $(OPT_FLAGS)
LDFLAGS    = -pthread -lfftw3 -lm -lpulse-simple

DEBUG ?= 0
ifeq ($(DEBUG), 1)
	CXXFLAGS += -DDEBUG
endif

DEPS       = NeoPixelUART.h ThreadHelpers.h PulseAudioRead.h DFT.h Visuals.h
SRCS       = main.cpp NeoPixelUART.cpp ThreadHelpers.cpp PulseAudioRead.cpp DFT.cpp Visuals.cpp
OBJECTS    = $(SRCS:.cpp=.o)
OBJECTS    := $(addprefix $(OBJ_DIR)/,$(OBJECTS))


all: $(NAME)

run:
	./$(NAME)

$(NAME): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@

$(OBJ_DIR)/%.o: %.cpp $(DEPS) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -fr $(BIN_DIR)
