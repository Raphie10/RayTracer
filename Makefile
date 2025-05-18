##
## EPITECH PROJECT, 2024
## RayTracer
## File description: Useless Makefile (for mouli)
## Makefile
##

tests_run:
	mkdir -p build && cd build && cmake .. && cmake --build . --target unittest
	./unittest