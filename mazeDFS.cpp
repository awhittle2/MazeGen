#include "vector"
#include <algorithm>
#include <iostream>
#include <utility>
#include <cmath>
#include <random>
#include <chrono>

using namespace std;

int findNeighbors(vector<vector<int>> maze, int x, int y) {
  int count = 0;
  vector<pair<int,int>> directions = {{1,0},{0,1},{-1,0},{0,-1}};

  for(int i = 0; i < 4; i++) {
    int nx = x + directions[i].first;
    int ny = y + directions[i].second;

    if(nx < 0 || ny < 0 || nx >= maze.size() || ny >= maze[0].size()) continue;

    if(maze[nx][ny] == 0) {
      count = count + 1;
    }
  }
  return count;
}

void dfsMaze(vector<vector<int>>& maze, vector<vector<bool>> visited, vector<pair<int,int>> neighbors, unsigned seed, int x,int y) {
  if(x < 0 || x >= maze.size() || y < 0 || y >= maze[0].size() || visited[x][y] || maze[x][y] == 0) return;

  visited[x][y] = true;
  int count = 0;
  count = findNeighbors(maze, x, y);

  if(count > 1) return;
  
  maze[x][y] = 0;

  shuffle(neighbors.begin(), neighbors.end(), default_random_engine(seed));
  
  dfsMaze(maze, visited, neighbors, seed, x+neighbors[0].first, y+neighbors[0].second);
  dfsMaze(maze, visited, neighbors, seed, x+neighbors[1].first, y+neighbors[1].second);
  dfsMaze(maze, visited, neighbors, seed, x+neighbors[2].first, y+neighbors[2].second);
  dfsMaze(maze, visited, neighbors, seed, x+neighbors[3].first, y+neighbors[3].second);
}

void setGoal(vector<vector<int>>& maze) {
  bool isSet = false;
  while(!isSet) {
    int x = rand() % maze.size();
    int y = rand() % maze[0].size();

    if(maze[x][y] != 1) {
      maze[x][y] = 2;
      isSet = true;
    }
  }
}

pair<int,int> setPacMan(vector<vector<int>> maze) {
  bool isSet = false;
  while(!isSet) {
    int x = rand() % maze.size();
    int y = rand() % maze[0].size();

    if(maze[x][y] != 1 && maze[x][y] != 2) {
      isSet = true;
      return {x,y};
    }
  }
  return {};
}

void dfsMazeGenerator(vector<vector<int>>& maze) {
  vector<vector<bool>> visited(maze.size(), vector<bool>(maze[0].size(), false));
  vector<pair<int,int>> neighbors = {{1,0},{0,1},{-1,0},{0,-1}};
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();

  dfsMaze(maze, visited, neighbors, seed, ceil(maze.size()/2), ceil(maze[0].size()/2));

  setGoal(maze);
  pair<int,int> starting = setPacMan(maze);
  cout << "Pacman's starting position: X : " << starting.first << "   Y: " << starting.second << "\n\n";
}

int main() {
  int n = 10;
  vector<vector<int>> maze(n, vector<int>(n,1)); // Initialize vector of size n by n full of walls

  dfsMazeGenerator(maze);

  cout << "{";
  for(int i = 0; i < maze.size(); i++) {
    cout << "{";
    for(int j = 0; j < maze.size(); j++) {
      cout << maze[i][j] << (j == maze[0].size() - 1 ? "" : ",");
    }
    cout << "}\n";
  }
  cout << "}";
}