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

int randomIndex(vector<pair<int,int>> list) {
  int randomId = rand() % list.size();
  return randomId;
}

// Function to find and output all valid neighbors
vector<pair<int,int>> getNeighbors(vector<vector<int>> maze, pair<int,int> current) {
  vector<pair<int,int>> neighbors;

  // If right neighbor is within bounds
  if((current.first + 1) >= 0 && (current.first + 1 < maze.size())) {
    // Push the neighbor to the vector
    neighbors.push_back({current.first+1,current.second});
  }

  // If left neighbor is within bounds
  if((current.first - 1) >= 0 && (current.first - 1 < maze.size())) {
    // Push the neighbor to the vector
    neighbors.push_back({current.first-1,current.second});
  }

  // If up neighbor is within bounds
  if((current.second + 1) >= 0 && (current.second + 1 < maze.size())) {
    // Push the neighbor to the vector
    neighbors.push_back({current.first,current.second+1});
  }

  // If down neighbor is within bounds
  if((current.second - 1) >= 0 && (current.second - 1 < maze.size())) {
    // Push the neighbor to the vector
    neighbors.push_back({current.first,current.second-1});
  }

  // Return all neighbors
  return neighbors;
}

int visitedNeighbors(set<pair<int,int>> s, vector<pair<int,int>> neighbors) {
  int count = 0;

  for(int i = 0; i < neighbors.size(); i++) {
    if(s.find(neighbors[i]) != s.end()) count++;
  }
  
  return count;
}

void primMaze(vector<vector<int>>& maze) {
  // Vector to hold list of walls
  vector<pair<int,int>> list;
  // Set to hold visited
  set<pair<int,int>> s;
  // Pair to hold starting position
  pair<int,int> start = {maze.size()/2, maze[0].size()/2};

  // Push the starting position to the vector
  list.push_back(start);

  // Find the neighbors of the starting node
  vector<pair<int,int>> neighbors = getNeighbors(maze, start);

  // Loop through all neighbors
  for(int i = 0; i < neighbors.size(); i++) {
    // Push each neighbor to the list of walls
    list.push_back(neighbors[i]);
  }

  // While the vector is not empty
  while(list.size() != 0) {
    // Find a random index in the list
    int index = randomIndex(list);
    pair<int,int> current = list[index];

    // Add to visited
    s.insert(current);

    // Find all valid neighbors and its count
    vector<pair<int,int>> neighbors = getNeighbors(maze, current);
    int count = visitedNeighbors(s, neighbors);

    if(count > 1) {
      // Do nothing
    } else {
      // Remove the wall
      maze[current.first][current.second] = 0;
      for(int i = 0; i < neighbors.size(); i++) {
        // Add each neighbor to the list
        list.push_back(neighbors[i]);
      }
    }

    // Remove from vector
    list.erase(list.begin() + index);
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

void primMazeGenerator(vector<vector<int>>& maze) {
  // Random seed generator
  unsigned seed = chrono::system_clock::now().time_since_epoch().count();
  srand(seed);

  primMaze(maze);
}

int main() {
  // Size of maze (nxn)
  int n = 11;
  // Vector to hold the maze
  vector<vector<int>> maze(n, vector<int>(n,1));

  primMazeGenerator(maze);

  // Outputs the entire maze
  // Outputs to txt file
  freopen("output.txt","w",stdout);
  // Sets the goal and adds it to the maze
  setGoal(maze);
  // Finds the starting position
  pair<int,int> starting = setPacMan(maze);
  cout << "Pacman's starting position: X : " << starting.first << "   Y: " << starting.second << "\n\n";
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