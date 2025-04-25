#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <random> 
#include <fstream>
#include <algorithm>
#include <sstream>
#define WALL '#'
#define CELL ' '

namespace maze_gen{
    struct maze{
        int width, height;
        std::vector<std::vector<char>> grid;
        std::string name;

        maze() : width(0), height(0), name("Unnamed") {};
        maze(int w, int h) : width(w), height(h), name("Unnamed") {};
    };
    class maze_generator{
        private:
            maze current_maze;
            int width, height;
            std::mt19937 rng; // pseudo random number generator

            void carve_maze(int x, int y);
            void add_entrance_and_exit();
        public:
            maze_generator();
            maze_generator(int w, int h);
            
            void get_width_and_height();
            void generate_maze();
            void print_maze()const;
            void parameters_definition_check() const;
            void save(const std::string& filename);
            bool load(const std::string& filename);
    };
} 