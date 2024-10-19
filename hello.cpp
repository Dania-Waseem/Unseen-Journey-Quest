#include <iostream>
#include "curses.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
using namespace std;

class Node
{
public:
    char value; 
    bool key;
    bool coin;
    bool door;
    bool bomb;
    Node *up, *down, *left, *right;

    Node(char val) : value(val), up(NULL), down(NULL), left(NULL), right(NULL), door(false), key(false),bomb(false) {}
};

class Grid
{
public:
    Node *head;

    Grid(int size)
    {
        head = createGrid(size);
    }

    Node *createGrid(int size)
    {
        Node *start = NULL;
        Node *prevRowStart = NULL;
        Node *prev = NULL;

        for (int i = 0; i < size; i++)
        {
            Node *rowStart = NULL;
            Node *temp = NULL;

            for (int j = 0; j < size; j++)
            {
                temp = new Node('.'); 
                if (j == 0)
                {
                    rowStart = temp; 
                }
                else
                {
                    prev->right = temp;
                    temp->left = prev;
                }

                if (prevRowStart != NULL)
                {
                    prevRowStart->down = temp;
                    temp->up = prevRowStart;
                    prevRowStart = prevRowStart->right;
                }

                prev = temp; 
            }

            prevRowStart = rowStart; 
            prev = NULL;

            if (start == NULL)
            {
                start = rowStart; 
            }
        }

        return start;
    }

void displayGrid(int size)
{
    Node *rowPtr = head;
    Node *colPtr;

    clear();                                     
    mvprintw(0, 0, "Grid: %d x %d", size, size); 

    int row = 2; 
    
    // Top boundary
    mvprintw(row - 1, 0, "# ");
    for (int i = 0; i < size; i++)
    {
        printw("# "); 
    }
    printw("#");

    
    while (rowPtr != NULL)
    {
        colPtr = rowPtr;
        float col = 1;
        
        mvprintw(row++, 0, "# "); 
        while (colPtr != NULL)
        {
            mvprintw(row - 1, col, " %c ", colPtr->value); // Space between cells
            col += 2.14; // spacing
            colPtr = colPtr->right;
        }
        mvprintw(row - 1, col, "#"); // Right boundary
        rowPtr = rowPtr->down;
    }

    // Bottom boundary
    mvprintw(row, 0, "# ");
    for (int i = 0; i < size; i++)
    {
        printw("# "); 
    }
    printw("#");

    refresh(); 
}


};

int main()
{
    initscr();   
    noecho();    
    cbreak();    
    curs_set(0); 


    int mode = 0;
    int gridSize = 0;

    mvprintw(0, 0, "Select Difficulty Level:\n1. Easy (10x10)\n2. Medium (15x15)\n3. Hard (20x20)\nEnter your choice: ");
    refresh(); 

    // Use scanw to capture numeric input for the mode
    scanw("%d", &mode); 

    clear();

    switch (mode)
    {
    case 1:
        gridSize = 10;
        break;
    case 2:
        gridSize = 15;
        break;
    case 3:
        gridSize = 20;
        break;
    default:
        mvprintw(0, 0, "Invalid choice! Exiting...");
        refresh();
        getch();
        endwin();
        return 0;
    }

    clear();                    
    Grid grid(gridSize);        
    grid.displayGrid(gridSize); 

    getch();  
    endwin(); 

    return 0;
}
