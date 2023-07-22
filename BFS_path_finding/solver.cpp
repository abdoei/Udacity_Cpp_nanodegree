#include "solver.hpp"

int main()
{
    auto board = ReadBoardFile("1.board");
    vector<vector<Node>> map = mapOfNodes(board);
    Node start(0,0,0,true,nullptr);
    Node end(4,5,0,true,nullptr);
    bool found = search(map, start, end, false);
    std::cout << "Search result: " << found << "\n";
    std::cout << "number of steps: " << end.g << "\n";
    std::cout << "parent of end: " << end.parent << "\n";

    // print path from end to start
    Node *current = &end;
    while(current->parent != nullptr){
        std::cout << "x: " << current->x << " y: " << current->y << "\n";
        current = current->parent;
    }
    // PrintBoard(board);
    return 0;
}
