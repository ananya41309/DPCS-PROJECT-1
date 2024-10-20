CC = g++
CFLAGS = -Wall -I src/include
LDFLAGS = -L /usr/local/lib -lSDL2main -lSDL2 -lm # SDL2 paths for macOS
OBJDIR = obj

# Source files
SRCS = src/poly_operations.c src/io_operations.c src/data_structures.c src/visualization.c src/main.c
# Object files
OBJS = $(SRCS:src/%.c=$(OBJDIR)/%.o)

# Target to create the binary
polyhedron_app: $(OBJS)
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -o polyhedron_app $(OBJS) $(LDFLAGS)

# Rule for building object files
$(OBJDIR)/%.o: src/%.c
	mkdir -p $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Phony target for cleaning up
.PHONY: clean
clean:
	rm -rf $(OBJDIR)