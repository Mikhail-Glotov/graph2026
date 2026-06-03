/**
 * @file dijkstra_method.cpp
 * @author Glotov Mikhail
 *
 * Серверная часть алгоритма Дейкстры.
 */

#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <weighted_graph.hpp>
#include <weighted_oriented_graph.hpp>
#include <dijkstra.hpp>

namespace graph {

int DijkstraMethod(const nlohmann::json& input, nlohmann::json* output) {
  std::string graphType = input.at("graph_type");

  if (graphType == "WeightedGraph") {
    std::string weightType = input.at("weight_type");
    if (weightType == "int") {
      WeightedGraph<int> graph;
      for (const auto& vertex : input.at("vertices"))
        graph.AddVertex(vertex);
      for (const auto& edge : input.at("edges"))
        graph.AddEdge(edge.at("from"), edge.at("to"), edge.at("weight"));
      size_t start = input.at("start");
      std::vector<int> distances, parents;
      Dijkstra(graph, start, distances, parents);
      nlohmann::json result;
      for (size_t i = 0; i < distances.size(); ++i) {
        if (distances[i] == INF) {
          result[i]["distance"] = -1;
          result[i]["path"] = nlohmann::json::array();
        } else {
          result[i]["distance"] = distances[i];
          result[i]["path"] = RestorePath(parents, start, i);
        }
      }
      (*output)["result"] = result;
      return 0;
    }
  } else if (graphType == "WeightedOrientedGraph") {
    std::string weightType = input.at("weight_type");
    if (weightType == "int") {
      WeightedOrientedGraph<int> graph;
      for (const auto& vertex : input.at("vertices"))
        graph.AddVertex(vertex);
      for (const auto& edge : input.at("edges"))
        graph.AddEdge(edge.at("from"), edge.at("to"), edge.at("weight"));
      size_t start = input.at("start");
      std::vector<int> distances, parents;
      Dijkstra(graph, start, distances, parents);
      nlohmann::json result;
      for (size_t i = 0; i < distances.size(); ++i) {
        if (distances[i] == INF) {
          result[i]["distance"] = -1;
          result[i]["path"] = nlohmann::json::array();
        } else {
          result[i]["distance"] = distances[i];
          result[i]["path"] = RestorePath(parents, start, i);
        }
      }
      (*output)["result"] = result;
      return 0;
    }
  }

  return -1;
}

}  // namespace graph
