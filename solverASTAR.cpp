#include "vector"
#include <algorithm>
#include <iostream>
#include <utility>
#include <queue>
#include <climits>
#include <set>
#include <bits/stdc++.h>

using namespace std;

// Struct to hold node information
struct Node {
  // Current coords
  int x, y;
  // Distance from start
  int distF;
  // Distance to end
  int distT;
  // Previous node
  Node* prev;
};

// Struct to customize the priority queue to be based on the distance from plus the distance to
struct Compare {
  bool operator()(const Node* a, const Node* b) {
    return a->distF + a->distT > b->distF + b->distT;
  }
};

// Struct to allow for pair keys in the unordered map
struct hash_pair {
  template<class T1, class T2>
  size_t operator()(const pair<T1, T2>& p) const {
    auto hash1 = hash<T1>{}(p.first);
    auto hash2 = hash<T2>{}(p.second);
  
    if(hash1 != hash2) {
      return hash1 ^ hash2;
    }
    return hash1;
  }
};

// Function to find and output all valid neighbors
vector<Node*> getNeighbors(vector<vector<int>> maze, unordered_map<pair<int,int>, Node*, hash_pair> m, Node* node) {
  vector<Node*> neighbors;

  // If right neighbor is within bounds, not a wall, and not visited
  if((node->x + 1) >= 0 && (node->x + 1 < maze.size()) && maze[node->x+1][node->y] != 1 && m.find({node->x+1, node->y}) == m.end()) {
    // Create a new node
    Node* newN = new Node();
    newN->x = node->x + 1;
    newN->y = node->y;
    // Push the node to the vector
    neighbors.push_back(newN);
  }

  // If the left neighbor is within bounds, not a wall, and not visited
  if(node->x - 1 >= 0 && node->x - 1 < maze.size() && maze[node->x-1][node->y] != 1 && m.find({node->x-1, node->y}) == m.end()) {
    // Create a new node
    Node* newN = new Node();
    newN->x = node->x - 1;
    newN->y = node->y;
    // Push the node to the vector
    neighbors.push_back(newN);
  }

  // If the up neighbor is within bounds, not a wall, and not visited
  if(node->y + 1 >= 0 && node->y + 1 < maze.size() && maze[node->x][node->y+1] != 1 && m.find({node->x, node->y+1}) == m.end()) {
    // Create a new node
    Node* newN = new Node();
    newN->x = node->x;
    newN->y = node->y + 1;
    // Push the node to the vector
    neighbors.push_back(newN);
  }

  // If the down neighbor is within bounds, not a wall, and not visited
  if(node->y - 1 >= 0 && node->y - 1 < maze.size() && maze[node->x][node->y-1] != 1 && m.find({node->x, node->y-1}) == m.end()) {
    // Create a new node
    Node* newN = new Node();
    newN->x = node->x;
    newN->y = node->y - 1;
    // Push the node to the vector
    neighbors.push_back(newN);
  }

  // Return all neighbors
  return neighbors;
}

// Function to get the euc distance between two nodes
int getDistance(Node *a, Node *b) {
  // Absolute value of x1 - x2
  float distX = abs(a->x - b->x);
  // Absolute value of y1 - y2
  float distY = abs(a->y - b->y);
  // Square the first term
  distX = pow(distX, 2);
  // Square the second term
  distY = pow(distY, 2);

  // Add the two terms together
  float result = distX + distY;
  // Square the result and return it
  result = sqrt(result);
  return result;
}

// Function to recursively reverse from end node to start node
void recurse(Node* node, stack<Node*>& s, int startX, int startY) {
  // If there is a previous node
  if(node->prev) {
    // Push it to the stack
    s.push(node);
    // Try again with the previous previous node
    recurse(node->prev, s, startX, startY);
    // If there is no previous node
  } else {
    // Push the starting node to the stack
    Node *temp = new Node();
    temp->x = startX;
    temp->y = startY;
    s.push(temp);

    // Outputs to txt file
    freopen("output.txt","w",stdout);

    cout << "{";
    while(!s.empty()) {
      // Output the top of the stack
      cout << "{" << s.top()->x << "," << s.top()->y << "}" << (s.size() - 1 == 0 ? "" : ",");
      // Pop it off of the stack
      s.pop();
    }
    cout << "}";
    fclose(stdout);
    return;
  }
}

// Function to set up and perform a* seach
void aStarMazeSolver(vector<vector<int>> maze, int startX, int startY) {
  // Node start info
  Node* source = new Node();
  source->x = startX;
  source->y = startY;
  source->distF = 0;

  Node* goal = new Node();

  // Search for the ending node
  for(int i = 0; i < maze.size(); i++) {
    for(int j = 0; j < maze[0].size(); j++) {
      // If goal node
      if(maze[i][j] == 2) {
        // Update the goal node info
        goal->x = i;
        goal->y = j;
        // Set the distanceTo 0
        goal->distT = 0;
        // Set the distanceFrom to be the euc distance
        goal->distF = getDistance(goal, source);
      }
    }
  }

  // Set the starting distanceTo to be the euc distance
  source->distT = getDistance(source, goal);

  // Priority queue based on minimum distances from plus the minimum distances to of the nodes
  priority_queue<Node*, vector<Node*>, Compare> q;
  // Unordered map for visited values
  unordered_map<pair<int,int>, Node*, hash_pair> m;

  // Push the starting node to the pq
  q.push(source);

  bool found = false;
  while(!found) {
    // Explore and pop the top node
    Node* node = q.top();
    q.pop();

    // Current node info
    int x = node->x;
    int y = node->y;
    int distanceT = node->distT;
    int distanceF = node->distF;

    // Add to visited
    m[{x,y}] = node;

    // If current is the goal
    if(maze[x][y] == 2) {
      // Exit out of the while loop
      found = true;
    }

    // Find all valid neighbors
    vector<Node*> neighbors = getNeighbors(maze,m,node);

    // Go through each neighbor
    for(int i = 0; i < neighbors.size(); i++) {
      // Create a new node at the index
      Node* current = neighbors[i];
      // Distance to is the euc distance
      current->distT = getDistance(current,goal);
      // Distance from is current distance + 1
      current->distF = node->distF + 1;
      // Previous node is current
      current->prev = node;

      // Push the node to the priority queue
      q.push(current);
    }
  }

  // Loop through all items in the unordered map
  for(auto it : m) {
    if(maze[it.first.first][it.first.second] == 2) {
      // Create a stack as temp storage for reversing
      stack<Node*> s;
      // Recursively reverse until starting node
      recurse(it.second, s, startX, startY);
    }  
  }
}

int main() {
  // Maze input
  vector<vector<int>> maze = {
	{1,0,1,0,0,0,0,0,0,0},
	{0,0,0,0,1,0,1,0,1,1},
	{1,0,1,0,1,0,1,0,0,0},
	{1,0,1,0,0,1,0,0,1,0},
	{0,0,1,0,1,1,1,0,0,1},
	{1,0,1,0,0,1,0,0,1,0},
	{1,1,0,0,1,0,1,0,0,0},
	{1,0,1,0,0,0,0,1,1,0},
	{0,0,0,1,0,1,0,1,2,0},
	{0,1,0,0,0,0,1,1,1,0}
  };
  // Pacman starting position
  int startY = 0, startX = 7;

  aStarMazeSolver(maze, startY, startX);

  // Program close
  return 0;
}