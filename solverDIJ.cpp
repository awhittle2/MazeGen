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
  int x, y;
  int dist;
  Node* prev;
};

struct Compare {
  bool operator()(const Node* a, const Node* b) {
    return a->dist > b->dist;
  }
};

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

void dijMazeSolver(vector<vector<int>> maze, int startX, int startY) {
  // Node start info
  Node* source = new Node();
  source->x = startX;
  source->y = startY;
  source->dist = 0;
  
  // Priority queue based on minimum distances of the nodes
  priority_queue<Node*, vector<Node*>, Compare> q;
  // Unordered map to hold expandend values
  unordered_map<pair<int,int>, Node*, hash_pair> m;
  // Valid directions
  vector<pair<int,int>> directions = {{1,0},{0,1},{-1,0},{0,-1}};

  q.push(source);

  // While theres still stuff to explore
  while(!q.empty()) {
    // Explore and pop the top node
    Node* node = q.top();
    q.pop();

    // Current node info
    int x = node->x;
    int y = node->y;
    int distance = node->dist;

    // Add to the expanded list
    m[{x,y}] = node;
    
    // If the node is the goal
    if(maze[x][y] == 2) {
     // Do something
    }

    // Go through each direction
    for(int i = 0; i < directions.size(); i++) {
      int nx = x + directions[i].first;
      int ny = y + directions[i].second;
  
      // If not out of bounds
      if(nx < 0 || ny < 0 || nx >= maze.size() || ny >= maze[0].size()) continue;

      // If not visited
      if(m.find({x,y}) != m.end()) continue;
      
      // If not a wall
      if(maze[nx][ny] == 0) {
        // Create a new node
        Node* newNode = new Node();
        newNode->x = nx;
        newNode->y = ny;
        // Set distance to prev + 1
        newNode->dist = distance + 1;
        // Set prev to current node
        newNode->prev = node;

        // Test to see if already in priority queue
        bool inQueue = false;
        // Holds nodes in queue incase we need to replace a node
        vector<Node*> temp;
        // Queue find if element is in queue
        priority_queue<Node*, vector<Node*>, Compare> q2;
        // q2 has all elements from q
        q2 = q;
        // Goes until q2 is empty
        while(!q2.empty()) {
          // Saves the top
          Node* top = q2.top();
          q2.pop();
          // If its the element we're looking for
          if(top->x == nx && top->y == ny) {
            inQueue = true;
          // Else add it to the vector
          } else {
            temp.push_back(top);
          }
        }

        // If after search it wasnt in queue
        if(!inQueue) {
          // Push the new node
          q.push(newNode);
          // If it was in the queue
        } else {
          // Add all elements from the vector to the queue
          for(int i = 0; i < temp.size(); i++) {
            q2.push(temp[i]);
            // Add the new updated node
            q2.push(newNode);
            // Swap the values of q and q2
            swap(q, q2);
          }
        }
      }
    }
  }
  for(auto it : m) {
        cout << "{" << it.first.first << "," << it.first.second << "}";
      }
}



int main() {
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
int startY = 0, startX = 0;

  dijMazeSolver(maze, startY, startX);
}
