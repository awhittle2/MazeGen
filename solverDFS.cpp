#include "vector"
#include <algorithm>
#include <iostream>
#include <utility>
#include <cmath>
#include <random>
#include <chrono>

using namespace std;

// Function to find the number of non wall valid neighbors
int findNeighbors(vector<vector<int>> maze, int x, int y) {
  int count = 0;
  // Valid directions
  vector<pair<int,int>> directions = {{1,0},{0,1},{-1,0},{0,-1}};

  // Goes through each direction
  for(int i = 0; i < 4; i++) {
    int nx = x + directions[i].first;
    int ny = y + directions[i].second;

    // If out of bounds, go to the next neighbor
    if(nx < 0 || ny < 0 || nx >= maze.size() || ny >= maze[0].size()) continue;

    // If not a wall
    if(maze[nx][ny] == 0) {
      // Increase the count
      count = count + 1;
    }
  }
  return count;
}

// Function to recursively preform dfs to create a maze
void dfsMaze(vector<vector<int>>& maze, vector<vector<bool>> visited, vector<pair<int,int>> neighbors, unsigned seed, int x,int y) {
  // If out of bounds, visited, or not a wall return
  if(x < 0 || x >= maze.size() || y < 0 || y >= maze[0].size() || visited[x][y] || maze[x][y] == 0) return;

  // Mark as visited
  visited[x][y] = true;
  int count = 0;
  // Finds the number of nonwall neighbors
  count = findNeighbors(maze, x, y);

  // If there is 2 or more nonwall neighbors return
  if(count > 1) return;

  // Make the current coordinates not a wall
  maze[x][y] = 0;

  // Shuffles the directions
  shuffle(neighbors.begin(), neighbors.end(), default_random_engine(seed));

  // Goes to the first direction
  dfsMaze(maze, visited, neighbors, seed, x+neighbors[0].first, y+neighbors[0].second);
  // Goes to the second direction
  dfsMaze(maze, visited, neighbors, seed, x+neighbors[1].first, y+neighbors[1].second);
  // Goes to the third direction
  dfsMaze(maze, visited, neighbors, seed, x+neighbors[2].first, y+neighbors[2].second);
  // Goes to the fourth direction
  dfsMaze(maze, visited, neighbors, seed, x+neighbors[3].first, y+neighbors[3].second);
}

// Function to set the goal in the maze
void setGoal(vector<vector<int>>& maze) {
  bool isSet = false;
  while(!isSet) {
    // Random x
    int x = rand() % maze.size();
    // Random y
    int y = rand() % maze[0].size();

    // If the coords is not a wall
    if(maze[x][y] != 1) {
      // Set it to the goal
      maze[x][y] = 2;
      isSet = true;
    }
  }
}

// Function to set the starting position for pacman
pair<int,int> setPacMan(vector<vector<int>> maze) {
  bool isSet = false;
  while(!isSet) {
    // Random x
    int x = rand() % maze.size();
    // Random y
    int y = rand() % maze[0].size();

    // If the coords is not a wall or the goal
    if(maze[x][y] != 1 && maze[x][y] != 2) {
      isSet = true;
      // Set starting to that position
      return {x,y};
    }
  }
  return {};
}

// Function to create vectors, call the recursive function, and set the goal and starting position
void dfsMazeGenerator(vector<vector<int>>& maze) {
  // Vector to hold visited
  vector<vector<bool>> visited(maze.size(), vector<bool>(maze[0].size(), false));
  // Vector to hold possible directions of the neighbors
  vector<pair<int,int>> neighbors = {{1,0},{0,1},{-1,0},{0,-1}};
  // Allows for random maze generation
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();

  dfsMaze(maze, visited, neighbors, seed, ceil(maze.size()/2), ceil(maze[0].size()/2));

  // Sets the goal and adds it to the maze
  setGoal(maze);
  // Finds the starting position
  pair<int,int> starting = setPacMan(maze);
  // Oututs to txt file
  freopen("output.txt","w",stdout);
  cout << "Pacman's starting position: X : " << starting.first << "   Y: " << starting.second << "\n\n";
  fclose(stdout);
}

int main() {
  // Size of maze (nxn)
  int n = 10;
  // Vector to hold maze, initialized to be all walls
  vector<vector<int>> maze(n, vector<int>(n,1)); // Initialize vector of size n by n full of walls

  dfsMazeGenerator(maze);

  // Outputs the entire maze
  // Outputs to txt file
  freopen("output.txt","w",stdout);
  cout << "{";
  for(int i = 0; i < maze.size(); i++) {
    cout << "{";
    for(int j = 0; j < maze.size(); j++) {
      cout << maze[i][j] << (j == maze[0].size() - 1 ? "" : ",");
    }
    cout << "}" << (i == maze.size() - 1 ? "" : ",") << "\n";
  }
  cout << "}";
  fclose(stdout);

  // Program close
  return 0;
}