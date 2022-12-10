#include "vector"
#include <algorithm>
#include <iostream>
#include <utility>
#include <cmath>
#include <random>
#include <chrono>

using namespace std;

// This function counts the number of walls surrounding a given coordinate (x, y) in a maze.
int findNeighbors(vector<vector<int>> maze, int x, int y) {
  int count = 0;
  // These are the four possible directions in which the function can check for walls.
  vector<pair<int,int>> directions = {{1,0},{0,1},{-1,0},{0,-1}};

  for(int i = 0; i < 4; i++) {
    // nx and ny are the coordinates of the adjacent cell in the direction specified by the current value of i.
    int nx = x + directions[i].first;
    int ny = y + directions[i].second;

    // If the coordinates are outside the bounds of the maze, skip them.
    if(nx < 0 || ny < 0 || nx >= maze.size() || ny >= maze[0].size()) continue;

    // If there is a wall at the adjacent cell, increment the count.
    if(maze[nx][nx] == 0) {
      count = count + 1;
    }
  }
  // Return the number of walls surrounding the given coordinate.
  return count;
}

// This function is used to generate the maze using the depth-first search algorithm.
void dfsMaze(vector<vector<int>>& maze, vector<vector<bool>> visited, vector<pair<int,int>> neighbors, unsigned seed, int x,int y) {
  // If the current coordinates are outside the bounds of the maze, or if the current cell has already been visited, or if the current cell is a wall, return.
  if(x < 0 || x >= maze.size() || y < 0 || y >= maze[0].size() || visited[x][y] || maze[x][y] == 0) return;
  // Mark the current cell as visited.
  visited[x][y] = true;
  // Count the number of walls surrounding the current cell.
  int count = 0;
  count = findNeighbors(maze, x, y);

  // If the number of walls surrounding the current cell is greater than 1, return.
  if(count > 1) return;

  // Set the current cell to a path.
  maze[x][y] = 0;

  // Shuffle the list of possible directions.
  shuffle(neighbors.begin(), neighbors.end(), default_random_engine(seed));

  // Recursively generate the maze in each of the possible directions.
  dfsMaze(maze, visited, neighbors, seed, x+neighbors[0].first, y+neighbors[0].second);
  dfsMaze(maze, visited, neighbors, seed, x+neighbors[1].first, y+neighbors[1].second);
  dfsMaze(maze, visited, neighbors, seed, x+neighbors[2].first, y+neighbors[2].second);
  dfsMaze(maze, visited, neighbors, seed, x+neighbors[3].first, y+neighbors[3].second);
}

void setGoal(vector<vector<int>>& maze) {
  bool isSet = false;
  // Keep generating random coordinates until a valid location for the goal is found.
  while(!isSet) {
    int x = rand() % maze.size();
    int y = rand() % maze[0].size();

    // If the current cell is not a wall, set it as the goal and end the loop.
    if(maze[x][y] != 1) {
      maze[x][y] = 2;
      isSet = true;
    }
  }
}

pair<int,int> setPacMan(vector<vector<int>> maze) {
  bool isSet = false;
  // Keep generating random coordinates until a valid location for Pac-Man is found.
  while(!isSet) {
    int x = rand() % maze.size();
    int y = rand() % maze[0].size();

    // If the current cell is not a wall or the goal, set it as the starting location for Pac-Man and return the coordinates.
    if(maze[x][y] != 1 && maze[x][y] != 2) {
      isSet = true;
      return {x,y};
    }
  }
  // If no valid starting location for Pac-Man was found, return an empty pair of coordinates.
  return {};
}

void dfsMazeGenerator(vector<vector<int>>& maze) {

  // Create a 2D vector to keep track of which cells have been visited during the maze generation.
  vector<vector<bool>> visited(maze.size(), vector<bool>(maze[0].size(), false));

  // These are the four possible directions in which the maze generation can proceed.
  vector<pair<int,int>> neighbors = {{1,0},{0,1},{-1,0},{0,-1}};
 
  // Use the current time as a seed for the random number generator.
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();

  // Start generating the maze at the middle of the grid.
  dfsMaze(maze, visited, neighbors, seed, ceil(maze.size()/2), ceil(maze[0].size()/2));

  // Set the goal in the maze.
  setGoal(maze);

  // Set the starting location for Pac-Man in the maze.
  pair<int,int> starting = setPacMan(maze);

  // Open the output file for writing and print the starting position of Pac-Man.
  freopen("output.txt","w",stdout);
  cout << "Pacman's starting position: X : " << starting.first << "   Y: " << starting.second << "\n\n";
  fclose(stdout);
}

int main() {
  // Set the size of the maze.
  int n = 10;
  // Initialize the maze with walls.
  vector<vector<int>> maze(n, vector<int>(n,1));

  // Generate the maze.
  dfsMazeGenerator(maze);

// Open the output file for writing and print the opening bracket for the maze representation.
freopen("output.txt","w",stdout);
cout << "{";

// Loop through each row in the maze.
for(int i = 0; i < maze.size(); i++) {
  // Print the opening bracket for the current row.
  cout << "{";
  // Loop through each cell in the current row.
  for(int j = 0; j < maze.size(); j++) {
    // Print the value of the current cell. If it is the last cell in the row, don't print a comma after it.
    cout << maze[i][j] << (j == maze[0].size() - 1 ? "" : ",");
  }
  // Print the closing bracket for the current row. If it is the last row, don't print a comma after it.
  cout << "}" << (i == maze.size() - 1 ? "" : ",") << "\n";
}
// Print the closing bracket for the maze representation and close the output file.
cout << "}";
fclose(stdout);
}
