#include <cstdio>
#include <vector>
#include <queue>
#include <algorithm>
#include <numeric>
#include <limits>

using namespace std;

void printVec(const vector<int>& vec) {
	printf("[%zu]: ", vec.size());
	for (int v : vec)
		printf("%d, ", v);
	printf("\n");
}

void printVec(const vector<size_t>& vec) {
	printf("[%zu]: ", vec.size());
	for (size_t v : vec)
		printf("%03zu ", v);
	printf("\n");
}

class Solution {
  vector<vector<size_t>> _dist;
  vector<vector<size_t>> _reachCount;
public:
  int shortestDistance(vector<vector<int>>& grid) {
    size_t nBldgs = std::accumulate(grid.begin(), grid.end(), size_t(0), [] (size_t s, auto& v) {
      return s + std::count(v.begin(), v.end(), 1);
    });

		printf("nBldgs: %zu\n", nBldgs);
    _dist = vector<vector<size_t>>(grid.size(), vector<size_t>(grid[0].size(), 0));
    _reachCount = vector<vector<size_t>>(grid.size(), vector<size_t>(grid[0].size(), 0));
    for (size_t r = 0; r < grid.size(); ++r) {
      const auto& col = grid[r];
      for (size_t c = 0; c < col.size(); ++c) {
        if (col[c] == 1) {
					printf("bfs: %zu - %zu\n", r, c);
          bool foundAllBldgs = bfs(grid, r, c, nBldgs);
          if (!foundAllBldgs) return -1;
        }
        if (col[c] == 1 || col[c] == 2)
          _reachCount[r][c] = nBldgs;
      }
    }

    size_t minDist = std::numeric_limits<size_t>::max();
    for (size_t r = 0; r < grid.size(); ++r) {
      const auto& col = grid[r];
      for (size_t c = 0; c < col.size(); ++c) {

        // 2nd condition means that not all buildings are reachable from that location
        if (grid[r][c] != 0 || _reachCount[r][c] != nBldgs) continue;
        if (_dist[r][c] < minDist)
          minDist = _dist[r][c];
      }
    }

    return minDist == std::numeric_limits<size_t>::max() ? -1 : minDist;
  }

  bool bfs(const vector<vector<int>>& grid, size_t r, size_t c, const size_t nBldgs) {
		size_t nBldgsFound = 1;
		const vector<pair<int, int>> dirs { {1, 0}, {0, 1}, {-1, 0}, {0, -1} };
		
		const size_t rows = grid.size(), cols = grid[0].size();
		vector<vector<bool>> visited(rows, vector<bool>(cols, false));
    queue<pair<size_t, size_t>> q;
		
    q.push({r, c});
    visited[r][c] = true;
    int distance = 0;
		bool hitZero = false;
    while (!q.empty()) {
      
      const size_t qSz = q.size();
      for (size_t i = 0; i < qSz; ++i) {
        auto val = q.front();
        q.pop();
				
				size_t r = val.first, c = val.second;
				
				// update distance
				_dist[r][c] += (grid[r][c] == 0) ? distance : 0;
				
				hitZero = hitZero || grid[r][c] == 0;
				
				// loop for each 4 directions
				for (const auto& dir : dirs) {
					if ((r == 0 && dir.first == -1)  || (r == rows - 1 && dir.first == 1) || 
							(c == 0 && dir.second == -1) || (c == cols - 1 && dir.second == 1))
						continue;
						
					size_t r1 = r + dir.first, c1 = c + dir.second;
					if (visited[r1][c1]) continue; //  || grid[r1][c1] == 2
					
					// 2nd condition is to avoid 2 adjacent buildings
					nBldgsFound += grid[r1][c1] == 1 && grid[r][c] == 0;
					
					// can't directly come from one building to another - so don't mark as visited
					if (grid[r1][c1] == 1 && grid[r][c] == 1) continue;
					
					visited[r1][c1] = true;
          ++_reachCount[r1][c1];

					if (grid[r1][c1] == 0) q.push({r1, c1});
				}
      }
			
      ++distance;
    }
		
		for (auto& v : _dist)
			printVec(v);
		
		printf("nBldgsFound: %zu\n", nBldgsFound);
    return nBldgsFound == nBldgs && hitZero;
  }
};

int main() {
	
	vector<vector<int>> vec { {1,0,2,0,1}, {0,0,0,0,0}, {0,0,1,0,0} };
	vec = { {0, 1}, {1, 2} };
	//vec = { {1, 2, 0} };
	vec = {{1,1,1,1,1,0},{0,0,0,0,0,1},{0,1,1,0,0,1},{1,0,0,1,0,1},{1,0,1,0,0,1},{1,0,0,0,0,1},{0,1,1,1,1,0}};
	printf("Original:\n");
	for (auto& v : vec)
		printVec(v);
		
	Solution s;
	int dist = s.shortestDistance(vec);
	printf("Distance: %d\n", dist);
	return 0;
}