# Unseen-Journey-Quest
## Grid Game (C++ and pdcurses)
## How to run the game?
</br>
1.Clone this repository to your local machine.</br>
2.Ensure you have the required libraries installed </br>
    the needed libraries are: Pdcurses</br>
3.Compile the code using a C++ compiler:</br>
g++ -o grid_adventure game.cpp -lcurses</br>
4.Run the game</br>
./grid_adventure</br>

### Introduction
</br>
A 2D maze game which is made using **multidimentional linked lists**.</br> The player navigates a grid, collecting coins and a key while avoiding bombs. The objective is to reach the door after acquiring the key within a limited number of moves. </br>The game incorporates an undo feature to allow players to revert their last move.
</br>

### Key Features
</br>
**Player Movement**: Navigate through the grid using keyboard inputs (WASD for movement).</br>
'w' -> to move up</br>
'a' -> to move left</br>
's' -> to move down</br>
'd' -> to move right</br>
**Key Collection**: Collect the key to unlock the door. The key also grants an additional 5 points to your score.</br>
**Coin Collection**: Collect coins scattered throughout the grid, each worth 2 points.</br>
**Door Interaction**: You cannot pass through the door unless you have collected the key.</br>
**Bombs**: Stepping on a bomb ends the game immediately.</br>
**Undo Feature**: Revisit previous positions with limited undos.</br>
**Hints**: Receive hints based on your proximity to the key, helping you strategize your movements.</br>
**Scoring System**: Your final score is calculated based on collected coins, the key, and remaining moves.</br>

## Game Mechanics

### Game Start
</br>
After running the game u will see the following screen: </br>
Select your desired level by pressing, 1, 2 or 3 and then press Enter.</br>

![1](https://github.com/user-attachments/assets/ba9dc0d9-9465-4891-bc30-66bbf4162f3a)

### During Gameplay
![2](https://github.com/user-attachments/assets/10b77aa8-ee47-44f0-8516-bc44b8478304)

### Game Over
![3](https://github.com/user-attachments/assets/ea1e3494-fadb-44a8-bfbf-16bfd712dbaa)
![image](https://github.com/user-attachments/assets/29221cec-ab64-4dcf-8e7c-da28206a8874)

## Built With
C++</br>
PdCurses

