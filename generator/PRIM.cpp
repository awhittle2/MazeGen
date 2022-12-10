#include "vector"
#include <algorithm>
#include <iostream>
#include <utility>
#include <cmath>
#include <random>
#include <chrono>
#include <set>
#include <queue>

using namespace std;

  // This function returns a random index from the given list of pairs
int randomIndex(vector<pair<int,int>> list) {
  int randomId = rand() % list.size();
  return randomId;
}

// This function returns a list of neighboring cells to the current cell in the maze
vector<pair<int,int>> getNeighbors(vector<vector<int>> maze, pair<int,int> current) {
  vector<pair<int,int>> neighbors;

  // Check if the cell to the right of the current cell is within the bounds of the maze and add it to the list of neighbors if it is
  if((current.first + 1) >= 0 && (current.first + 1 < maze.size())) {
    neighbors.push_back({current.first+1,current.second});
  }

  // Check if the cell to the left of the current cell is within the bounds of the maze and add it to the list of neighbors if it is
  if((current.first - 1) >= 0 && (current.first - 1 < maze.size())) {
    neighbors.push_back({current.first-1,current.second});
  }

  // Check if the cell above the current cell is within the bounds of the maze and add it to the list of neighbors if it is
  if((current.second + 1) >= 0 && (current.second + 1 < maze.size())) {
    neighbors.push_back({current.first,current.second+1});
  }

  // Check if the cell below the current cell is within the bounds of the maze and add it to the list of neighbors if it is
  if((current.second - 1) >= 0 && (current.second - 1 < maze.size())) {
    neighbors.push_back({current.first,current.second-1});
  }

  return neighbors;
}

// This function returns the number of neighbors of a given cell that have already been visited
int visitedNeighbors(set<pair<int,int>> s, vector<pair<int,int>> neighbors) {
  int count = 0;

  // Loop through the list of neighbors
  for(int i = 0; i < neighbors.size(); i++) {
    // If the current neighbor is present in the set of visited cells, increment the count
    if(s.find(neighbors[i]) != s.end()) count++;
  }
  
  return count;
}

// This function generates a random maze using the Prim's algorithm
void primMaze(vector<vector<int>>& maze) {
  vector<pair<int,int>> list;  // This is the list of cells that we have not visited yet
  set<pair<int,int>> s;  // This is the set of cells that we have already visited
  pair<int,int> start = {maze.size()/2, maze[0].size()/2};  // This is the starting point for the maze generation

  // Add the starting point to the list of unvisited cells
  list.push_back(start);

  // Get the neighbors of the starting point
  vector<pair<int,int>> neighbors = getNeighbors(maze, start);

  // Add all the neighbors of the starting point to the list of unvisited cells
  for(int i = 0; i < neighbors.size(); i++) {
    list.push_back(neighbors[i]);
  }

  // Repeat the following until there are no more unvisited cells
  while(list.size() != 0) {
    // Get a random index from the list of unvisited cells
    int index = randomIndex(list);

    // Get the cell at the random index
    pair<int,int> current = list[index];

    // Add the current cell to the set of visited cells
    s.insert(current);

    // Get the neighbors of the current cell
    vector<pair<int,int>> neighbors = getNeighbors(maze, current);

    // Get the number of neighbors of the current cell that have already been visited
    int count = visitedNeighbors(s, neighbors);

    // If more than one neighbor of the current cell has already been visited, we do not add any new walls to the maze
    if(count > 1) {
    } else {
      // If only one or zero neighbors of the current cell have already been visited, we add a wall between the current cell and the unvisited neighbors
      maze[current.first][current.second] = 0;
      for(int i = 0; i < neighbors.size(); i++) {
        list.push_back(neighbors[i]);
      }
    }

    // Remove the current cell from the list of unvisited cells
    list.erase(list.begin() + index);
  }
}

// This function randomly sets the goal position in the maze
void setGoal(vector<vector<int>>& maze) {
  bool isSet = false;
  // Repeat the following until the goal position has been successfully set
  while(!isSet) {
    // Generate random coordinates for the goal position
    int x = rand() % maze.size();
    int y = rand() % maze[0].size();

    // Check if the cell at the random coordinates is not a wall
    if(maze[x][y] != 1) {
      // Set the value of the cell at the random coordinates to 2 to indicate that it is the goal position
      maze[x][y] = 2;
      // Set the flag to indicate that the goal position has been successfully set
      isSet = true;
    }
  }
}

// This function randomly sets the initial position of Pac-Man in the maze
pair<int,int> setPacMan(vector<vector<int>> maze) {
  bool isSet = false;
  // Repeat the following until the initial position of Pac-Man has been successfully set
  while(!isSet) {
    // Generate random coordinates for the initial position of Pac-Man
    int x = rand() % maze.size();
    int y = rand() % maze[0].size();

    // Check if the cell at the random coordinates is not a wall and not the goal position
    if(maze[x][y] != 1 && maze[x][y] != 2) {
      // Set the flag to indicate that the initial position of Pac-Man has been successfully set
      isSet = true;
      // Return the coordinates of the initial position of Pac-Man
      return {x,y};
    }
  }
  // Return an empty pair if the initial position of Pac-Man could not be set
  return {};
}

// This function generates a random maze using the Prim's algorithm
void primMazeGenerator(vector<vector<int>>& maze) {
  // Set the random seed based on the current time
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();
  srand(seed);

  // Generate the maze
  primMaze(maze);
}

int main() {
  // Set the size of the maze
  int n = 11;

  // Initialize the maze with walls
  vector<vector<int>> maze(n, vector<int>(n,1));

  // Generate a random maze using the Prim's algorithm
  primMazeGenerator(maze);

  // Open the output file
  freopen("output.txt","w",stdout);

  // Set the goal position in the maze
  setGoal(maze);

  // Set the initial position of Pac-Man in the maze
  pair<int,int> starting = setPacMan(maze);

  // Print the initial position of Pac-Man
  cout << "Pacman's starting position: X : " << starting.first << "   Y: " << starting.second << "\n\n";

  // Print the maze to the output file
  cout << "{";
  for(int i = 0; i < maze.size(); i++) {
    cout << "{";
    for(int j = 0; j < maze.size(); j++) {
      cout << maze[i][j] << (j == maze[0].size() - 1 ? "" : ",");
    }
    cout << "}" << (i == maze.size() - 1 ? "" : ",") << "\n";
  }
  cout << "}";

  // Close the output file
  fclose(stdout);

  return 0;
}
