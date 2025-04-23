#include <iostream>
#include "driver.h"
#include "maze.h"

namespace driver_logic {
void clear_screen(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void display_menu(){
    std::cout << R"(
Maze Generator 
    1. Set dimensions
    2. Generate new maze
    3. Save maze 
    4. Load maze
    5. Solve maze
    6. Solve maze yourself
    7. Exit)" << std::endl;
}

void todo(){
    std::cout << "Yet to be implemented :(" << std::endl;
}

int run(){
    clear_screen();
    display_menu();
    int choice;
    do{
        std::cin >> choice;
        clear_screen();
        switch(choice){
        case 1:
            maze_gen::maze_generator::get_width_and_height();
            clear_screen();
            display_menu();
            break;
        case 2:
            maze_gen::maze_generator::generate_maze();
            maze_gen::maze_generator::print_maze();
            display_menu();
            break;
        case 3:
            todo();
            break;        
        case 4:
            todo();
            break;
        case 5:
            todo();
            break;
        case 6:
            todo();
            break;
        case 7:
            continue;
        default: 
            std::cout << "Please try again" << std::endl;
            break;
        }
    } while (choice != 7);
    exit(0);
}
}


