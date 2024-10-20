// Dania Waseem 23i2622
// Assignment 2
// Data Structures

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
    bool hasKey;

    Inventory() : coinHead(NULL), score(0), hasKey(false) {}

    void addCoin(int row, int col)
    {
        CoinNode *newCoin = new CoinNode(row, col);
        newCoin->next = coinHead;
        coinHead = newCoin;
        score += 2;
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
        return score + remainingMoves; // total score
    }
};

// stack implemented to calculate the exact previous movement
// in this way we can implement the undo feature

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
        if (top >= 0)
        {
            return data[top--];
        }
        else
        {
            return NULL; // if the stack is empty
        }
    }

    Node *peek()
    {
        if (top >= 0)
        {
            return data[top];
        }
        else
        {
            return NULL;
        }
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
    Node *playerPos;
    Node *keyPos;
    Node *doorPos;
    Stack moveHistory;
    Inventory inventory;
    int remainingMoves, remainingUndos;

    Grid(int size, int extraMoves, int undos)
    {
        head = createGrid(size);
        placeObjects(size);

        // calculating the total number  of moves allowed
        // using the distance of key and door

        int movesToKey = manhattanDistance(playerPos, keyPos);
        int movesToDoor = manhattanDistance(keyPos, doorPos);

        remainingMoves = movesToKey + movesToDoor + extraMoves;
        remainingUndos = undos;
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
                temp = new Node('.', i, j);
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

    void placeObjects(int size)
    {
        srand(time(0));

        playerPos = getRandomNode(size);
        playerPos->value = 'P';

        keyPos = getRandomNode(size);
        keyPos->key = true;
        keyPos->value = 'K';

        doorPos = getRandomNode(size);
        doorPos->door = true;
        doorPos->value = 'D';

        Node *bombPos = getRandomNode(size);
        bombPos->bomb = true;
        bombPos->value = 'B';

        // placing the  coins randomly
        for (int i = 0; i < 5; i++)
        {
            Node *coinPos = getRandomNode(size);
            if (coinPos->value == '.')
            {
                coinPos->coin = true;
                coinPos->value = 'C';
            }
        }
    }

    Node *getRandomNode(int size)
    {
        int randRow = rand() % size;
        int randCol = rand() % size;
        Node *node = head;
        for (int i = 0; i < randRow; i++)
        {
            node = node->down;
        }
        for (int i = 0; i < randCol; i++)
        {
            node = node->right;
        }
        return node;
    }

    int manhattanDistance(Node *a, Node *b)
    {
        return abs(a->row - b->row) + abs(a->col - b->col);
    }

    void movePlayer(char direction)
    {
        if (remainingMoves <= 0)
        {
            gameOver("No moves left! Game over.");
            return;
        }

        Node *newPos = NULL;

        switch (direction)
        {
        case 'w': // Up
            newPos = playerPos->up;
            break;

        case 's': // Down
            newPos = playerPos->down;
            break;

        case 'a': // Left
            newPos = playerPos->left;
            break;

        case 'd': // Right
            newPos = playerPos->right;
            break;
        default:
            return;
        }

        if (newPos != NULL)
        {
            if (newPos->bomb)
            {
                gameOver("You stepped on a bomb! Game over.");
                return;
            }

            if (newPos->door)
            {
                // if (inventory.coinHead)
                if (inventory.hasKey)
                {
                    gameOver("Congratulations! You found the door and have the key! You win!");
                    return;
                }
                else
                {
                    // prevent moving onto the door if the player doesn't have the key
                    mvprintw(6, 0, "You need a key to pass through the door.");
                    return; 
                }
            }

            // undo if revisiting the same position
            if (!moveHistory.isEmpty() && moveHistory.peek() == newPos)
            {
                undoMove();
                return;
            }

            moveHistory.push(playerPos);

            // moving player to the new position
            playerPos->value = '.';
            playerPos = newPos;
            playerPos->value = 'P';

            if (playerPos->key)
            {
                inventory.hasKey = true;
                // removing key from grid
                playerPos->key = false;
                inventory.score += 5; //  add points for collecting the key
            }

            // Check for coins
            if (playerPos->coin)
            {
                inventory.addCoin(playerPos->row, playerPos->col);
                playerPos->coin = false;
                remainingUndos++;
            }

            remainingMoves--;
            mvprintw(0, 0, "Moves left: %d  Undos left: %d  Score: %d", remainingMoves, remainingUndos, inventory.score);
            refresh();
        }
    }
    void gameOver(const char *message)
    {
        clear();
        mvprintw(0, 0, "%s", message);
        mvprintw(1, 0, "Final Score: %d", inventory.calculateFinalScore(remainingMoves));

        mvprintw(2, 0, "Collected Coins (in order): ");
        CoinNode *temp = inventory.coinHead;
        while (temp != NULL)
        {
            printw("(%d, %d) ", temp->row, temp->col);
            temp = temp->next;
        }

        if (remainingMoves == 0)
        {
            mvprintw(3, 0, "You ran out of moves. Game over.");
        }
        else
        {
            mvprintw(3, 0, "Congratulations on completing the game!");
        }

        refresh();
        getch();
        endwin();
        exit(0);
    }

    void undoMove()
    {
        if (remainingUndos > 0 && !moveHistory.isEmpty())
        {
            Node *previousPos = moveHistory.pop();
            playerPos->value = '.';
            playerPos = previousPos;
            playerPos->value = 'P';
            remainingUndos--;

            mvprintw(0, 0, "Moves left: %d  Undos left: %d", remainingMoves, remainingUndos);

            if (remainingUndos == 0)
            {
                mvprintw(7, 0, "You used all your undo features.");
            }
            refresh();
        }
    }

    void displayGrid(int size)
    {
        Node *rowPtr = head;
        Node *colPtr;

        clear();
        mvprintw(0, 0, "Grid: %d x %d", size, size);

        mvprintw(1, 0, "Mode: %s | Moves left: %d | Undos left: %d | Key status: %s",
                 (size == 10) ? "Easy" : (size == 15) ? "Medium"
                                                      : "Hard",
                 remainingMoves, remainingUndos, (inventory.hasKey ? "Obtained" : "Not Obtained"));

        // updataing the hint
        int currentDistance = manhattanDistance(playerPos, keyPos);
        static int previousDistance = manhattanDistance(playerPos, keyPos);
        if (currentDistance < previousDistance)
        {
            mvprintw(4, 0, "Hint: Getting Closer to the key!");
        }
        else if (currentDistance > previousDistance)
        {
            mvprintw(4, 0, "Hint: You are getting further away from the key.");
        }
        else
        {
            mvprintw(4, 0, "Hint: You are at the same distance from the key.");
        }
        previousDistance = currentDistance;

        int row = 6;

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
                col += 2.14;                                   // spacing
                colPtr = colPtr->right;
            }
            // Right boundary
            mvprintw(row - 1, col, "#");
            rowPtr = rowPtr->down;
        }

        // Bottom boundary
        mvprintw(row, 0, "# ");
        for (int i = 0; i < size; i++)
        {
            printw("# ");
        }
        printw("#");

        mvprintw(row + 2, 0, "Score: %d", inventory.score);
        refresh();
    }
};

int main()
{
    initscr();
    noecho();
    cbreak();
    keypad(stdscr, TRUE); // Enable special keys

    int difficulty;
    mvprintw(0, 0, "Select Difficulty Level:\n1. Easy (10x10)\n2. Medium (15x15)\n3. Hard (20x20)\nEnter your choice: ");
    refresh();

    //  scanw to capture numeric input for the mode
    scanw("%d", &difficulty);

    int size, extraMoves, undos;

    switch (difficulty)
    {
    // Easy
    case 1:
        size = 10;
        extraMoves = 6;
        undos = 6;
        break;
    case 2:
        // Medium
        size = 15;
        extraMoves = 2;
        undos = 4;
        break;
    case 3:
        // Hard
        size = 20;
        extraMoves = 0;
        undos = 1;
        break;
    // Default to Easy
    default:
        size = 10;
        extraMoves = 6;
        undos = 6;
    }

    Grid gameGrid(size, extraMoves, undos);
    gameGrid.displayGrid(size);

    char input;

    while (true)
    {
        input = getch();

        // to quit
        if (input == 'q')
        {
            break;
        }
        if (input == 'u')
        {
            gameGrid.undoMove();
        }
        else
        {
            gameGrid.movePlayer(input);
        }

        gameGrid.displayGrid(size);
        gameGrid.inventory.displayCollectedCoins();
    }

    endwin();
    return 0;
}
