#pragma once
#include <vector>
#include <iostream>
#define WALL '#'
#define CELL ' '
namespace maze_gen{
extern int width, height;

//std::vector<std::vector<char>> maze;

void get_width_and_height();
void generate_maze();
void print_maze();
void parameters_definition_check();
}