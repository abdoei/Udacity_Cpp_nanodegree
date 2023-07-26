#include <bits/stdc++.h>
#include <cstdlib>
using namespace std;

enum class State // strongly typed enum means kEmpty is not implicitly converted to 0 and so on
{
  kEmpty,
  kObstacle,
  kClosed,
  kPath,
  kStart,
  kFinish
};

// prototype
string CellString(State cell);
void PrintBoard(const vector<vector<State>> &board);
auto ReadBoardFile(string path) -> vector<vector<State>>;
int Heuristic(int x1, int y1, int x2, int y2);
void AddToOpen(int x, int y, int g, int h, vector<vector<int>> &openNodes, vector<vector<State>> &map);
auto Search(vector<vector<State>> &board, int init[2], int goal[2]) -> vector<vector<State>>;
bool Compare(const vector<int> &v1, const vector<int> &v2);
void CellSort(vector<vector<int>> *v);
bool CheckValidCell(int x, int y, vector<vector<State>> &grid);
void ExpandNeighbors(const vector<int> &current, vector<vector<int>> &open, vector<vector<State>> &grid, int goal[2]);


vector<State> ParseLine(string line)
{
  istringstream sline(line);
  int n;
  char c;
  vector<State> row;
  while (sline >> n >> c && c == ',')
  {
    if (n == 0)
    {
      row.push_back(State::kEmpty);
    }
    else
    {
      row.push_back(State::kObstacle);
    }
  }
  return row;
}

auto ReadBoardFile(string path) -> vector<vector<State>>
{
  ifstream myfile(path);
  vector<vector<State>> board{};
  if (myfile)
  {
    string line;
    while (getline(myfile, line))
    {
      vector<State> row = ParseLine(line);
      board.push_back(row);
    }
  }
  return board;
}

bool Compare(const vector<int> &v1, const vector<int> &v2)
{
  return ((v1[2] + v1[3]) > (v2[2] + v2[3]));
}
/**
 * Sort the two-dimensional vector of ints in descending order.
 */
void CellSort(vector<vector<int>> *v)
{
  sort(v->begin(), v->end(), Compare);
}

// TODO: Write the Search function stub here.
auto Search(vector<vector<State>> &board,const int init[2],const int goal[2]) -> vector<vector<State>>
{
  vector<vector<int>> open{};
  int x = init[0], y = init[1], g = 0;
  int h = Heuristic(x, y, goal[0], goal[1]);
  AddToOpen(x, y, g, h, open, board);

  while (!open.empty())
  {
    CellSort(&open);
    auto current = open.back();
    open.pop_back();
    int x = current[0], y = current[1];
    int h = Heuristic(x, y, goal[0], goal[1]);
    board[x][y] = State::kPath;
    if (x == goal[0] && y == goal[1])
    {
      board[init[0]][init[1]] = State::kStart;
      board[goal[0]][goal[1]] = State::kFinish;
      cout << "Goal found!" << endl;
      return board;
    }
    ExpandNeighbors(current, open, board, goal);
  }
  vector<std::vector<State>> path{};
  cout << "No path found! \n";
  return path;
}

string CellString(State cell)
{
  switch (cell)
  {
  case State::kObstacle:
    return "⛰️\t";
  case State::kPath:
    return "🚗\t";
  case State::kStart:
    return "🚦\t";
  case State::kFinish:
    return "🏁\t";
  default:
    return "🕳\t";
  }
}

void PrintBoard(const vector<vector<State>> &board)
{
  for (size_t i = 0; i < board.size(); i++)
  {
    for (size_t j = 0; j < board[i].size(); j++)
    {
      cout << CellString(board[i][j]);
    }
    cout << "\n";
  }
}

int Heuristic(int x1, int y1, int x2, int y2)
{
  return (abs(x1 - x2) + abs(y1 - y2));
}

void AddToOpen(int x, int y, int g, int h, vector<vector<int>> &openNodes, vector<vector<State>> &map)
{
  vector<int> node = {x, y, g, h};
  openNodes.emplace_back(node);
  map[x][y] = State::kClosed;
}

bool CheckValidCell(int x, int y, vector<vector<State>> &grid)
{
  bool on_grid = (x >= 0 && x < grid.size());
  on_grid = (on_grid && (x >= 0 && grid[0].size()));
  if (on_grid)
    return (grid[x][y] == State::kEmpty);
  return false;
}

void ExpandNeighbors(const vector<int> &current, vector<vector<int>> &open, vector<vector<State>> &grid, int goal[2])
{
  // directional deltas
  const int delta[4][2]{{-1, 0}, {0, -1}, {1, 0}, {0, 1}};
  // current node data
  int x = current[0];int y = current[1];int g = current[2];
  // int h = Heuristic(x, y, goal[0], goal[1]);
  for (const int *i : delta)
  {
    int x2 = x + i[0];
    int y2 = y + i[1];
    if (CheckValidCell(x2, y2, grid))
    {
      int g2 = g + 1;
      int h2 = Heuristic(x2, y2, goal[0], goal[1]);
      AddToOpen(x2, y2, g2, h2, open, grid);
    }
  }
}

int main()
{
  // TODO: Declare "init" and "goal" arrays with values {0, 0} and {4, 5} respectively.
  int init[2]{0, 0}, goal[2]{4, 5};
  auto board = ReadBoardFile("1.board");
  // TODO: Call Search with "board", "init", and "goal". Store the results in the variable "solution".
  std::vector<std::vector<State>> solution = Search(board, init, goal);
  // TODO: Change the following line to pass "solution" to PrintBoard.
  system("chcp 65001");
  PrintBoard(solution);
}
