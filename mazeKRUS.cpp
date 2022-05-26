#include "vector"
#include <algorithm>
#include <iostream>
#include <utility>
#include <cmath>
#include <random>
#include <chrono>
#include <set>

using namespace std;

// Function to pick a random open cell
pair<int,int> randomInd(vector<vector<int>> maze) {
  bool isValid = false;
  int x = 0, y = 0;
  while(!isValid) {
    // Random x
    x = rand() % maze.size();
    // Random y
    y = rand() % maze[0].size();

    // If the cell is not a wall
    if(maze[x][y] == 0) {
      isValid = true;
    }
  }
  // Return the coords
  return {x,y};
}

// Function to pick a random valid direction
vector<pair<int,int>> randomDir(vector<vector<int>> maze, pair<int,int> cell) {
  // Returns two cells
  vector<pair<int,int>> output;
  // Valid list of direction
  vector<pair<int,int>> directions = {{1,0},{0,1},{-1,0},{0,-1}};
  pair<int,int> wall;
  pair<int,int> cellB;

  // Picks a random index
  int randInd = rand() % directions.size();
  // Sets the wall to be the cell plus the direction
  wall = {cell.first + directions[randInd].first, cell.second + directions[randInd].second};

  // If the coordinates are out of bounds try again
  if(wall.first < 0 || wall.first >= maze.size() || wall.second < 0 || wall.second >= maze.size()) return randomDir(maze, cell);

  // Sets the cell b to be the wall plus the direction
  cellB = {wall.first + directions[randInd].first, wall.second + directions[randInd].second};

  // If the coordinates are out of bounds, try again
  if(cellB.first < 0 || cellB.first >= maze.size() || cellB.second < 0 || cellB.first >= maze.size()) return randomDir(maze, cell);

  // Return the two cells
  output.push_back(wall);
  output.push_back(cellB);
  return output;
}

// Function to union two distinct sets
void reGroup(vector<vector<int>>& maze, vector<vector<int>>& num, pair<int,int> cell, pair<int,int> wall, pair<int,int> cellB) {
  int newGroup = 0;
  int oldGroup = 0;

  // If cell a has a higher number, set it to be the new group
  if(num[cell.first][cell.second] > num[cellB.first][cellB.second]) {
    newGroup = num[cell.first][cell.second];
    oldGroup = num[cellB.first][cellB.second];
    // If cell b has a higher number, set it to be the new group
  } else if(num[cellB.first][cellB.second] > num[cell.first][cell.second]){
    newGroup = num[cellB.first][cellB.second];
    oldGroup = num[cell.first][cell.second];
    // If cell a is equal to cell b
  } else {
    // Dont do anything and return
    return;
  }

  // Loops through all of the sets
  for(int i = 0; i < num.size(); i++) {
    for(int j = 0; j < num[0].size(); j++) {
      // If the current cell is not a wall and is equal to the old group, change it to the new group
      if(num[i][j] == oldGroup && num[i][j] != -1) num[i][j] = newGroup;
    }
  }
  
  // Set the wall to the new group
  num[wall.first][wall.second] = newGroup;
  // Open the wall in the maze
  maze[wall.first][wall.second] = 0;
}

// Function to loop through all sets and make sure they are equivalent
bool isPerfectMaze(vector<vector<int>> num, int count) {
  // Loops through all groups
  for(int i = 0; i < num.size(); i++) {
    for(int j = 0; j < num.size(); j++) {
      // If it isnt a wall or not equal to the last number it is not a perfect maze
      if(num[i][j] != -1 && num[i][j] != count) return false;
    }
  }
  return true;
}

// Function to check if cell b is a wall or not
bool isConnected(vector<vector<int>> maze, pair<int,int> cellB) {
  // If cell b isnt a wall then the wall connects two cells
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

    // If not a wall try again
    if(maze[wall.first][wall.second] == 0) return krusMaze(maze, num, count);

    // If the wall connects two cells
    if(isConnected(maze, cellB)) {
      // Union one of the two sets
      reGroup(maze, num, cell, wall, cellB);
    }

    // Run the function again
    krusMaze(maze, num, count);
  } else { // If maze is perfect return
    return;
  }
}

// Function to set the goal inside of the maze
void setGoal(vector<vector<int>>& maze) {
  bool isSet = false;
  while(!isSet) {
    // Random x
    int x = rand() % maze.size();
    // Random y
    int y = rand() % maze[0].size();

    // If the coords is not a wall
    if(maze[x][y] != 1) {
      // Set it to be the goal
      maze[x][y] = 2;
      isSet = true;
    }
  }
}

// Function to determine PacMans starting position
pair<int,int> setPacMan(vector<vector<int>> maze) {
  bool isSet = false;
  while(!isSet) {
    // Random x
    int x = rand() % maze.size();
    // Random y
    int y = rand() % maze[0].size();

    // If the coords are not a wall or the goal
    if(maze[x][y] != 1 && maze[x][y] != 2) {
      isSet = true;
      // Return the coordinates
      return {x,y};
    }
  }
  return {};
}

void krusMazeGenerator(vector<vector<int>>& maze) {
  // Random seed generator to randomize the direction
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();
  srand(seed);
  
  // Vector to keep track of the different sets
  vector<vector<int>> num(maze.size(), vector<int>(maze[0].size(), -1));
  // First set will be 0
  int count = -1;
  
  // Loops through the entire maze
  for(int i = 0; i < maze.size(); i+=2) {
    for(int j = 0; j < maze[0].size(); j+=2) {
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

  // Function to create the maze recursively
  krusMaze(maze, num, count);

  // Function to set the goal in the maze
  setGoal(maze);
  // Function to find the starting position for pacman
  pair<int,int> starting = setPacMan(maze);
  // Outputs to txt file
  freopen("output.txt","w",stdout);
  cout << "Pacman's starting position: X : " << starting.first << "   Y: " << starting.second << "\n\n";
  fclose(stdout);
}

int main() {
  // N MUST BE ODD FOR THIS ALGO TO WORK SINCE EDGES ARE REPRESENTED BY THE WALLS
  int n = 11;
  // Vector to hold the maze
  vector<vector<int>> maze(n, vector<int>(n,1));

  krusMazeGenerator(maze);

  // Print maze
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

  // Close program
  return 0;
}