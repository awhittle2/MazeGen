#include "vector"
#include <algorithm>
#include <iostream>
#include <utility>
#include <cmath>
#include <random>
#include <chrono>
#include <set>

using namespace std;

pair<int,int> randomInd(vector<vector<int>> maze) {
  bool isValid = false;
  int x = 0, y = 0;
  while(!isValid) {
    x = rand() % maze.size();
    y = rand() % maze[0].size();

    if(maze[x][y] == 0) {
      isValid = true;
    }
  }
  return {x,y};
}

vector<pair<int,int>> randomDir(vector<vector<int>> maze, pair<int,int> cell) {
  vector<pair<int,int>> output;
  vector<pair<int,int>> directions = {{1,0},{0,1},{-1,0},{0,-1}};
  pair<int,int> wall;
  pair<int,int> cellB;

  int randInd = rand() % directions.size();
  wall = {cell.first + directions[randInd].first, cell.second + directions[randInd].second};

  if(wall.first < 0 || wall.first >= maze.size() || wall.second < 0 || wall.second >= maze.size()) return randomDir(maze, cell);

  cellB = {wall.first + directions[randInd].first, wall.second + directions[randInd].second};

  if(cellB.first < 0 || cellB.first >= maze.size() || cellB.second < 0 || cellB.first >= maze.size()) return randomDir(maze, cell);
  
  output.push_back(wall);
  output.push_back(cellB);
  return output;
}

void reGroup(vector<vector<int>>& maze, vector<vector<int>>& num, pair<int,int> cell, pair<int,int> wall, pair<int,int> cellB) {
  int newGroup = 0;
  int oldGroup = 0;
  
  if(num[cell.first][cell.second] > num[cellB.first][cellB.second]) {
    newGroup = num[cell.first][cell.second];
    oldGroup = num[cellB.first][cellB.second];
  } else if(num[cellB.first][cellB.second] > num[cell.first][cell.second]){
    newGroup = num[cellB.first][cellB.second];
    oldGroup = num[cell.first][cell.second];
  } else {
    return;
  }

  for(int i = 0; i < num.size(); i++) {
    for(int j = 0; j < num[0].size(); j++) {
      if(num[i][j] == oldGroup && num[i][j] != -1) num[i][j] = newGroup;
    }
  }
  num[wall.first][wall.second] = newGroup;
  maze[wall.first][wall.second] = 0;
}

bool isPerfectMaze(vector<vector<int>> num, int count) {
  // Loops through all groups and makes sure everything is in the same group
  for(int i = 0; i < num.size(); i++) {
    for(int j = 0; j < num.size(); j++) {
      if(num[i][j] != -1 && num[i][j] != count) return false;
    }
  }
  return true;
}

bool isConnected(vector<vector<int>> maze, pair<int,int> cellB) {
  if(maze[cellB.first][cellB.second] == 0) return true;
  return false;
}

void krusMaze(vector<vector<int>>& maze, vector<vector<int>>& num, int count) {
  // If not perfect maze
  if(!isPerfectMaze(num, count)) {
    // Get random index for starting cell
   pair<int,int> cell = randomInd(maze);
    
    // Get random direction
    vector<pair<int,int>> output = randomDir(maze, cell);
    pair<int,int> wall = output[0];
    pair<int,int> cellB = output[1];

    // If not a wall
    if(maze[wall.first][wall.second] == 0) return krusMaze(maze, num, count);

    // If it connects cell
    if(isConnected(maze, cellB)) {
      reGroup(maze, num, cell, wall, cellB);
    } else {
      // Set wall to cell's color
      num[wall.first][wall.second] = num[cell.first][cell.second];
    }

    // Run it again
    krusMaze(maze, num, count);
  } else { // If maze is perfect
    return;
  }
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

void krusMazeGenerator(vector<vector<int>>& maze) {
  // Random seed generator
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();
  srand(seed);
  // Keeps track of the different groups
  vector<vector<int>> num(maze.size(), vector<int>(maze[0].size(), -1)); // Walls are -1
  int count = -1;
  // Loops through the entire maze
  for(int i = 0; i < maze.size(); i++) {
    for(int j = 0; j < maze[0].size(); j++) {
      // If the row and column is odd
      if(i%2 == 0 && j%2 == 0) {
        // Open the wall
        maze[i][j] = 0;
        // Assign a unique group
        count = count + 1;
        num[i][j] = count;
      }
    }
  }

  krusMaze(maze, num, count);

  setGoal(maze);
  pair<int,int> starting = setPacMan(maze);
  cout << "Pacman's starting position: X : " << starting.first << "   Y: " << starting.second << "\n\n";
}

int main() {
  // N MUST BE ODD FOR THIS ALGO TO WORK SINCE EDGES ARE REPRESENTED BY 0S
  int n = 11;
  vector<vector<int>> maze(n, vector<int>(n,1)); // Initialize vector of size n by n full of walls

  krusMazeGenerator(maze);

  // Print maze
  cout << "{";
  for(int i = 0; i < maze.size(); i++) {
    cout << "{";
    for(int j = 0; j < maze.size(); j++) {
      cout << maze[i][j] << (j == maze[0].size() - 1 ? "" : ",");
    }
    cout << "}" << (i == maze.size() - 1 ? "" : ",") << "\n";
  }
  cout << "}";
}