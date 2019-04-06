CC=g++
FLAGS=-std=c++11 -lglfw -lGLEW -lGL 

.PHONY: vertices.hpp

modern: modern.cpp vertices.hpp
	$(CC) $(FLAGS) -o modern modern.cpp

vertices.hpp: point.txt point_calc.py
	./point_calc.py < point.txt > vertices.hpp