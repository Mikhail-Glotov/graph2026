/**
 * @file dijkstra.hpp
 * @author Glotov Mikhail
 *
 * Алгоритм Дейкстры поиска кратчайших путей от заданной вершины.
 */

#ifndef INCLUDE_DIJKSTRA_HPP_
#define INCLUDE_DIJKSTRA_HPP_

#include <vector>
#include <limits>
#include <algorithm>

namespace graph {

const int INF = std::numeric_limits<int>::max();

template<typename GraphType>
void Dijkstra(const GraphType& graph, size_t start,
              std::vector<int>& distances,
              std::vector<int>& parents) {
  const size_t n = graph.Vertices().size();
  distances.assign(n, INF);
  parents.assign(n, -1);
  std::vector<char> visited(n, false);
  distances[start] = 0;

  for (size_t i = 0; i < n; ++i) {
    int v = -1;
    for (size_t j = 0; j < n; ++j) {
      if (!visited[j] && (v == -1 || distances[j] < distances[v]))
        v = j;
    }
    if (v == -1 || distances[v] == INF) break;
    visited[v] = true;
    for (const auto& edge : graph.Edges(v)) {
      int to = edge.To();
      int weight = edge.Weight();
      if (distances[v] + weight < distances[to]) {
        distances[to] = distances[v] + weight;
        parents[to] = v;
      }
    }
  }
}

inline std::vector<size_t> RestorePath(const std::vector<int>& parents,
                                        size_t start, size_t target) {
  std::vector<size_t> path;
  if (start == target) return {start};
  for (int v = target; v != -1 && static_cast<size_t>(v) != start;
       v = parents[v]) {
    path.push_back(static_cast<size_t>(v));
  }
  if (path.empty() || parents[target] == -1) return {};
  path.push_back(start);
  std::reverse(path.begin(), path.end());
  return path;
}

}  // namespace graph

#endif  // INCLUDE_DIJKSTRA_HPP_
