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

maze_gen::maze_generator M(0,0);

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
            M.get_width_and_height();
            clear_screen();
            display_menu();
            break;
        case 2:
            M.generate_maze();
            clear_screen();
            M.print_maze();
            display_menu();
            break;
        case 3:{
            std::cout<<"Enter the file name:" << std::endl;
            std::string filename;
            std::cin >> filename;
            clear_screen();
            M.save(filename);
            display_menu();
            }   
            break;        
        case 4:{
            std::cout<<"Enter the file name:" << std::endl;
            std::string filename;
            std::cin >> filename;
            clear_screen();
            if(M.load(filename))
                M.print_maze();
            else
                std::cout << "Failed to load maze. Please try again" << std::endl;
            display_menu();
            }   
            break;
        case 5:
            M.solve();
            display_menu();
            break;
        case 6:
            todo();
            display_menu();
            break;
        case 7:
            continue;
        default: 
            std::cout << "Please try again" << std::endl;
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            display_menu();
            break;
        }
    } while (choice != 7);
    exit(0);
}
}


