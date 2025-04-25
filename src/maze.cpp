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
    current_maze = maze(width, height);
    rng.seed();
    current_maze.grid.resize(height);
    for (int i = 0; i < height; ++i){
        current_maze.grid[i].resize(width);
        for (int j = 0; j < width; ++j)
            if(i % 2 != 0 && j % 2 != 0 && (i < height - 1) && (j < width - 1))
                current_maze.grid[i][j] = CELL;
            else
                current_maze.grid[i][j] = WALL;
    }
    carve_maze(1,1);
    add_entrance_and_exit();
}

void maze_generator::print_maze()const{
    parameters_definition_check();
    std::stringstream ss;
    ss<<"Viewing " << current_maze.name << "maze" << std::endl;

    for(const auto &row : current_maze.grid){
        for (char cell : row)
            ss << cell;
        ss << std::endl;
    }

    std::cout << ss.str();
}

void maze_generator::parameters_definition_check() const{
    if(current_maze.grid.empty()){
        std::cout << "Please define width and height and/or load pre-made maze" << std::endl;
        return;
    }
}


/*class cell{
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

    cell_string get_neighbors(cell cell){
        cell_string res;
        if (cell.x < width - 1){

    }
    return res;
    }
    */
}
