#include "vector"
#include <algorithm>
#include <iostream>
#include <utility>
#include <queue>
#include <climits>
#include <set>
#include <bits/stdc++.h>

using namespace std;

// This is a struct representing a node in a graph.
struct Node {
  // The x and y coordinates of the node.
  int x, y;

  // The distance of the node from the source.
  int dist;

  // A pointer to the previous node in the shortest path from the source.
  Node* prev;
};

// This is a struct used to compare nodes based on their distance from the source.
struct Compare {
  // This is the operator overload for the "greater than" operator.
  // It returns true if the distance of node "a" is greater than the distance of node "b".
  bool operator()(const Node* a, const Node* b) {
    return a->dist > b->dist;
  }
};

// This is a struct used to compute a hash value for a pair of values.
struct hash_pair {
  // This is a template function that takes two types and a pair of values of those types.
  // It returns the hash value of the pair by combining the hash values of its two elements.
  template<class T1, class T2>
  size_t operator()(const pair<T1, T2>& p) const {
    // Compute the hash values of the first and second elements of the pair.
    auto hash1 = hash<T1>{}(p.first);
    auto hash2 = hash<T2>{}(p.second);

    // If the hash values are not equal, return their XOR.
    // Otherwise, return the hash value of the first element.
    if(hash1 != hash2) {
      return hash1 ^ hash2;
    }
    return hash1;
  }
};

// This function recursively traverses the shortest path from the source to the destination
// and stores the path in a stack.
void recurse(Node* node, stack<Node*>& s, int startX, int startY) {
  // If the current node has a previous node,
  // we continue recursively traversing the path.
  if(node->prev) {
    // Push the current node onto the stack.
    s.push(node);

    // Recursively call this function on the previous node.
    recurse(node->prev, s, startX, startY);
  }
  // If the current node doesn't have a previous node,
  // we have reached the source.
  else {
    // Create a new node for the source.
    Node *temp = new Node();

    // Set the coordinates of the source node.
    temp->x = startX;
    temp->y = startY;

    // Push the source node onto the stack.
    s.push(temp);

    // Open the output file in write mode.
    freopen("output.txt","w",stdout);

    // Print the start of the array.
    cout << "{";

    // While the stack is not empty,
    // pop the nodes from the stack and print their coordinates.
    while(!s.empty()) {
      cout << "{" << s.top()->x << "," << s.top()->y << "}" << (s.size() - 1 == 0 ? "" : ",");
      s.pop();
    }

    // Print the end of the array and close the output file.
    cout << "}";
    fclose(stdout);

    // Return from the function.
    return;
  }
}

// This function solves a maze using Dijkstra's algorithm.
// It takes a 2D vector representing the maze,
// the x and y coordinates of the starting position,
// and returns the shortest path from the starting position to the destination in the maze.
void dijMazeSolver(vector<vector<int>> maze, int startX, int startY) {
  // Create a new node for the starting position.
  Node* source = new Node();

  // Set the coordinates and distance of the source node.
  source->x = startX;
  source->y = startY;
  source->dist = 0;

  // Create a priority queue of nodes to visit, sorted by their distance from the source.
  priority_queue<Node*, vector<Node*>, Compare> q;

  // Create a map to store the nodes that have been visited.
  unordered_map<pair<int,int>, Node*, hash_pair> m;

  // Define the possible directions for moving in the maze.
  vector<pair<int,int>> directions = {{1,0},{0,1},{-1,0},{0,-1}};

  // Push the source node onto the queue.
  q.push(source);

  // Set a flag to indicate whether the destination has been found.
  bool found = false;

  // While the destination has not been found,
  // continue searching for the shortest path.
  while(!found) {
    // Get the node at the top of the queue.
    Node* node = q.top();

    // Pop the node from the queue.
    q.pop();

    // Get the coordinates and distance of the node.
    int x = node->x;
    int y = node->y;
    int distance = node->dist;

    // Add the node to the map of visited nodes.
    m[{x,y}] = node;

    // If the node is the destination, set the found flag to true.
    if(maze[x][y] == 2) {
      found = true;
    }

    // For each possible direction,
    for(int i = 0; i < directions.size(); i++) {
      // Compute the coordinates of the next position in that direction.
      int nx = x + directions[i].first;
      int ny = y + directions[i].second;

      // If the next position is within the bounds of the maze,
      // and it is not a wall and has not been visited yet,
      if(nx >= 0 && ny >= 0 && nx < maze.size() && ny < maze[0].size() && maze[nx][ny] != 1 && m.find({nx,ny}) == m.end()) {
        // Create a new node for the position.
        Node* newNode = new Node();

        // Set the coordinates, distance, and previous node of the new node.
        newNode->x = nx;
        newNode->y = ny;
        newNode->dist = distance + 1;
        newNode->prev = node;

        // Push the new node onto the queue.
        q.push(newNode);
      }
    }
  }

  // For each node in the map of visited nodes,
  for(auto it : m) {
    // If the node is the destination,
    if(maze[it.first.first][it.first.second] == 2) {
      // Create an empty stack of nodes.
      stack<Node*> s;

      // Recursively traverse the shortest path from the source to the destination,
      // storing the nodes in the stack.
      recurse(it.second, s, startX, startY);
    }  
  }
}

// This is the main function of the program.
int main() {
  // Define the maze as a 2D vector of integers.
  // 0 represents an open space, 1 represents a wall, and 2 represents the destination.
  vector<vector<int>> maze = {
        {0,0,0,0,0,0,1,1,1,0},
        {0,1,0,1,0,1,0,1,0,0},
        {1,0,0,1,0,0,0,0,0,1},
        {0,0,1,1,0,1,1,0,1,0},
        {0,1,1,0,0,1,0,0,1,0},
        {1,0,0,0,1,0,1,0,1,0},
        {0,1,0,1,0,0,0,0,0,0},
        {0,0,0,0,1,0,1,1,0,1},
        {1,0,1,0,0,1,0,0,0,0},
        {0,2,0,1,0,0,1,0,1,0}
    };

  // Define the starting position of the maze.
  int startY = 0, startX = 0;

  // Solve the maze and print the shortest path from the starting position to the destination.
  dijMazeSolver(maze, startY, startX);

  // Return 0 to indicate successful execution of the program.
  return 0;
}
