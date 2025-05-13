#include <iostream>
#include "driver.h"
#include "maze.h"
namespace driver_logic {
/*
  Clears the console screen based on the operating system.
  Uses 'cls' for Windows and 'clear' for Unix-based systems.
 */
void clear_screen(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Global maze generator object initialized with zero dimensions
maze_gen::maze_generator M(0,0);

// Displays the main menu options for the maze generator program.
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

 //Runs the main loop of the maze generator program, handling user input and menu navigation.
int run(){
    clear_screen();
    display_menu();
    int choice;
    do{
        std::cin >> choice;
        clear_screen();
        switch(choice){
        case 1:
            // Prompt user to set maze width and height
            M.get_width_and_height();
            clear_screen();
            display_menu();
            break;
        case 2:
            // Generate and display a new maze
            M.generate_maze();
            clear_screen();
            M.print_maze();
            display_menu();
            break;
        case 3:{
            // Save the current maze to a file
            std::cout<<"Enter the file name:" << std::endl;
            std::string filename;
            std::cin >> filename;
            clear_screen();
            M.save(filename);
            display_menu();
            }   
            break;        
        case 4:{
            // Load a maze from a file and display it if successful
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
            // Solve the maze automatically and display the solution
            M.solve();
            display_menu();
            break;
        case 6:
            // Allow the user to solve the maze interactively
            M.play();
            display_menu();
            break;
        case 7:
            // Exit the program
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


