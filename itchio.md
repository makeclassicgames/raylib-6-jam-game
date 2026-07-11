# Hex Wars

Hex Wars is a turn-based strategy game where players control armies on a hexagonal grid. The objective is to outmaneuver and defeat your opponents using strategic planning and tactical decisions.

This game is part of the Raylib 6.x Game Jam (2026) and is developed using the Raylib C/C++ Game Development Library.

You control the Blue Team and your opponent controls the Red Team. The game is played on a hexagonal grid where each player takes turns moving their units and attacking the opponent's units.

Each Unit has unique properties and can be used strategically to outmaneuver the opponent. The game also features different terrain types that can affect the outcome of battles (On development).

## Features

* Turn Based gameplay on a hexagonal grid.
* Multiple units with unique properties.
* Unique terrain types.
* Merge Mode: Combine attacks for increased damage.
* Defense mode: Fortify your units to withstand enemy attacks.

## Controls

Mouse

* Left Click: Select unit / Move unit / Attack.
* Right Click: Cancel action / Deselect unit.   

Mobile
* Tap: Select unit / Move unit / Attack.
* Double Tap: Cancel action / Deselect unit.

## Game Instructions

Each Team has a set of units that can be moved and used to attack the opponent's units. The game is played in turns, with each player taking their turn to move and attack.

Each unit has unique properties that can be used strategically to outmaneuver the opponent. 

To control one of your units, simply click on the unit to select it.

You can see the units properties at the right side of the screen. You can also see the units properties by clicking on the unit.

To control the unit select one of the available actions (Move, Attack, Merge, Defense) and follow the instructions on the screen.

### Move

Move an unit to a new position on the hexagonal grid. Each unit has a limited number of moves per turn. You can see the number of moves available for each unit at the right side of the screen.

One Unit can be move once per turn.

### Attack

Attack an opponent's unit. Each unit can only attack once per turn, and have a limited range of attack. You can see the range of attack for each unit at the right side of the screen.

One unit can attack once per turn; and if an unit attack, it cannot move in the same turn.

### Merge

This is an special move that allows you to combine the attacks of two units for increased damage. You can only merge two units that to minus 3 cells away from each other. When you select merge, first you need to select the unit that will be merged, and and then select the opponent's unit that will be attacked. The damage of the attack will be increased by the number of units that are merged.

When two units are merged, the attack at the same time and cannot be used again in the same turn.

### Defense

Defense allows you to stay in the same position and fortify your unit to withstand enemy attacks. You can only use defense once per turn, and the unit will be fortified until the next turn.

When a unit is fortified, it cannot move or attack in the same turn.

The defense increase armor unit x1.5 and the unit will be fortified until the next turn.

## Terrain Types

Each cell of the hexagonal grid can have different terrain types that can affect the outcome of battles. The terrain types are:

* **Grass** (Green): No effect on units.
* **Water** (Blue): Units Except Planes cannot move through water cells.
* **Mountain** (Gray): Units Except Planes and soldiers cannot move through mountain cells.
* **Forest** (Dark Green): Units Except Planes and Tanks cannot move through forest cells.

**NOTE**: This feature is still under development and will be implemented in future updates.

## Units

There are different types of units that can be used in the game. Each unit has unique properties that can be used strategically to outmaneuver the opponent.

* **Soldier**: Basic unit with low health and attack. Can move 3 cells per turn and attack 2 cell away.
* **Tank**: Heavy unit with high health and attack. Can move 4 cells per turn and attack 3 cell away. This unit have heavy armor and can withstand more damage than other units.
* **Plane**: Fast unit with low health and attack. Can move 10 cells per turn and attack 5 cell away. This unit can fly over water and mountain cells. This unit have low armor and can be easily destroyed by other units.

## Turns

This game is playeed in turns, with each player taking their turn to move and attack. Each player has a limited number of moves and attacks per turn, and the game ends when one player has no units left on the board.

## Game Development

This game is developed using the Raylib C/C++ Game Development Library.

The Game is developed by:

* Zerasul (Make Classic Games) -  Game Design and Programming.
* Emiliollbb - Programming, Art and Sound Design.

All the source code is available on GitHub and is licensed under the GPL v3.0 License. You can check the [LICENSE](https://github.com/makeclassicgames/raylib-6-jam-game/blob/music/LICENSE) file for further details.

You can check the source code at the next Link:

[https://github.com/makeclassicgames/raylib-6-jam-game/](https://github.com/makeclassicgames/raylib-6-jam-game/)


## Issues and Feedback

This game is still under development and we are looking for feedback and suggestions to improve the game. If you have any issues or suggestions, please feel free to send an issue or comment.

Thank you and we hope you enjoy the game!
