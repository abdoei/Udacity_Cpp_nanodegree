# BFS path finder
For a given map, find the shortest path from a start point to an end point using the breadth-first search algorithm. The map is a 2D grid of nodes stored in [1.board](./1.board) file, 1 represents an obstacle and 0 represents a free space. the start and end point are definded in the main function.

The algorithm can solve the problem while traversing diagonally or not.

For this map:
```
S, 1, 0, 0, 0, 0,
0, 1, 0, 0, 0, 0,
0, 1, 0, 0, 0, 0,
0, 1, 0, 0, 0, 0,
0, 0, 0, 0, 1, E,
```
The algorithm will find this path (with diagonal traversal):
```
Search result: 1
number of steps: 8
parent of end: 0x1c4041d6200
x: 4 y: 5
x: 3 y: 4
x: 4 y: 3
x: 4 y: 2
x: 4 y: 1
x: 3 y: 0
x: 2 y: 0
x: 1 y: 0
```
And this path (without diagonal traversal):
```
Search result: 1
number of steps: 11
parent of end: 0x1846473dec0
x: 4 y: 5
x: 3 y: 5
x: 3 y: 4
x: 3 y: 3
x: 3 y: 2
x: 4 y: 2
x: 4 y: 1
x: 4 y: 0
x: 3 y: 0
x: 2 y: 0
x: 1 y: 0
```