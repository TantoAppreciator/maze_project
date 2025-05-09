#include "maze.h"

namespace maze_gen{

void maze_generator::carve_maze(int x, int y){
    std::stack<cell> st;
    cell current_cell(x,y);
    current_maze.visited[x][y] = VISITED;
    cell neighbor_cell;
    do{
        std::vector<cell> neighbors = get_neighbors(current_cell);
        if(neighbors.size() > 0){
            std::shuffle(neighbors.begin(), neighbors.end(),rng);
            neighbor_cell = neighbors[0];
            st.push(current_cell);
            remove_wall(current_cell, neighbor_cell);
            current_cell = neighbor_cell;
            current_maze.visited[current_cell.x][current_cell.y] = VISITED;
        }
        else if (st.size() > 0){
            st.pop();
            current_cell = st.top();
        }
        else if (if_unvisited()){
            std::vector<cell> unvisited = get_unvisited_cells();
            std::shuffle(unvisited.begin(), unvisited.end(), rng);
            current_cell = unvisited[0];
        }
        else {
            break;
        }
    } while(if_unvisited());
    return;
}

void maze_generator::add_entrance_and_exit(){
    current_maze.grid[0][1] = CELL;
    current_maze.grid[height - 1][width - 2] = CELL;
    return;
}
bool maze_generator::if_unvisited()const{
    for(int i = 1; i < height; i+=2)
        for(int j = 1; j < width; j+=2)
            if(!current_maze.visited[i][j])
                return true;
    return false;
}
std::vector<cell> maze_generator::get_unvisited_cells()const{
    std::vector<cell> res;
    for(int i = 1; i < height; i+=2)
        for(int j = 1; j < width; j+=2)
            if(!current_maze.visited[i][j]){
                cell c(i,j);
                res.push_back(c);
            } 
    return res;
}


maze_generator::maze_generator():
    width(0), height(0), rng(std::random_device{}()){}

maze_generator::maze_generator(int w, int h):
    width(w), height(h), rng(std::random_device{}()){}

void maze_generator::get_width_and_height() {
    int num;

    do {
        std::cout << "Enter an odd number for width: ";
        std::cin >> num;
        if (num % 2 == 0 || num <= 3) {
            std::cout << "Width must be an odd number greater than 3!" << std::endl;
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
        if (num % 2 == 0 || num <= 3) {
            std::cout << "Height must be an odd number greater than 3!" << std::endl;
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
    return;
}

void maze_generator::generate_maze(){
    if(width < 1 || height < 1){
        std::cout << "Please define width and height and/or load pre-made maze" << std::endl;
        return;
    }

    std::cout<<"Generating maze..." << std::endl;

    rng.seed(std::random_device{}());

    current_maze = maze(width, height);

    current_maze.grid.resize(height);
    current_maze.visited.resize(height);

    for (int i = 0; i < height; ++i){
        current_maze.grid[i].resize(width);
        current_maze.visited[i].resize(width);

        for (int j = 0; j < width; ++j){
            if(i % 2 != 0 && j % 2 != 0 && (i < height - 1) && (j < width - 1)){
                current_maze.grid[i][j] = CELL;
            }
            else{
                current_maze.grid[i][j] = WALL;
            }
            current_maze.visited[i][j] = false;
        }
    }
    carve_maze(1,1);
    add_entrance_and_exit();
    return;
}

void maze_generator::print_maze()const{
    if(width < 1 || height < 1){
        std::cout << "Please define width and height and/or load pre-made maze" << std::endl;
        return;
    }
    std::stringstream ss;
    ss<<"Viewing " << current_maze.name << " maze" << std::endl;

    for(const auto &row : current_maze.grid){
        for (char cell : row)
            ss << cell << CELL;
        ss << std::endl;
    }

    std::cout << ss.str();
    return;
}

std::vector<cell> maze_generator::get_neighbors(cell &c)const{
    std::vector<cell> res;
    std::vector<cell> cells;

    cell up = cell(c.x, c.y - 2);
    cell rt = cell(c.x + 2, c.y);
    cell dw = cell(c.x, c.y + 2);
    cell lt = cell(c.x - 2, c.y);

    cells.insert(res.end(), {up, rt, dw, lt});

    for (cell neighb : cells){
        if(neighb.x < height && neighb.y < width && neighb.x > 0 && neighb.y > 0 && 
            current_maze.grid[neighb.x][neighb.y] != WALL && current_maze.visited[neighb.x][neighb.y] != VISITED){
            res.push_back(neighb);
        }
    }
    return res;
}
    void maze_generator::remove_wall(cell &first, cell &second){
        short int xdiff = second.x - first.x;
        short int ydiff = second.y - first.y;

        short int addx = (xdiff != 0) ? (xdiff/abs(xdiff)) : 0;
        short int addy = (ydiff != 0) ? (ydiff/abs(ydiff)) : 0;

        current_maze.grid[first.x + addx][first.y + addy] = CELL;
        current_maze.visited[first.x + addx][first.y + addy] = VISITED;
        return;
}
    void maze_generator::save(const std::string& filename){
        current_maze.name = filename;

        std::ofstream file(filename, std::ios::binary);

        if (!file || !file.is_open()) {
            std::cerr << "Error opening file" << std::endl;
            return;
        }

        size_t name_length = current_maze.name.size();

        file.write(reinterpret_cast<const char*>(&name_length), sizeof(name_length));
        file.write((current_maze.name.data()), name_length);

        file.write(reinterpret_cast<const char*>(&current_maze.height), sizeof(current_maze.height));
        file.write(reinterpret_cast<const char*>(&current_maze.width), sizeof(current_maze.width));

        for (const auto &row : current_maze.grid){
            file.write(row.data(), row.size());
        }

        file.close();

        std::cout<<"Maze successfully saved to " << filename << std::endl;
        return;
    }

    bool maze_generator::load(const std::string& filename){
        std::ifstream file(filename, std::ios::binary);

        if (!file || !file.is_open()) {
            std::cerr << "Error loading file. File may not exist" << std::endl;
            return false;
        }

        size_t name_length;

        file.read(reinterpret_cast<char*>(&name_length), sizeof(name_length));
        current_maze.name.resize(name_length);
        file.read(&current_maze.name[0], name_length);

        file.read(reinterpret_cast<char*>(&current_maze.height), sizeof(current_maze.height));
        file.read(reinterpret_cast<char*>(&current_maze.width), sizeof(current_maze.width));

        current_maze.grid.assign(current_maze.height, std::vector<char>(current_maze.width));
        for (auto &row : current_maze.grid){
            file.read(row.data(), row.size());
        }

        width = current_maze.width;
        height = current_maze.height;
        file.close();

        std::cout<<"Maze " << current_maze.name << " successfully loaded" << std::endl;
        return true;
    }
    std::vector<cell> maze_generator::get_neighbors_solver(cell& c)const{
        std::vector<cell> res;
        std::vector<cell> cells;
    
        cell up = cell(c.x, c.y - 2);
        cell rt = cell(c.x + 2, c.y);
        cell dw = cell(c.x, c.y + 2);
        cell lt = cell(c.x - 2, c.y);
    
        cells.insert(res.end(), {up, rt, dw, lt});
    
        for (cell neighb : cells){
            if(neighb.x < height && neighb.y < width && neighb.x > 0 && neighb.y > 0 && 
                current_maze.grid[neighb.x][neighb.y] != WALL && current_maze.visited[neighb.x][neighb.y] != VISITED && 
                current_maze.grid[(neighb.x + c.x)/2][(neighb.y + c.y)/2] != WALL){     //unlike get_neighbors, checks if there's a wall between them
                res.push_back(neighb);                                                  //needed for solver
            }
        }
        return res;
    }

    void maze_generator::solve(){
        if(current_maze.grid.size() <= 0){
            std::cout << "Please generate or load maze first" << std::endl;
            return;
        }
        std::stack<cell> solution;
        cell current_cell(1, 1);
        cell neighbor_cell;
        std::vector<std::vector<char>> solved_grid = current_maze.grid;
        for (int i = 0; i < height; ++i){
            for (int j = 0; j < width; ++j)
                current_maze.visited[i][j] = false;
        }
        current_maze.visited[1][1] = VISITED;
       do{
            std::vector<cell> neighbors = get_neighbors_solver(current_cell);
            if(neighbors.size() > 0){
                solution.push(current_cell);
                solved_grid[current_cell.x][current_cell.y] = PATH;
                current_maze.visited[current_cell.x][current_cell.y] = VISITED;
                std::shuffle(neighbors.begin(), neighbors.end(),rng);
                neighbor_cell = neighbors[0];
                solution.push(neighbor_cell);
                solved_grid[(current_cell.x + neighbor_cell.x)/2][(current_cell.y + neighbor_cell.y)/2] = PATH;
                solved_grid[neighbor_cell.x][neighbor_cell.y] = PATH;
                current_maze.visited[neighbor_cell.x][neighbor_cell.y] = VISITED;
                current_cell = neighbor_cell;
            }
            else{
                solution.pop();
                solved_grid[current_cell.x][current_cell.y] = CELL;
                solved_grid[(current_cell.x + solution.top().x)/2][(current_cell.y + solution.top().y)/2] = CELL;
                current_cell = solution.top();
                solved_grid[current_cell.x][current_cell.y] = CELL;
            }
        } while(current_cell.x != height - 2 || current_cell.y != width - 2);
        std::stringstream ss;
        ss<<"Viewing solved " << current_maze.name << " maze" << std::endl;
    
        for(const auto &row : solved_grid){
            for (char cell : row)
                if(cell == '*'){
                    ss << RED << cell << RESET << CELL;
                }
                else{
                    ss << cell << CELL;
                }
            ss << std::endl;
        }
    
        std::cout << ss.str();
        return;
    }
    void maze_generator::play()const{
        if(current_maze.grid.size() <= 0){
            std::cout << "Please generate or load maze first" << std::endl;
            return;
        }
        // saving current shell settings
        termios original_attributes;
        tcgetattr(STDIN_FILENO, &original_attributes);

        // changing shell settings: turning off canonical mode and echo
        termios modified_attributes = original_attributes;
        modified_attributes.c_lflag &= ~(ICANON | ECHO);
        modified_attributes.c_cc[VMIN] = 0;  // do not wait input
        modified_attributes.c_cc[VTIME] = 0; // return handling right away
        tcsetattr(STDIN_FILENO, TCSANOW, &modified_attributes);

        std::vector<std::vector<char>> play_grid = current_maze.grid;
        cell current_cell(1,1);
        while(current_cell.x != height - 2 || current_cell.y != width - 2){
            char c;
            play_grid[current_cell.x][current_cell.y] = ' ';

            if (read(STDIN_FILENO, &c, 1) == 1) {
                if (c == 'q') break;              
                else if (c == 'w' && play_grid[current_cell.x-1][current_cell.y] != WALL && (current_cell.x != 1 || current_cell.y != 1)) current_cell.x-=2;  
                else if (c == 's' && play_grid[current_cell.x+1][current_cell.y] != WALL) current_cell.x+=2;  
                else if (c == 'a' && play_grid[current_cell.x][current_cell.y-1] != WALL) current_cell.y-=2;  
                else if (c == 'd' && play_grid[current_cell.x][current_cell.y+1] != WALL) current_cell.y+=2;  
            }

            play_grid[current_cell.x][current_cell.y] = '*';
            driver_logic::clear_screen();
    
            std::stringstream ss;
            for(const auto &row : play_grid){
                for (char cell : row)
                    if(cell == '*'){
                        ss << RED << cell << RESET << CELL;
                    }
                    else{
                        ss << cell << CELL;
                    }
                ss << std::endl;
            }

            std::cout << ss.str();

            std::cout << "Use WASD to move, Q to quit" << std::endl;
            std::cout << std::flush; 
    
            usleep(100000);
        }
        // setting shell settings back
        tcsetattr(STDIN_FILENO, TCSANOW, &original_attributes);
        return;
        }
}
