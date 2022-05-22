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

void recurse(Node* node, stack<Node*> s, int startX, int startY) {
  if(node->prev) {
    s.push(node);
    recurse(node->prev, s, startX, startY);
  } else {
    Node *temp = new Node();
    temp->x = startX;
    temp->y = startY;
    s.push(temp);

    while(!s.empty()) {
      cout << "{" << s.top()->x << "," << s.top()->y << "}" << (s.size() - 1 == 0 ? "" : ",");
      s.pop();
    }
  }
}

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

  bool found = false;
  // While theres still stuff to explore
  while(!found) {
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
     found = true;
    }

    // Go through each direction
    for(int i = 0; i < directions.size(); i++) {
      int nx = x + directions[i].first;
      int ny = y + directions[i].second;
  
      // If not out of bounds or not wall
      if(nx >= 0 && ny >= 0 && nx < maze.size() && ny < maze[0].size() && maze[nx][ny] != 1 && m.find({nx,ny}) == m.end()) {
    // Create a new node
    Node* newNode = new Node();
    newNode->x = nx;
    newNode->y = ny;
    // Set distance to prev + 1
    newNode->dist = distance + 1;
    // Set prev to current node
    newNode->prev = node;

    q.push(newNode);
    }
  }
}
  for(auto it : m) {
    if(maze[it.first.first][it.first.second] == 2) {
      stack<Node*> s;
      recurse(it.second, s, startX, startY);
    }  
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
  return 0;
}
