GLAD_DIR=glad
GLAD_C=$(GLAD_DIR)/src/glad.c
CXXFLAGS=-std=c++11 -lglfw -lGLEW -lGL -ldl

# main.out: main.o
# 	$(CXX) $(CXXFLAGS) -o $@ $(GLAD_C) $<

main.out: main.o
	$(CXX) $(CXXFLAGS) -o $@ $<