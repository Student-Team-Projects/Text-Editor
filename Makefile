CXX := g++
LD := g++
CXXFLAGS := -Wall -Wextra -Werror -Wpedantic -pedantic-errors -std=c++20
LDFLAGS := -lncurses

debug: CXXFLAGS += -g3 -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -fsanitize=address -fsanitize=undefined
debug: LDFLAGS += -g3 -fsanitize=address -fsanitize=undefined
release: CXXFLAGS += -O3
release: LDFLAGS += -O3

srcdir := src
builddir := build
outdir := bin

SOURCES := $(wildcard $(srcdir)/*.cpp)
OBJS := $(SOURCES:$(srcdir)/%.cpp=$(builddir)/%.o)
DEPS := $(SOURCES:$(srcdir)/%.cpp=$(builddir)/%.d)
TARGET := $(outdir)/program

all: release

release: $(TARGET)
debug: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p bin
	$(LD) $+ $(LDFLAGS) -o $@

$(builddir)/%.o: $(srcdir)/%.cpp $(builddir)/%.d
	@mkdir -p build
	$(CXX) $< $(CXXFLAGS) -c -o $@

$(builddir)/%.d: $(srcdir)/%.cpp
	@mkdir -p build
	@$(CXX) $< -MM | \
	sed 's=\($*\)\.o[ :]*=$(builddir)/\1.o $@:=g' > $@

clean:
	@rm -rf $(OBJS) $(DEPS) $(TARGET)
	-@rmdir bin build 2>/dev/null || true

run: debug
	@./$(TARGET)

.PHONY: clean all release debug run
.SECONDARY: $(OBJS) $(DEPS)
-include $(DEPS)