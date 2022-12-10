#include "vector"
#include <algorithm>
#include <iostream>
#include <utility>
#include <cmath>
#include <random>
#include <chrono>
#include <set>

using namespace std;

// Generates a pair of random x,y coordinates in the maze where the cell at those coordinates is empty
pair<int,int> randomInd(vector<vector<int>> maze) {
  bool isValid = false;
  int x = 0, y = 0;
  while(!isValid) {
    x = rand() % maze.size(); // Generates a random number between 0 and the size of the maze
    y = rand() % maze[0].size();

    if(maze[x][y] == 0) { // Check if the cell at the generated coordinates is empty
      isValid = true;
    }
  }
  return {x,y};
}

// Generates two random coordinates representing a wall and the cell on the other side of that wall from a given cell
vector<pair<int,int>> randomDir(vector<vector<int>> maze, pair<int,int> cell) {
  vector<pair<int,int>> output;
  vector<pair<int,int>> directions = {{1,0},{0,1},{-1,0},{0,-1}}; // Vector of possible directions to check (right, down, left, up)
  pair<int,int> wall;
  pair<int,int> cellB;

  int randInd = rand() % directions.size(); // Generate a random index to pick a random direction from the vector of directions
  wall = {cell.first + directions[randInd].first, cell.second + directions[randInd].second}; // Calculate the coordinates of the wall based on the direction

  if(wall.first < 0 || wall.first >= maze.size() || wall.second < 0 || wall.second >= maze.size()) return randomDir(maze, cell); // Recursively call this function if the wall is out of bounds
  cellB = {wall.first + directions[randInd].first, wall.second + directions[randInd].second}; // Calculate the coordinates of the cell on the other side of the wall

  if(cellB.first < 0 || cellB.first >= maze.size() || cellB.second < 0 || cellB.first >= maze.size()) return randomDir(maze, cell); // Recursively call this function if the cell is out of bounds

  output.push_back(wall);
  output.push_back(cellB);
  return output;
}

// Changes the group that a given cell belongs to, and updates the maze and group number matrices accordingly
void reGroup(vector<vector<int>>& maze, vector<vector<int>>& num, pair<int,int> cell, pair<int,int> wall, pair<int,int> cellB) {
  int newGroup = 0;
  int oldGroup = 0;

  // Determine which group the cell belongs to
  if(num[cell.first][cell.second] > num[cellB.first][cellB.second]) {
    newGroup = num[cell.first][cell.second];
    oldGroup = num[cellB.first][cellB.second];
  } else if(num[cellB.first][cellB.second] > num[cell.first][cell.second]){
    newGroup = num[cellB.first][cellB.second];
    oldGroup = num[cell.first][cell.second];
  } else {
    return;
  }

  // Update the group number matrix with the new group number
  for(int i = 0; i < num.size(); i++) {
    for(int j = 0; j < num[0].size(); j++) {
      if(num[i][j] == oldGroup && num[i][j] != -1) num[i][j] = newGroup;
    }
  }

  // Update the maze and group number matrices with the new wall and updated group number
  num[wall.first][wall.second] = newGroup;
  maze[wall.first][wall.second] = 0;
}

// Check if the maze is perfect, i.e. all cells belong to the same group and there are no isolated cells
bool isPerfectMaze(vector<vector<int>> num, int count) {
  for(int i = 0; i < num.size(); i++) {
    for(int j = 0; j < num.size(); j++) {
      if(num[i][j] != -1 && num[i][j] != count) return false; // Return false if a cell does not belong to the correct group
    }
  }
  return true;
}

// Check if the cell on the other side of the wall is connected to the maze
bool isConnected(vector<vector<int>> maze, pair<int,int> cellB) {
  if(maze[cellB.first][cellB.second] == 0) return true; // Return true if the cell is connected
  return false;
}

// Recursively generate the maze
void krusMaze(vector<vector<int>>& maze, vector<vector<int>>& num, int count) {
  if(!isPerfectMaze(num, count)) { // Stop recursion if the maze is perfect
   pair<int,int> cell = randomInd(maze); // Generate random coordinates for a cell in the maze

    vector<pair<int,int>> output = randomDir(maze, cell); // Generate coordinates for the wall and the cell on the other side of the wall
    pair<int,int> wall = output[0];
    pair<int,int> cellB = output[1];

    if(maze[wall.first][wall.second] == 0) return krusMaze(maze, num, count); // Recursively call this function if the wall is already present in the maze

    if(isConnected(maze, cellB)) { // If the cell on the other side of the wall is connected to the maze, add the wall and update the group number
      reGroup(maze, num, cell, wall, cellB);
    }

    krusMaze(maze, num, count); // Recursively call this function to continue generating the maze
  } else {
    return;
  }
}

// Set the goal cell in the maze
void setGoal(vector<vector<int>>& maze) {
  bool isSet = false; // Flag to track if the goal has been set
  while(!isSet) {
    int x = rand() % maze.size(); // Generate random coordinates for the goal cell
    int y = rand() % maze[0].size();

    if(maze[x][y] != 1) { // Set the goal cell if it is not a wall
      maze[x][j] = 2;
      isSet = true;
    }
  }
}

// Set the starting position of Pac-Man in the maze
pair<int,int> setPacMan(vector<vector<int>> maze) {
  bool isSet = false; // Flag to track if the starting position has been set
  while(!isSet) {
    int x = rand() % maze.size(); // Generate random coordinates for the starting position
    int y = rand() % maze[0].size();

    if(maze[x][y] != 1 && maze[x][y] != 2) { // Set the starting position if it is not a wall or the goal
      isSet = true;
      return {x,y};
    }
  }
  return {};
}

// Generate a Kruskal's maze and set the goal and starting position of Pac-Man
void krusMazeGenerator(vector<vector<int>>& maze) {
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();
  srand(seed); // Initialize random seed using system clock
  
  vector<vector<int>> num(maze.size(), vector<int>(maze[0].size(), -1)); // Create a grid with the same dimensions as the maze and initialize all elements to -1
  int count = -1;
  
  for(int i = 0; i < maze.size(); i+=2) {
    for(int j = 0; j < maze[0].size(); j+=2) {
      if(i%2 == 0 && j%2 == 0) { // If the current cell is on an even row and column
        maze[i][j] = 0; // Set the cell as part of the maze
        count = count + 1; // Increment the group count
        num[i][j] = count; // Set the cell's group number to the current count
      }
    }
  }
  
  krusMaze(maze, num, count);
  
  setGoal(maze); // Place the goal (2) in the maze
  pair<int,int> starting = setPacMan(maze); // Randomly choose a starting position for Pac-Man in the maze
  freopen("output.txt","w",stdout); // Open output file in write mode
  cout << "Pacman's starting position: X : " << starting.first << "   Y: " << starting.second << "\n\n"; // Print Pac-Man's starting position
  fclose(stdout); // Close output file
}

int main() {
  int n = 11;
  vector<vector<int>> maze(n, vector<int>(n,1)); // Initialize a grid of size 11x11 with all cells set as walls (1)

  krusMazeGenerator(maze); // Generate the maze

  freopen("output.txt","w",stdout); // Open output file in write mode
  cout << "{";
  for(int i = 0; i < maze.size(); i++) {
    cout << "{";
    for(int j = 0; j < maze.size(); j++) {
      cout << maze[i][j] << (j == maze[0].size() - 1 ? "" : ","); // Print the maze to output file
    }
    cout << "}" << (i == maze.size() - 1 ? "" : ",") << "\n";
  }
  cout << "}";
  fclose(stdout); // Close output file

  return 0;
}
