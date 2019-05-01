GLAD_DIR=glad
GLAD_C=$(GLAD_DIR)/src/glad.c
CXXFLAGS=-std=c++11 -lglfw -lGL -ldl -I$(GLAD_DIR)/include
# CXXFLAGS=-std=c++11 -lglfw -lGL -ldl -lGLEW

main.out: main.o car.h
	$(CXX) $(CXXFLAGS) -o $@ $(GLAD_C) main.o

# main.out: main.o
# 	$(CXX) $(CXXFLAGS) -o $@ $<