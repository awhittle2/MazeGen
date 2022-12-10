#include "vector"
#include <algorithm>
#include <iostream>
#include <utility>
#include <queue>
#include <climits>
#include <set>
#include <bits/stdc++.h>

using namespace std;

struct Node {
  // x and y coordinates of the node
  int x, y;
  
  // distance from the starting node
  int distF;
  
  // distance to the target node
  int distT;
  
  // pointer to the previous node in the path
  Node* prev;
};

struct Compare {
  // operator to compare two nodes based on their total distance
  bool operator()(const Node* a, const Node* b) {
    return a->distF + a->distT > b->distF + b->distT;
  }
};

struct hash_pair {
  // operator to hash a pair of values
  template<class T1, class T2>
  size_t operator()(const pair<T1, T2>& p) const {
    auto hash1 = hash<T1>{}(p.first);
    auto hash2 = hash<T2>{}(p.second);
  
    // combine the hashes of the two values using XOR
    if(hash1 != hash2) {
      return hash1 ^ hash2;
    }
    return hash1;
  }
};

// Returns the neighbors of the given node
vector<Node*> getNeighbors(vector<vector<int>> maze, unordered_map<pair<int,int>, Node*, hash_pair> m, Node* node) {
  vector<Node*> neighbors;

  // Check if the neighbor to the right is valid and not already visited
  if((node->x + 1) >= 0 && (node->x + 1 < maze.size()) && maze[node->x+1][node->y] != 1 && m.find({node->x+1, node->y}) == m.end()) {
    Node* newN = new Node();
    newN->x = node->x + 1;
    newN->y = node->y;
    neighbors.push_back(newN);
  }

  // Check if the neighbor to the left is valid and not already visited
  if(node->x - 1 >= 0 && node->x - 1 < maze.size() && maze[node->x-1][node->y] != 1 && m.find({node->x-1, node->y}) == m.end()) {
    Node* newN = new Node();
    newN->x = node->x - 1;
    newN->y = node->y;
    neighbors.push_back(newN);
  }

  // Check if the neighbor above is valid and not already visited
  if(node->y + 1 >= 0 && node->y + 1 < maze.size() && maze[node->x][node->y+1] != 1 && m.find({node->x, node->y+1}) == m.end()) {
    Node* newN = new Node();
    newN->x = node->x;
    newN->y = node->y + 1;
    neighbors.push_back(newN);
  }

  // Check if the neighbor below is valid and not already visited
  if(node->y - 1 >= 0 && node->y - 1 < maze.size() && maze[node->x][node->y-1] != 1 && m.find({node->x, node->y-1}) == m.end()) {
    Node* newN = new Node();
    newN->x = node->x;
    newN->y = node->y - 1;
    neighbors.push_back(newN);
  }

  return neighbors;
}

// Returns the Euclidean distance between two nodes
int getDistance(Node *a, Node *b) {
  // Calculate the x and y distances between the two nodes
  float distX = abs(a->x - b->x);
  float distY = abs(a->y - b->y);

  // Square the distances
  distX = pow(distX, 2);
  distY = pow(distY, 2);

  // Take the square root of the sum of the squares to get the Euclidean distance
  float result = distX + distY;
  result = sqrt(result);
  return result;
}

// Recursively follow the path of the node back to the starting node
void recurse(Node* node, stack<Node*>& s, int startX, int startY) {
  // If the current node has a previous node in the path, continue recursion
  if(node->prev) {
    s.push(node);
    recurse(node->prev, s, startX, startY);
  }
  // Otherwise, the starting node has been reached
  else {
    // Create a new node representing the starting node
    Node *temp = new Node();
    temp->x = startX;
    temp->y = startY;
    s.push(temp);

    // Open the output file and print the path in the specified format
    freopen("output.txt","w",stdout);

    cout << "{";
    while(!s.empty()) {
      cout << "{" << s.top()->x << "," << s.top()->y << "}" << (s.size() - 1 == 0 ? "" : ",");
      s.pop();
    }
    cout << "}";
    fclose(stdout);

    // End the function
    return;
  }
}

// Solves the maze using the A* search algorithm
void aStarMazeSolver(vector<vector<int>> maze, int startX, int startY) {
  // Create the starting node
  Node* source = new Node();
  source->x = startX;
  source->y = startY;
  source->distF = 0;

  // Create the goal node
  Node* goal = new Node();

  // Find the location of the goal node in the maze
  for(int i = 0; i < maze.size(); i++) {
    for(int j = 0; j < maze[0].size(); j++) {
      if(maze[i][j] == 2) {
        goal->x = i;
        goal->y = j;
        goal->distT = 0;
        goal->distF = getDistance(goal, source);
      }
    }
  }

  // Calculate the distance from the starting node to the goal node
  source->distT = getDistance(source, goal);

  // Create a priority queue and a hash map to store the nodes
  priority_queue<Node*, vector<Node*>, Compare> q;
  unordered_map<pair<int,int>, Node*, hash_pair> m;

  // Add the starting node to the queue
  q.push(source);

  // Flag to keep track of when the goal node has been found
  bool found = false;
  // Continuously search for the goal node
  while(!found) {
    // Get the top node from the queue
    Node* node = q.top();
    q.pop();

    // Store the coordinates and distances of the node
    int x = node->x;
    int y = node->y;
    int distanceT = node->distT;
    int distanceF = node->distF;

    // Add the node to the hash map
    m[{x,y}] = node;

    // If the current node is the goal node, set the found flag to true
    if(maze[x][y] == 2) {
      found = true;
    }

    // Get the neighbors of the current node
    vector<Node*> neighbors = getNeighbors(maze,m,node);

    // Loop through the neighbors and update their distances and previous nodes
    for(int i = 0; i < neighbors.size(); i++) {
      Node* current = neighbors[i];
      current->distT = getDistance(current,goal);
      current->distF = node->distF + 1;
      current->prev = node;

      // Add the updated node to the queue
      q.push(current);
    }
  }

  // Loop through the hash map to find the goal node
  for(auto it : m) {
    // If the current node is the goal node, print the path to the output file
    if(maze[it.first.first][it.first.second] == 2) {
      stack<Node*> s;
      recurse(it.second, s, startX, startY);
    }  
  }
}

int main() {
  // Create the maze as a 2D vector
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

  // Store the starting position
  int startY = 0, startX = 7;

  // Solve the maze using the A* search algorithm
  aStarMazeSolver(maze, startY, startX);

  return 0;
}
