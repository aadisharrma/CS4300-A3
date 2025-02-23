# This sets the compiler we use: g++ for C++ code.
CXX      = g++

# This sets compiler flags:
#   - std=c++17: use C++17 standard
#   - Wall: turn on common warnings
#   - I...: specify include paths
CXXFLAGS = -std=c++17 -Wall         \
           -Iinclude                \
           -Iinclude/glad           \
           -Iinclude/GLFW           \
           -Iinclude/glm            \
           -Isrc

# This directs the linker to look in the local 'lib/' folder.
LDFLAGS  = -Llib

# This links against libraries found in lib/, plus macOS frameworks for OpenGL.
LIBS     = -lglfw3 -lglad           \
           -framework Cocoa         \
           -framework IOKit         \
           -framework CoreVideo     \
           -framework OpenGL

# This lists all of our .cpp files.
SRCS = src/main.cpp \
       src/MySceneGraph.cpp \
       src/PrintVisitor.cpp \
       src/Trackball.cpp

# This transforms each .cpp file into a .o file.
OBJS = $(SRCS:.cpp=.o)

# This is the path + name of the final executable.
# Note the "bin/" prefix.
TARGET = bin/Assignment3

# This is the default rule that builds the target.
all: $(TARGET)

# This links all .o files into the final executable in bin/.
$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

# This compiles .cpp -> .o with our flags.
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# This removes the compiled objects and the executable in bin/.
clean:
	rm -f $(OBJS) $(TARGET)
