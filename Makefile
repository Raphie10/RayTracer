##
## EPITECH PROJECT, 2024
## RayTracer
## File description: Useless Makefile (for mouli)
## Makefile
##

NAME	=	raytracer

all:
	g++ Plugins/compil.cpp -o $(NAME)

clean:
	rm -f $(NAME)

fclean: clean
	rm -f $(NAME)

tests_run:
	mkdir -p build && cd build && cmake .. && cmake --build . --target unittest
	./unittest

re: fclean all