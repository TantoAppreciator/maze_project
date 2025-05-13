#include "maze.h"
#include <cassert>

namespace maze_gen {

// Carves paths in the maze starting from the given cell (x, y) using a depth-first search algorithm with backtracking.
void maze_generator::carve_maze(int x, int y) {
    // Ensure starting cell is valid: within bounds and at an odd index (cell position)
    assert(x > 0 && x < height && y > 0 && y < width && x % 2 == 1 && y % 2 == 1);

    std::stack<cell> st;              // Stack for backtracking
    cell current_cell(x, y);          // Current position in the maze
    current_maze.visited[x][y] = VISITED; // Mark starting cell as visited
    cell neighbor_cell;

    do {
        std::vector<cell> neighbors = get_neighbors(current_cell); // Get unvisited neighbors
        if (neighbors.size() > 0) {
            // Choose a random neighbor and carve a path to it
            std::shuffle(neighbors.begin(), neighbors.end(), rng);
            neighbor_cell = neighbors[0];
            st.push(current_cell);            // Save current cell for backtracking
            remove_wall(current_cell, neighbor_cell); // Remove wall between cells
            current_cell = neighbor_cell;     // Move to the neighbor
            current_maze.visited[current_cell.x][current_cell.y] = VISITED; // Mark as visited
        } else if (st.size() > 0) {
            // Backtrack to the previous cell if no unvisited neighbors
            st.pop();
            current_cell = st.top();
        } else if (if_unvisited()) {
            // Jump to a random unvisited cell if stack is empty
            std::vector<cell> unvisited = get_unvisited_cells();
            std::shuffle(unvisited.begin(), unvisited.end(), rng);
            current_cell = unvisited[0];
        } else {
            break; // Exit when all cells are visited
        }
    } while (if_unvisited());
}


 //Adds an entrance at the top-left (0,1) and an exit at the bottom-right (height-1, width-2) of the maze

void maze_generator::add_entrance_and_exit() {
    current_maze.grid[0][1] = CELL;           // Set entrance
    current_maze.grid[height - 1][width - 2] = CELL; // Set exit
}

/*
  Checks if there are any unvisited cells left in the maze.
  return True if unvisited cells exist, false otherwise.
 */
bool maze_generator::if_unvisited() const {
    for (int i = 1; i < height; i += 2)       // Check only cell positions (odd indices)
        for (int j = 1; j < width; j += 2)
            if (!current_maze.visited[i][j])
                return true;
    return false;
}

/*
Returns a vector of all unvisited cells in the maze.
return Vector of unvisited cell coordinates.
 */
std::vector<cell> maze_generator::get_unvisited_cells() const {
    std::vector<cell> res;
    for (int i = 1; i < height; i += 2)       // Iterate over cell positions
        for (int j = 1; j < width; j += 2)
            if (!current_maze.visited[i][j]) {
                cell c(i, j);
                res.push_back(c);
            }
    return res;
}

//Default constructor: initializes maze dimensions to 0 and seeds the random number generator.
maze_generator::maze_generator() : width(0), height(0), rng(std::random_device{}()) {}

/*
 Parameterized constructor: sets maze dimensions and seeds the random number generator.
 param w Width of the maze.
 param h Height of the maze.
 */
maze_generator::maze_generator(int w, int h) : width(w), height(h), rng(std::random_device{}()) {}

/**
 Prompts the user to input odd numbers greater than 3 for maze width and height.
 Includes input validation to ensure valid dimensions.
 */
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
}

/*
 Generates a maze with the specified width and height.
 Initializes the grid and visited array, carves paths, and adds entrance/exit.
 */
void maze_generator::generate_maze() {
    // Ensure dimensions are valid for maze generation
    assert(width > 3 && height > 3 && width % 2 == 1 && height % 2 == 1);

    if (width < 1 || height < 1) {
        std::cout << "Please define width and height and/or load pre-made maze" << std::endl;
        return;
    }

    std::cout << "Generating maze..." << std::endl;

    rng.seed(std::random_device{}()); // Reseed RNG for randomness

    current_maze = maze(width, height); // Initialize maze object

    // Resize grid and visited arrays
    current_maze.grid.resize(height);
    current_maze.visited.resize(height);

    for (int i = 0; i < height; ++i) {
        current_maze.grid[i].resize(width);
        current_maze.visited[i].resize(width);
        for (int j = 0; j < width; ++j) {
            // Set cells at odd indices, walls elsewhere
            if (i % 2 != 0 && j % 2 != 0 && (i < height - 1) && (j < width - 1)) {
                current_maze.grid[i][j] = CELL;
            } else {
                current_maze.grid[i][j] = WALL;
            }
            current_maze.visited[i][j] = false;
        }
    }
    carve_maze(1, 1);         // Start carving from (1,1)
    add_entrance_and_exit();  // Add entrance and exit
}

//Prints the current maze to the console with its name.
void maze_generator::print_maze() const {
    if (width < 1 || height < 1) {
        std::cout << "Please define width and height and/or load pre-made maze" << std::endl;
        return;
    }
    std::stringstream ss;
    ss << "Viewing " << current_maze.name << " maze" << std::endl;

    for (const auto &row : current_maze.grid) {
        for (char cell : row)
            ss << cell << CELL;
        ss << std::endl;
    }

    std::cout << ss.str();
}

 //Returns a vector of unvisited neighboring cells that are not walls.
 //param c The current cell.
 //return Vector of neighboring cells.
std::vector<cell> maze_generator::get_neighbors(cell &c) const {
    std::vector<cell> res;
    std::vector<cell> cells;

    // Define potential neighbors (2 steps away due to wall spacing)
    cell up(c.x, c.y - 2);
    cell rt(c.x + 2, c.y);
    cell dw(c.x, c.y + 2);
    cell lt(c.x - 2, c.y);

    cells.insert(cells.end(), {up, rt, dw, lt});

    for (cell neighb : cells) {
        if (neighb.x < height && neighb.y < width && neighb.x > 0 && neighb.y > 0 &&
            current_maze.grid[neighb.x][neighb.y] != WALL && !current_maze.visited[neighb.x][neighb.y]) {
            res.push_back(neighb);
        }
    }
    return res;
}

/*
Removes the wall between two adjacent cells.
param first The first cell.
param second The second cell (must be adjacent to the first).
 */
void maze_generator::remove_wall(cell &first, cell &second) {
    // Ensure cells are adjacent (exactly 2 units apart in one direction)
    assert(abs(first.x - second.x) + abs(first.y - second.y) == 2);

    short int xdiff = second.x - first.x;
    short int ydiff = second.y - first.y;

    short int addx = (xdiff != 0) ? (xdiff / abs(xdiff)) : 0; // Direction in x
    short int addy = (ydiff != 0) ? (ydiff / abs(ydiff)) : 0; // Direction in y

    current_maze.grid[first.x + addx][first.y + addy] = CELL; // Remove wall
    current_maze.visited[first.x + addx][first.y + addy] = VISITED; // Mark as visited
}

/*
Saves the current maze to a binary file.
param filename The name of the file to save to.
 */
void maze_generator::save(const std::string &filename) {
    current_maze.name = filename;

    std::ofstream file(filename, std::ios::binary);

    if (!file || !file.is_open()) {
        std::cerr << "Error opening file" << std::endl;
        return;
    }

    size_t name_length = current_maze.name.size();
    // Serialize maze data: name length, name, dimensions, grid
    file.write(reinterpret_cast<const char *>(&name_length), sizeof(name_length));
    file.write(current_maze.name.data(), name_length);
    file.write(reinterpret_cast<const char *>(&current_maze.height), sizeof(current_maze.height));
    file.write(reinterpret_cast<const char *>(&current_maze.width), sizeof(current_maze.width));

    for (const auto &row : current_maze.grid) {
        file.write(row.data(), row.size());
    }

    file.close();
    std::cout << "Maze successfully saved to " << filename << std::endl;
}

/*
Loads a maze from a binary file.
param filename The name of the file to load from.
return True if loading succeeds, false otherwise.
 */
bool maze_generator::load(const std::string &filename) {
    std::ifstream file(filename, std::ios::binary);

    if (!file || !file.is_open()) {
        std::cerr << "Error loading file. File may not exist" << std::endl;
        return false;
    }

    size_t name_length;
    // Deserialize maze data: name length, name, dimensions, grid
    file.read(reinterpret_cast<char *>(&name_length), sizeof(name_length));
    current_maze.name.resize(name_length);
    file.read(&current_maze.name[0], name_length);
    file.read(reinterpret_cast<char *>(&current_maze.height), sizeof(current_maze.height));
    file.read(reinterpret_cast<char *>(&current_maze.width), sizeof(current_maze.width));

    current_maze.grid.assign(current_maze.height, std::vector<char>(current_maze.width));
    for (auto &row : current_maze.grid) {
        file.read(row.data(), row.size());
    }

    width = current_maze.width;
    height = current_maze.height;
    file.close();

    std::cout << "Maze " << current_maze.name << " successfully loaded" << std::endl;
    return true;
}

/*
 Returns a vector of neighboring cells for the solver, checking for walls between them.
 param c The current cell.
 return Vector of reachable neighboring cells.
 */
std::vector<cell> maze_generator::get_neighbors_solver(cell &c) const {
    std::vector<cell> res;
    std::vector<cell> cells;

    // Define potential neighbors
    cell up(c.x, c.y - 2);
    cell rt(c.x + 2, c.y);
    cell dw(c.x, c.y + 2);
    cell lt(c.x - 2, c.y);

    cells.insert(cells.end(), {up, rt, dw, lt});

    for (cell neighb : cells) {
        if (neighb.x < height && neighb.y < width && neighb.x > 0 && neighb.y > 0 &&
            current_maze.grid[neighb.x][neighb.y] != WALL && !current_maze.visited[neighb.x][neighb.y] &&
            current_maze.grid[(neighb.x + c.x) / 2][(neighb.y + c.y) / 2] != WALL) { // Check wall between
            res.push_back(neighb);
        }
    }
    return res;
}

//Solves the maze using a stack-based approach and displays the solution.

void maze_generator::solve() {
    // Ensure maze is initialized
    assert(current_maze.grid.size() > 0);

    if (current_maze.grid.size() <= 0) {
        std::cout << "Please generate or load maze first" << std::endl;
        return;
    }
    std::stack<cell> solution;
    cell current_cell(1, 1);         // Start at entrance
    cell neighbor_cell;
    std::vector<std::vector<char>> solved_grid = current_maze.grid; // Copy grid for solution

    // Reset visited array
    for (int i = 0; i < height; ++i)
        for (int j = 0; j < width; ++j)
            current_maze.visited[i][j] = false;

    current_maze.visited[1][1] = VISITED;

    do {
        std::vector<cell> neighbors = get_neighbors_solver(current_cell);
        if (neighbors.size() > 0) {
            // Move to a neighbor and mark the path
            solution.push(current_cell);
            solved_grid[current_cell.x][current_cell.y] = PATH;
            current_maze.visited[current_cell.x][current_cell.y] = VISITED;
            std::shuffle(neighbors.begin(), neighbors.end(), rng);
            neighbor_cell = neighbors[0];
            solution.push(neighbor_cell);
            solved_grid[(current_cell.x + neighbor_cell.x) / 2][(current_cell.y + neighbor_cell.y) / 2] = PATH;
            solved_grid[neighbor_cell.x][neighbor_cell.y] = PATH;
            current_maze.visited[neighbor_cell.x][neighbor_cell.y] = VISITED;
            current_cell = neighbor_cell;
        } else {
            // Backtrack by removing the last step
            solution.pop();
            solved_grid[current_cell.x][current_cell.y] = CELL;
            solved_grid[(current_cell.x + solution.top().x) / 2][(current_cell.y + solution.top().y) / 2] = CELL;
            current_cell = solution.top();
            solved_grid[current_cell.x][current_cell.y] = CELL;
        }
    } while (current_cell.x != height - 2 || current_cell.y != width - 2); // Until exit is reached

    std::stringstream ss;
    ss << "Viewing solved " << current_maze.name << " maze" << std::endl;

    // Print solved maze with path highlighted
    for (const auto &row : solved_grid) {
        for (char cell : row)
            if (cell == '*') {
                ss << RED << cell << RESET << CELL;
            } else {
                ss << cell << CELL;
            }
        ss << std::endl;
    }

    std::cout << ss.str();
}

/*
Allows the user to play the maze using WASD keys in the terminal.
Note: Uses Unix-specific terminal settings (termios), may not work on all platforms.
*/
void maze_generator::play() const {
    // Ensure maze is initialized
    assert(current_maze.grid.size() > 0);

    if (current_maze.grid.size() <= 0) {
        std::cout << "Please generate or load maze first" << std::endl;
        return;
    }

    // Save current terminal settings
    termios original_attributes;
    tcgetattr(STDIN_FILENO, &original_attributes);

    // Modify terminal settings: disable canonical mode and echo for real-time input
    termios modified_attributes = original_attributes;
    modified_attributes.c_lflag &= ~(ICANON | ECHO);
    modified_attributes.c_cc[VMIN] = 0;  // Do not wait for input
    modified_attributes.c_cc[VTIME] = 0; // Return immediately
    tcsetattr(STDIN_FILENO, TCSANOW, &modified_attributes);

    std::vector<std::vector<char>> play_grid = current_maze.grid; // Copy grid for playing
    cell current_cell(1, 1); // Start at entrance

    while (current_cell.x != height - 2 || current_cell.y != width - 2) { // Until exit
        char c;
        play_grid[current_cell.x][current_cell.y] = ' '; // Clear current position

        if (read(STDIN_FILENO, &c, 1) == 1) {
            if (c == 'q') break; // Quit game
            // Move if no wall in the direction
            else if (c == 'w' && play_grid[current_cell.x - 1][current_cell.y] != WALL && (current_cell.x != 1 || current_cell.y != 1))
                current_cell.x -= 2;
            else if (c == 's' && play_grid[current_cell.x + 1][current_cell.y] != WALL)
                current_cell.x += 2;
            else if (c == 'a' && play_grid[current_cell.x][current_cell.y - 1] != WALL)
                current_cell.y -= 2;
            else if (c == 'd' && play_grid[current_cell.x][current_cell.y + 1] != WALL)
                current_cell.y += 2;
        }

        play_grid[current_cell.x][current_cell.y] = '*'; // Mark player position
        driver_logic::clear_screen(); // Clear screen for redraw

        std::stringstream ss;
        for (const auto &row : play_grid) {
            for (char cell : row)
                if (cell == '*') {
                    ss << RED << cell << RESET << CELL; // Highlight player
                } else {
                    ss << cell << CELL;
                }
            ss << std::endl;
        }

        std::cout << ss.str();
        std::cout << "Use WASD to move, Q to quit" << std::endl;
        std::cout << std::flush;

        usleep(100000); // Small delay for smoother display
    }

    // Restore original terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &original_attributes);
}

} 