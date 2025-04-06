#include "maze.h"

namespace maze_gen{
int width = -1;
int height = -1;

std::vector<std::vector<char>> maze;

void get_width_and_height() {
    int num;

    do {
        std::cout << "Enter an odd number for width: ";
        std::cin >> num;
        if (num % 2 == 0) {
            std::cout << "Width must be an odd number!" << std::endl;
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        if (!std::cin) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
        break;
    } while (true);
    width = num;

    do {
        std::cout << "Enter an odd number for height: ";
        std::cin >> num;
        if (num % 2 == 0) {
            std::cout << "Height must be an odd number!" << std::endl;
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            continue;
        }

        if (!std::cin) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
        break;
    } while (true);
    height = num;
}

void generate_maze(){
    parameters_definition_check();
    maze.resize(height);
    for (int i = 0; i < height; ++i){
        maze[i].resize(width);
        for (int j = 0; j < width; ++j)
            if(i % 2 != 0 && j % 2 != 0 && (i < height - 1) && (j < width - 1))
                maze[i][j] = CELL;
            else
                maze[i][j] = WALL;
    }
    //DFS algorithm
}

void print_maze(){
    parameters_definition_check();
    for (int i = 0; i < height; ++i){
        for (int j = 0; j < width; ++j)
            std::cout << maze[i][j] << CELL;
        std::cout << std::endl;
    }
}

void parameters_definition_check(){
    if(width == -1 || height == -1){
        std::cout << "Please define width and height" << std::endl;
        return;
    }
}
}

class cell{
    public:
        int x;
        int y;
        cell(int x, int y): x(x), y(y) {}
    };

class cell_string{
private:
    std::vector<cell> cells;

public:
    void add_cell(cell a){
        cells.push_back(a);
    }
    int get_length(){
        return cells.size();
    }
};

    /*cell_string get_neighbors(cell cell, maze maze){
        cell_string res;
        if (cell.x < SIZE - 1){

    }
    return res;
    }*/
