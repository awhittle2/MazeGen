#include "vector"
#include <algorithm>
#include <iostream>
#include <utility>
#include <cmath>
#include <random>
#include <chrono>

using namespace std;

// This function finds the number of neighbors of a cell in a maze that are not walls.
// It takes in a 2D vector representing the maze, the x and y coordinates of the cell,
// and returns the number of valid neighbors.
int findNeighbors(vector<vector<int>> maze, int x, int y) {
  // Initialize the count to zero
  int count = 0;

  // These are the valid directions for a neighbor
  vector<pair<int,int>> directions = {{1,0},{0,1},{-1,0},{0,-1}};

  // Iterate through each direction
  for(int i = 0; i < 4; i++) {
    // Calculate the coordinates of the neighbor in this direction
    int nx = x + directions[i].first;
    int ny = y + directions[i].second;

    // If the neighbor is out of bounds, go to the next neighbor
    if(nx < 0 || ny < 0 || nx >= maze.size() || ny >= maze[0].size()) continue;

    // If the neighbor is not a wall, increment the count
    if(maze[nx][ny] == 0) {
      count = count + 1;
    }
  }

  // Return the final count
  return count;
}

// This function performs a depth-first search on a maze to create a random maze.
// It takes in a reference to the 2D vector representing the maze, a 2D vector
// representing whether a cell has been visited, a vector of pairs representing
// the valid directions for a neighbor, a seed for the random number generator,
// and the x and y coordinates of the current cell.
void dfsMaze(vector<vector<int>>& maze, vector<vector<bool>> visited, vector<pair<int,int>> neighbors, unsigned seed, int x,int y) {
  // If the current cell is out of bounds, or has been visited, or is a wall, return
  if(x < 0 || x >= maze.size() || y < 0 || y >= maze[0].size() || visited[x][y] || maze[x][y] == 0) return;

  // Mark the current cell as visited
  visited[x][y] = true;

  // Count the number of neighbors of the current cell that are not walls
  int count = 0;
  count = findNeighbors(maze, x, y);

  // If the current cell has more than one valid neighbor, return
  if(count > 1) return;

  // Otherwise, make the current cell a wall
  maze[x][y] = 0;

  // Shuffle the valid directions for the neighbors
  shuffle(neighbors.begin(), neighbors.end(), default_random_engine(seed));

  // Recursively call the function for each valid neighbor
  dfsMaze(maze, visited, neighbors, seed, x+neighbors[0].first, y+neighbors[0].second);
  dfsMaze(maze, visited, neighbors, seed, x+neighbors[1].first, y+neighbors[1].second);
  dfsMaze(maze, visited, neighbors, seed, x+neighbors[2].first, y+neighbors[2].second);
  dfsMaze(maze, visited, neighbors, seed, x+neighbors[3].first, y+neighbors[3].second);
}


// This function sets the goal of a maze by randomly selecting a cell and setting
// it as the goal, provided it is not a wall.
// It takes in a reference to the 2D vector representing the maze.
void setGoal(vector<vector<int>>& maze) {
  // Flag to check if the goal has been set
  bool isSet = false;

  // Keep trying until the goal is set
  while(!isSet) {
    // Generate random x and y coordinates within the bounds of the maze
    int x = rand() % maze.size();
    int y = rand() % maze[0].size();

    // If the cell at these coordinates is not a wall, set it as the goal
    if(maze[x][y] != 1) {
      maze[x][y] = 2;
      isSet = true;
    }
  }
}

// This function sets the starting position of the Pac-Man in a maze by randomly
// selecting a cell and setting it as the starting position, provided it is not
// a wall or the goal.
// It takes in a 2D vector representing the maze and returns the x and y coordinates
// of the starting position as a pair.
pair<int,int> setPacMan(vector<vector<int>> maze) {
  // Flag to check if the starting position has been set
  bool isSet = false;

  // Keep trying until the starting position is set
  while(!isSet) {
    // Generate random x and y coordinates within the bounds of the maze
    int x = rand() % maze.size();
    int y = rand() % maze[0].size();

    // If the cell at these coordinates is not a wall or the goal, set it as the starting position
    if(maze[x][y] != 1 && maze[x][y] != 2) {
      isSet = true;
      return {x,y};
    }
  }

  // Return an empty pair if the starting position could not be set
  return {};
}

// This function generates a random maze using a depth-first search algorithm.
// It takes in a reference to the 2D vector representing the maze.
void dfsMazeGenerator(vector<vector<int>>& maze) {
  // Create a 2D vector to keep track of which cells have been visited
  vector<vector<bool>> visited(maze.size(), vector<bool>(maze[0].size(), false));

  // These are the valid directions for a neighbor
  vector<pair<int,int>> neighbors = {{1,0},{0,1},{-1,0},{0,-1}};

  // Generate a random seed
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();

  // Perform a depth-first search on the maze, starting from the center
  dfsMaze(maze, visited, neighbors, seed, ceil(maze.size()/2), ceil(maze[0].size()/2));

  // Set the goal of the maze
  setGoal(maze);

  // Set the starting position of the Pac-Man
  pair<int,int> starting = setPacMan(maze);

  // Print the starting position of the Pac-Man to the output file
  freopen("output.txt","w",stdout);
  cout << "Pacman's starting position: X : " << starting.first << "   Y: " << starting.second << "\n\n";
  fclose(stdout);
}

int main() {
  // Set the size of the maze
  int n = 10;

  // Initialize the maze with all cells as walls
  vector<vector<int>> maze(n, vector<int>(n,1));

  // Generate a random maze using a depth-first search algorithm
  dfsMazeGenerator(maze);

  // Open the output file
  freopen("output.txt","w",stdout);

  // Print the maze to the output file
  cout << "{";
  for(int i = 0; i < maze.size(); i++) {
    cout << "{";
    for(int j = 0; j < maze.size(); j++) {
      // Print the cell and a comma, unless it is the last cell in the row
      cout << maze[i][j] << (j == maze[0].size() - 1 ? "" : ",");
    }
    // Print a line break and a comma, unless it is the last row
    cout << "}" << (i == maze.size() - 1 ? "" : ",") << "\n";
  }
  cout << "}";

  // Close the output file
  fclose(stdout);

  return 0;
}
