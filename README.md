# Unseen-Journey-Quest
## Grid Game (C++ and pdcurses)
## How to run the game?
1.Clone this repository to your local machine.
2.Ensure you have the required libraries installed 
    the needed libraries are: Pdcurses
3.Compile the code using a C++ compiler:
g++ -o grid_adventure game.cpp -lcurses
4.Run the game
./grid_adventure

### Introduction
A 2D maze game which is made using **multidimentional linked lists**. The player navigates a grid, collecting coins and a key while avoiding bombs. The objective is to reach the door after acquiring the key within a limited number of moves. The game incorporates an undo feature to allow players to revert their last move.

### Key Features
**Player Movement**: Navigate through the grid using keyboard inputs (WASD for movement).
'w' -> to move up
'a' -> to move left
's' -> to move down
'd' -> to move right
**Key Collection**: Collect the key to unlock the door. The key also grants an additional 5 points to your score.
**Coin Collection**: Collect coins scattered throughout the grid, each worth 2 points.
**Door Interaction**: You cannot pass through the door unless you have collected the key.
**Bombs**: Stepping on a bomb ends the game immediately.
**Undo Feature**: Revisit previous positions with limited undos.
**Hints**: Receive hints based on your proximity to the key, helping you strategize your movements.
**Scoring System**: Your final score is calculated based on collected coins, the key, and remaining moves.

### Game Mechanics

**Game Start**

After running the game u will see the following screen: 
Select your desired level by pressing, 1, 2 or 3 and then press Enter.
![1](https://github.com/user-attachments/assets/efdf2ecf-3f05-4450-9426-4abe9733ddab)


**During Gameplay**
![2](https://github.com/user-attachments/assets/10b77aa8-ee47-44f0-8516-bc44b8478304)


**Game Over**
![3](https://github.com/user-attachments/assets/ea1e3494-fadb-44a8-bfbf-16bfd712dbaa)
![image](https://github.com/user-attachments/assets/29221cec-ab64-4dcf-8e7c-da28206a8874)
