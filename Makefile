# Compiler
CC = gcc

# Compiler flags
CFLAGS = -W -Wextra -Wall -Werror -I./include

# Source files
SRCS = container.c

# Object files
OBJS = $(SRCS:.c=.o)

# Executable name
TARGET = wafdunix_container

# Default target
all: $(TARGET)

# Link object files into executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

# Clean up object files and executable
clean:
	rm -f $(OBJS) $(TARGET)

build:
	sudo docker build -t wafdunix_container ./

run:
	sudo docker run -it --privileged wafdunix_container
.PHONY: all clean