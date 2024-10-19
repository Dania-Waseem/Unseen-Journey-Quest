#include <iostream>
#include "curses.h"
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

class Node
{
public:
    char value; // '.' for empty, 'P' for player, 'K' for key, 'D' for door, 'B' for bomb
    bool key;
    bool coin;
    bool door;
    bool bomb;
    Node *up, *down, *left, *right;
    int row, col;

    Node(char val, int r, int c) : value(val), up(NULL), down(NULL), left(NULL), right(NULL), door(false), key(false), bomb(false), coin(false), row(r), col(c) {}
};

class CoinNode
{
public:
    int row, col;
    CoinNode *next;

    CoinNode(int r, int c) : row(r), col(c), next(NULL) {}
};

class Inventory
{
public:
    CoinNode *coinHead;
    int score;

    Inventory() : coinHead(NULL), score(0) {}

    void addCoin(int row, int col)
    {
        CoinNode *newCoin = new CoinNode(row, col);
        newCoin->next = coinHead;
        coinHead = newCoin;
        score += 2; // Each coin adds 2 points
    }

    void displayCollectedCoins()
    {
        mvprintw(2, 0, "Collected Coins (in order): ");
        CoinNode *temp = coinHead;
        while (temp != NULL)
        {
            printw("(%d, %d) ", temp->row, temp->col);
            temp = temp->next;
        }
    }

    int calculateFinalScore(int remainingMoves)
    {
        return score + remainingMoves; // Total score calculation
    }
};

class Stack
{
public:
    Node *data[100];
    int top;

    Stack() : top(-1) {}

    void push(Node *node)
    {
        if (top < 99)
        {
            data[++top] = node;
        }
    }

    Node *pop()
    {
        return (top >= 0) ? data[top--] : NULL;
    }

    Node *peek()
    {
        return (top >= 0) ? data[top] : NULL;
    }

    bool isEmpty()
    {
        return top == -1;
    }
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
