GLAD_DIR=glad
GLAD_C=$(GLAD_DIR)/src/glad.c
CXXFLAGS=-std=c++11 -lglfw -lGL -ldl -I$(GLAD_DIR)/include
# CXXFLAGS=-std=c++11 -lglfw -lGL -ldl -lGLEW

main.out: main.cpp car.h
	$(CXX) $(CXXFLAGS) -o $@ $(GLAD_C) main.cpp

# main.out: main.o
# 	$(CXX) $(CXXFLAGS) -o $@ $<