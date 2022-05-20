#include "vector"
#include <algorithm>
#include <iostream>
#include <utility>

using namespace std;

void dfs(vector<vector<int>> maze, vector<pair<int, int>> path,
         vector<vector<bool>> visited, int x, int y) {
  if (x < 0 || y < 0 || x >= maze.size() || y >= maze[0].size() ||
      maze[x][y] == 1 || visited[x][y])
    return; // If out of bounds, wall, or visited

  pair<int, int> current = {x, y};
  path.emplace_back(current); // Add to current path
  visited[x][y] = true;       // Add to visited

  if (maze[x][y] == 2) { // If goal
    cout << "{";
    for (int i = 0; i < path.size(); i++) {
      cout << "{" << path[i].first << "," << path[i].second << "}"
           << (i == path.size() - 1 ? "" : ",");
    }
    cout << "}";
    return;
  }

  dfs(maze, path, visited, x + 1, y); // Go right
  dfs(maze, path, visited, x, y + 1); // Go up
  dfs(maze, path, visited, x - 1, y); // Go left
  dfs(maze, path, visited, x, y - 1); // Go down
}

void helper(vector<vector<int>> maze, int startX, int startY) {
  vector<pair<int, int>> path;
  vector<vector<bool>> visited(maze.size(),
                               vector<bool>(maze[0].size(), false));

  dfs(maze, path, visited, startX, startY);
}

int main() {
  vector<vector<int>> maze = {
	{0,1,2,0,1,0,1,1,1,0},
	{0,1,1,0,1,0,0,1,0,0},
	{0,0,0,0,0,1,0,0,0,1},
	{1,1,0,1,0,0,1,0,1,0},
	{0,1,1,0,0,1,0,0,0,0},
	{0,0,0,1,0,0,0,1,0,1},
	{0,1,0,0,0,1,1,0,0,0},
	{1,0,1,0,1,0,1,1,0,1},
	{1,0,1,0,1,0,0,0,1,0},
	{0,0,0,0,0,0,1,0,0,0}
};
int startY = 9, startX = 3;

  helper(maze, startY, startX);
}