#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <random> 
#include <fstream>
#include <algorithm>
#include <sstream>
#include <stack>
#define RED "\033[31m"
#define RESET "\033[0m"
#define WALL '#'
#define CELL ' '
#define PATH '*'
#define VISITED true

namespace maze_gen{
    struct maze{
        int width, height;
        std::vector<std::vector<char>> grid;
        std::vector<std::vector<bool>> visited;
        std::string name;

        maze() : width(0), height(0), name("Unnamed") {};
        maze(int w, int h) : width(w), height(h), name("Unnamed") {};
    };

    struct cell{
        unsigned int x, y;
    
        cell() : x(0), y(0) {};
        cell(unsigned int x, unsigned int y): x(x), y(y) {}
    };

    class maze_generator{
        private:
            maze current_maze;
            int width, height;
            std::mt19937 rng; // pseudo random number generator

            void carve_maze(int x, int y);
            void add_entrance_and_exit();
            std::vector<cell> get_unvisited_cells()const;
        public:
            maze_generator();
            maze_generator(int w, int h);
            
            void get_width_and_height();
            void generate_maze();
            void print_maze()const;
            void save(const std::string& filename);
            bool load(const std::string& filename);
            void solve();
            std::vector<cell> get_neighbors(cell& c)const;
            std::vector<cell> get_neighbors_solver(cell& c)const;
            void remove_wall(cell &first, cell &second);
    };

} 