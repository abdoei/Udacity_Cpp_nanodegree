# Garbage Collector

### Introduction:

Garbage Collector implemented using **Reference Counting Algorithm**. The algorithm selected can easily be implemented upon the existing C++ dynamic allocation system. for more information about the algorithm, please refer to [this](https://en.wikipedia.org/wiki/Reference_counting) wikipedia link.

### Installation:

1. Clone the repository using command `git clone https://github.com/abdoei/Udacity_Cpp_nanodegree.git`.

### Project Execution:

1. Compile `GarbageCollector.cpp` file by executing command `g++ -o main main.cpp`.
2. Execute command `./main`.
3. Results should now be displayed on the screen.

### Screenshots:

- Memory Leakage Result:![Results](<Screenshot from 2023-09-15 12-39-59.png>)



## Project TODO List:
- [x] Complete `Pointer` constructor
- [x] Complete `Pointer` `operator==`
- [x] Complete `Pointer` destructor
- [x] Complete `PtrDetails` class
- [ ] Implement a pooling mechanism for `PtrDetails` objects

