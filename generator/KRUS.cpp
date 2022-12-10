#include "vector"
#include <algorithm>
#include <iostream>
#include <utility>
#include <cmath>
#include <random>
#include <chrono>
#include <set>

using namespace std;

// This function generates a random valid index in the given maze.
pair<int,int> randomInd(vector<vector<int>> maze) {
  bool isValid = false;
  int x = 0, y = 0;

  // Keep generating random indices until a valid one is found.
  while(!isValid) {
    x = rand() % maze.size();
    y = rand() % maze[0].size();

    // Check if the current index is valid, i.e. contains a 0.
    if(maze[x][y] == 0) {
      isValid = true;
    }
  }
  return {x,y};
}

// This function generates a random direction for the given cell in the given maze.
vector<pair<int,int>> randomDir(vector<vector<int>> maze, pair<int,int> cell) {
  vector<pair<int,int>> output;
  vector<pair<int,int>> directions = {{1,0},{0,1},{-1,0},{0,-1}};
  pair<int,int> wall;
  pair<int,int> cellB;

  // Choose a random direction.
  int randInd = rand() % directions.size();
  wall = {cell.first + directions[randInd].first, cell.second + directions[randInd].second};

  // Check if the chosen direction is valid, i.e. not outside the maze.
  if(wall.first < 0 || wall.first >= maze.size() || wall.second < 0 || wall.second >= maze.size()) {
    // If not valid, try again recursively.
    return randomDir(maze, cell);
  }

  cellB = {wall.first + directions[randInd].first, wall.second + directions[randInd].second};

  // Check if the next cell in the chosen direction is valid, i.e. not outside the maze.
  if(cellB.first < 0 || cellB.first >= maze.size() || cellB.second < 0 || cellB.first >= maze.size()) {
    // If not valid, try again recursively.
    return randomDir(maze, cell);
  }

  output.push_back(wall);
  output.push_back(cellB);
  return output;
}

// This function updates the given maze and number matrix to merge the groups of the given cell and cellB.
void reGroup(vector<vector<int>>& maze, vector<vector<int>>& num, pair<int,int> cell, pair<int,int> wall, pair<int,int> cellB) {
  int newGroup = 0;
  int oldGroup = 0;

  // Determine which group to merge into the other group.
  if(num[cell.first][cell.second] > num[cellB.first][cellB.second]) {
    newGroup = num[cell.first][cell.second];
    oldGroup = num[cellB.first][cellB.second];
  } else if(num[cellB.first][cellB.second] > num[cell.first][cell.second]){
    newGroup = num[cellB.first][cellB.second];
    oldGroup = num[cell.first][cell.second];
  } else {
    // If the two cells are already in the same group, return without doing anything.
    return;
  }

  // Update the number matrix to reflect the merged group.
  for(int i = 0; i < num.size(); i++) {
    for(int j = 0; j < num[0].size(); j++) {
      if(num[i][j] == oldGroup && num[i][j] != -1) num[i][j] = newGroup;
    }
  }

  // Update the maze to reflect the new wall.
  num[wall.first][wall.second] = newGroup;
  maze[wall.first][wall.second] = 0;
}

// This function checks if the given number matrix represents a perfect maze.
// A perfect maze is one where all cells belong to a single group, i.e. are connected.
bool isPerfectMaze(vector<vector<int>> num, int count) {
  for(int i = 0; i < num.size(); i++) {
    for(int j = 0; j < num.size(); j++) {
      if(num[i][j] != -1 && num[i][j] != count) {
        // If any cell is not in the same group as the starting cell, return false.
        return false;
      }
    }
  }
  // If all cells are in the same group as the starting cell, return true.
  return true;
}


// This function checks if the given cell is connected to the maze.
bool isConnected(vector<vector<int>> maze, pair<int,int> cellB) {
  // If the cell is a 0, it is connected to the maze.
  if(maze[cellB.first][cellB.second] == 0) return true;
  // Otherwise, it is not connected.
  return false;
}

// This function generates a maze using the Kruskal's algorithm.
void krusMaze(vector<vector<int>>& maze, vector<vector<int>>& num, int count) {
  // Check if the maze is perfect, i.e. all cells are in the same group.
  if(!isPerfectMaze(num, count)) {
    // If not, choose a random cell.
    pair<int,int> cell = randomInd(maze);
    // Choose a random direction from the cell.
    vector<pair<int,int>> output = randomDir(maze, cell);
    pair<int,int> wall = output[0];
    pair<int,int> cellB = output[1];

    // Check if the chosen direction is already part of the maze.
    if(maze[wall.first][wall.second] == 0) {
      // If it is, try again recursively.
      return krusMaze(maze, num, count);
    }

    // Check if the next cell in the chosen direction is connected to the maze.
    if(isConnected(maze, cellB)) {
      // If it is, merge the groups of the current cell and the next cell.
      reGroup(maze, num, cell, wall, cellB);
    }

    // Repeat the process until a perfect maze is generated.
    krusMaze(maze, num, count);
  } else {
    // If a perfect maze is generated, return.
    return;
  }
}

// This function sets the goal cell in the given maze.
void setGoal(vector<vector<int>>& maze) {
  bool isSet = false;
  while(!isSet) {
    int x = rand() % maze.size();
    int y = rand() % maze[0].size();

    // Check if the chosen cell is not a wall.
    if(maze[x][y] != 1) {
      // If it is not a wall, set it as the goal cell.
      maze[x][j] = 2;
      isSet = true;
    }
  }
}

// This function sets the Pac-Man cell in the given maze.
pair<int,int> setPacMan(vector<vector<int>> maze) {
  bool isSet = false;
  while(!isSet) {
    int x = rand() % maze.size();
    int y = rand() % maze[0].size();

    // Check if the chosen cell is not a wall or the goal cell.
    if(maze[x][y] != 1 && maze[x][y] != 2) { 
      isSet = true;
      // Return the coordinates of the chosen cell.
      return {x,y};
    }
  }
  // If no suitable cell is found, return an empty pair.
  return {};
}

// This function generates a maze using the Kruskal's algorithm.
// It also sets the goal and starting position for Pac-Man in the maze.
void krusMazeGenerator(vector<vector<int>>& maze) {
  // Initialize the random number generator with the current time.
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();
  srand(seed);

  // Create a number matrix for keeping track of the groups of cells in the maze.
  vector<vector<int>> num(maze.size(), vector<int>(maze[0].size(), -1));
  int count = -1;

  // Initialize the maze and number matrix.
  for(int i = 0; i < maze.size(); i+=2) {
    for(int j = 0; j < maze[0].size(); j+=2) {
      if(i%2 == 0 && j%2 == 0) {
        // Set every second cell as a 0.
        maze[i][j] = 0;
        count = count + 1;
        // Set the group of the current cell in the number matrix.
        num[i][j] = count;
      }
    }
  }

  // Generate the maze using the Kruskal's algorithm.
  krusMaze(maze, num, count);

  // Set the goal cell in the maze.
  setGoal(maze);

  // Set the starting position for Pac-Man in the maze.
  pair<int,int> starting = setPacMan(maze);

  // Open the output file in write mode.
  freopen("output.txt","w",stdout);
}

int main() {
  // Set the size of the maze.
  int n = 11;
  // Initialize the maze with walls (1).
  vector<vector<int>> maze(n, vector<int>(n,1));

  // Generate the maze.
  krusMazeGenerator(maze);

  // Open the output file in write mode.
  freopen("output.txt","w",stdout)
  // Print the maze in the output file.
  cout << "{";
  for(int i = 0; i < maze.size(); i++) {
    cout << "{";
    for(int j = 0; j < maze.size(); j++) {
      // Print each cell, followed by a comma unless it is the last cell in the row.
      cout << maze[i][j] << (j == maze[0].size() - 1 ? "" : ",");
    }
    // Print a closing bracket and a newline, unless it is the last row.
    cout << "}" << (i == maze.size() - 1 ? "" : ",") << "\n";
  }
  // Print a closing bracket to end the maze.
  cout << "}";
  // Close the output file.
  fclose(stdout);

  return 0;
}
