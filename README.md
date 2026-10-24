Tetris playable in terminal written in C++.
Written entirely by hand

Usage: tetris [OPTIONS]

Options:
-s: Sandbox mode - can increase level with u

Runs on two threads, one continuously reading input via stdin and applying actions,
the other responsible for advancing the game state every gravity tick. The game
level increases naturally every 10 lines cleared, causing gravity to speed up according
to the formula below:

gravity_period = (0.8 - (level - 1) * 0.007) ** (level - 1)
