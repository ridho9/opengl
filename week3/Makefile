CC=g++
FLAGS=-std=c++11 -lglfw -lGL -ldl -I../include

.PHONY: car.h

main:  main.cpp car.h
	$(CC) $(FLAGS) -o main main.cpp ../glad.c

car.h: car generate.py
	python generate.py < car > car.h