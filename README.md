Tetris playable in terminal written in C++

Usage: tetris \<frameLengthMs\> \<ticksPerGravTick\>

Runs on two threads, one reading input from stdin and writing to a buffer,
the other advancing the game state every `frameLengthMs`, with a gravity tick
every `ticksPerGravTick`
