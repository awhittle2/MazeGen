#include "vector"
#include <algorithm>
#include <iostream>
#include <utility>
#include <cmath>
#include <random>
#include <chrono>

using namespace std;

// This function finds the number of neighbors of a given cell in a maze that are zeros.
int findNeighbors(vector<vector<int>> maze, int x, int y) {
  // Initialize a count variable to 0
  int count = 0;
  // Define a vector of pairs of directions
  vector<pair<int,int>> directions = {{1,0},{0,1},{-1,0},{0,-1}};

  // Loop through the directions
  for(int i = 0; i < 4; i++) {
    // Calculate the new x and y coordinates of the neighbor
    int nx = x + directions[i].first;
    int ny = y + directions[i].second;

    // Skip if the new coordinates are out of bounds
    if(nx < 0 || ny < 0 || nx >= maze.size() || ny >= maze[0].size()) continue;

    // If the neighbor is a 0, increment the count
    if(maze[nx][nx] == 0) {
      count = count + 1;
    }
  }

  // Return the number of neighbors
  return count;
}

// Generates a maze using the depth-first search algorithm.
void dfsMaze(vector<vector<int>>& maze, vector<vector<bool>> visited, vector<pair<int,int>> neighbors, unsigned seed, int x,int y) {
  // Check if the current position is out of bounds, has been visited, or is not a valid space for the maze
  if(x < 0 || x >= maze.size() || y < 0 || y >= maze[0].size() || visited[x][y] || maze[x][y] == 0) return;

  // Mark the current position as visited
  visited[x][y] = true;

  // Count the number of neighbors of the current position
  int count = 0;
  count = findNeighbors(maze, x, y);

  // If the current position has more than one neighbor, return (this is not a dead end)
  if(count > 1) return;

  // Set the current position to be a part of the maze
  maze[x][y] = 0;

  // Shuffle the order of the neighbors
  shuffle(neighbors.begin(), neighbors.end(), default_random_engine(seed));

  // Recursively call the function on each neighbor
  dfsMaze(maze, visited, neighbors, seed, x+neighbors[0].first, y+neighbors[0].second);
  dfsMaze(maze, visited, neighbors, seed, x+neighbors[1].first, y+neighbors[1].second);
  dfsMaze(maze, visited, neighbors, seed, x+neighbors[2].first, y+neighbors[2].second);
  dfsMaze(maze, visited, neighbors, seed, x+neighbors[3].first, y+neighbors[3].second);
}

// Randomly sets the goal position in the maze
void setGoal(vector<vector<int>>& maze) {
  // Flag to keep track of whether the goal has been set
  bool isSet = false;

  // Loop until the goal has been set
  while(!isSet) {
    // Generate random coordinates
    int x = rand() % maze.size();
    int y = rand() % maze[0].size();

    // Check if the chosen position is valid (not part of the maze)
    if(maze[x][y] != 1) {
      // Set the position as the goal
      maze[x][y] = 2;

      // Set the flag to true, indicating that the goal has been set
      isSet = true;
    }
  }
}

// Randomly sets the starting position of Pac-Man in the maze
pair<int,int> setPacMan(vector<vector<int>> maze) {
  // Flag to keep track of whether Pac-Man has been placed
  bool isSet = false;

  // Loop until Pac-Man has been placed
  while(!isSet) {
    // Generate random coordinates
    int x = rand() % maze.size();
    int y = rand() % maze[0].size();

    // Check if the chosen position is valid (not part of the maze or the goal position)
    if(maze[x][y] != 1 && maze[x][y] != 2) {
      // Set the flag to true, indicating that Pac-Man has been placed
      isSet = true;

      // Return the coordinates of Pac-Man
      return {x,y};
    }
  }

  // Return an empty pair if Pac-Man could not be placed
  return {};
}

// Generates a maze using the depth-first search algorithm and sets the goal and starting positions for Pac-Man
void dfsMazeGenerator(vector<vector<int>>& maze) {
  // Create a 2D vector to keep track of the positions that have been visited in the maze
  vector<vector<bool>> visited(maze.size(), vector<bool>(maze[0].size(), false));

  // Create a vector to store the relative coordinates of the neighbors of a given position
  vector<pair<int,int>> neighbors = {{1,0},{0,1},{-1,0},{0,-1}};
 
  // Use the current time as the seed for the random number generator
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();

  // Generate the maze starting from the center of the grid
  dfsMaze(maze, visited, neighbors, seed, ceil(maze.size()/2), ceil(maze[0].size()/2));

  // Set the goal position in the maze
  setGoal(maze);

  // Set the starting position of Pac-Man in the maze
  pair<int,int> starting = setPacMan(maze);

  // Output the starting position of Pac-Man to a file
  freopen("output.txt","w",stdout);
  cout << "Pacman's starting position: X : " << starting.first << "   Y: " << starting.second << "\n\n";
  fclose(stdout);
}

int main() {
  // Set the size of the maze
  int n = 10;

  // Create a 2D vector to represent the maze
  vector<vector<int>> maze(n, vector<int>(n,1));

  // Generate the maze and set the goal and starting positions for Pac-Man
  dfsMazeGenerator(maze);

  // Output the maze to a file
  freopen("output.txt","w",stdout);

  // Output the start of the array
  cout << "{";

  // Loop over the rows of the maze
  for(int i = 0; i < maze.size(); i++) {
    // Output the start of a new row
    cout << "{";

    // Loop over the columns of the maze
    for(int j = 0; j < maze.size(); j++) {
      // Output the value at the current position
      cout << maze[i][j];

      // If this is not the last column, output a comma
      if(j != maze[0].size() - 1) cout << ",";
    }

    // Output the end of the row
    cout << "}";

    // If this is not the last row, output a comma and a newline
    if(i != maze.size() - 1) cout << "," << "\n";
  }

  // Output the end of the array
  cout << "}";

  // Close the output file
    fclose(stdout);

    return 0;
}
