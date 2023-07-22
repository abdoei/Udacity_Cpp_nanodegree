#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

struct Node
{
    int x;
    int y;
    int g;
    bool open;
    Node *parent;

    // constructor
    Node() : x(-1), y(-1), g(-1), open(true), parent(nullptr) {}
    Node(int x, int y, int g, bool open, Node *parent) : x(x), y(y), g(g), open(open), parent(parent) {}
};

// prototype
std::vector<std::vector<int>> ReadBoardFile(std::string path);
void PrintBoard(const vector<vector<int>> &board);
std::vector<int> ParseLine(std::string line);
vector<vector<Node>> mapOfNodes(vector<vector<int>> &board);
vector<Node> getNeighbors(Node &node, vector<vector<Node>> &map);
bool search(vector<vector<Node>> &map, Node &start, Node &end);

std::vector<std::vector<int>> ReadBoardFile(std::string path)
{
    std::vector<std::vector<int>> board{};
    std::ifstream myfile(path);
    if (myfile)
    {
        std::string line;
        while (getline(myfile, line))
        {
            std::vector<int> row = ParseLine(line);
            board.push_back(row);
        }
    }
    return board;
}

void PrintBoard(const vector<vector<int>> &board)
{
    for (size_t i = 0; i < board.size(); ++i)
    {
        for (size_t j = 0; j < board[i].size(); ++j)
        {
            cout << board[i][j];
        }
        cout << "\n";
    }
}

std::vector<int> ParseLine(std::string line)
{
    std::istringstream sline(line);
    int n;
    char c;
    std::vector<int> row;
    while (sline >> n >> c)
    {
        row.push_back(n);
    }
    return row;
}

vector<vector<Node>> mapOfNodes(vector<vector<int>> &board)
{
    vector<vector<Node>> map;
    for (size_t i = 0; i < board.size(); i++)
    {
        vector<Node> row;
        for (size_t j = 0; j < board[i].size(); j++)
        {
            Node node;
            node.x = i;
            node.y = j;
            node.g = -1;
            node.open = (board[i][j] == 0) ? true : false;
            node.parent = nullptr;
            row.push_back(node);
        }
        map.push_back(row);
    }
    return map;
}

vector<Node> getNeighbors(Node &node, vector<vector<Node>> &map, bool diagonal = false)
{
    vector<Node> neighbors;
    int x = node.x;
    int y = node.y;
    // left
    if (x > 0 && map[x - 1][y].open)
    {
        neighbors.push_back(map[x - 1][y]);
    }
    // right
    if ((size_t)x < map.size() - 1 && map[x + 1][y].open)
    {
        neighbors.push_back(map[x + 1][y]);
    }
    // up
    if (y > 0 && map[x][y - 1].open)
    {
        neighbors.push_back(map[x][y - 1]);
    }
    // down
    if ((size_t)y < map[0].size() - 1 && map[x][y + 1].open)
    {
        neighbors.push_back(map[x][y + 1]);
    }

    /***************************************************************************/
    /**/ /* to solve with diagonal movement uncomment the following lines */ /**/
    /***************************************************************************/

    if(diagonal){
    // diagonal
        // left down
        if(x > 0 && y > 0 && map[x-1][y-1].open){
            neighbors.push_back(map[x-1][y-1]);
        }
        // right down
        if((size_t)x < map.size()-1 && y > 0 && map[x+1][y-1].open){
            neighbors.push_back(map[x+1][y-1]);
        }
        // left up
        if(x > 0 && (size_t)y < map[0].size()-1 && map[x-1][y+1].open){
            neighbors.push_back(map[x-1][y+1]);
        }
        // right up
        if((size_t)x < map.size()-1 && (size_t)y < map[0].size()-1 && map[x+1][y+1].open){
            neighbors.push_back(map[x+1][y+1]);
        }
    }
    return neighbors;
}

bool search(vector<vector<Node>> &map, Node &start, Node &end, bool diagonal = false)
{
    vector<Node> open;
    open.push_back(start);
    while (open.size() > 0)
    {
        Node *current = new Node(open[0]);
        open.erase(open.begin());
        current->open = false;
        vector<Node> neighbors = getNeighbors(*current, map, diagonal);
        for (size_t i = 0; i < neighbors.size(); i++)
        {
            Node neighbor = neighbors[i];
            if (neighbor.x == end.x && neighbor.y == end.y)
            {
                end.parent = current;
                end.g = current->g + 1;
                return true;
            }
            if (neighbor.open)
            {
                neighbor.parent = current;
                neighbor.g = current->g + 1;
                open.push_back(neighbor);
            }
        }
    }
    return false;
}