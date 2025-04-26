#include "maze.h"

namespace maze_gen{

maze_generator::maze_generator():
    width(0), height(0), rng(std::random_device{}()){}

maze_generator::maze_generator(int w, int h):
    width(w), height(h), rng(std::random_device{}()){}

void maze_generator::get_width_and_height() {
    int num;

    do {
        std::cout << "Enter an odd number for width: ";
        std::cin >> num;
        if (num % 2 == 0 || num == 1) {
            std::cout << "Width must be an odd number and greater than 1!" << std::endl;
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
        if (num % 2 == 0 || num == 1) {
            std::cout << "Height must be an odd number and greater than 1!" << std::endl;
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

void maze_generator::generate_maze(){
    parameters_definition_check();
    rng.seed();
    current_maze = maze(width, height);
    current_maze.grid.resize(height);
    current_maze.visited.resize(height);
    for (int i = 0; i < height; ++i){
        current_maze.grid[i].resize(width);
        current_maze.visited[i].resize(width);
        for (int j = 0; j < width; ++j){
            if(i % 2 != 0 && j % 2 != 0 && (i < height - 1) && (j < width - 1))
                current_maze.grid[i][j] = CELL;
            else
                current_maze.grid[i][j] = WALL;
            current_maze.visited[i][j] = false;
        }
    }
    //carve_maze(1,1);
    //add_entrance_and_exit();
}

void maze_generator::print_maze()const{
    parameters_definition_check();
    std::stringstream ss;
    ss<<"Viewing " << current_maze.name << " maze" << std::endl;

    for(const auto &row : current_maze.grid){
        for (char cell : row)
            ss << cell << CELL;
        ss << std::endl;
    }

    std::cout << ss.str();
}

void maze_generator::parameters_definition_check()const{
    if(width < 1 || height < 1){
        std::cout << "Please define width and height and/or load pre-made maze" << std::endl;
        return;
    }
}

std::vector<cell> maze_generator::get_neighbors(cell c)const{
    std::vector<cell> res;
    std::vector<cell> cells;
    cell up = cell(c.x, c.y - 2);
    cell rt = cell(c.x + 2, c.y);
    cell dw = cell(c.x, c.y + 2);
    cell lt = cell(c.x - 2, c.y);
    cells.insert(res.end(), {up, rt, dw, lt});
    for (cell neighb : cells){
        if(neighb.x < width && neighb.y < height && neighb.x > 0 && neighb.y > 0 && 
            current_maze.grid[neighb.x][neighb.y] != WALL && current_maze.visited[neighb.x][neighb.y] != VISITED){
            res.push_back(neighb);
        }
    }
    return res;
}
    void maze_generator::remove_wall(cell first, cell second){
        short int xdiff = second.x - first.x;
        short int ydiff = second.y - first.y;

        short int addx = (xdiff != 0) ? (xdiff/abs(xdiff)) : 0;
        short int addy = (ydiff != 0) ? (ydiff/abs(ydiff)) : 0;

        current_maze.grid[first.x + addx][first.y + addy] = CELL;
        current_maze.visited[first.x + addx][first.y + addy] = VISITED;
}
}
