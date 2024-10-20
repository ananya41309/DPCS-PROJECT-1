CC = g++
CFLAGS = -Wall -I src/include
LDFLAGS = -L src/lib -lmingw32 -lSDL2main -lSDL2 -lm
OBJDIR = obj

# Source files
SRCS = src/poly_operations.c src/io_operations.c src/data_structures.c src/visualization.c src/main.c
# Object files
OBJS = $(SRCS:src/%.c=$(OBJDIR)/%.o)

# Target to create the binary
polyhedron_app: $(OBJS)
	@if not exist $(OBJDIR) mkdir $(OBJDIR)
	$(CC) $(CFLAGS) -o polyhedron_app $(OBJS) $(LDFLAGS)

# Rule for building object files
$(OBJDIR)/%.o: src/%.c
	@if not exist $(OBJDIR) mkdir $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Phony target for cleaning up
.PHONY: clean
clean:
	@if exist $(OBJDIR) rmdir /s /q $(OBJDIR)
