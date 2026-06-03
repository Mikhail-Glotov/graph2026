/**
 * @file tests/dijkstra_test.cpp
 * @author Glotov Mikhail
 *
 * Тесты для алгоритма Дейкстры.
 */

#include <httplib.h>
#include <vector>
#include <nlohmann/json.hpp>
#include "test_core.hpp"

void TestDijkstra(httplib::Client* cli) {
  nlohmann::json input;
  input["graph_type"] = "WeightedGraph";
  input["weight_type"] = "int";
  input["vertices"] = {0, 1, 2, 3};
  input["start"] = 0;
  input["edges"][0]["from"] = 0;
  input["edges"][0]["to"] = 1;
  input["edges"][0]["weight"] = 10;
  input["edges"][1]["from"] = 0;
  input["edges"][1]["to"] = 2;
  input["edges"][1]["weight"] = 3;
  input["edges"][2]["from"] = 2;
  input["edges"][2]["to"] = 1;
  input["edges"][2]["weight"] = 1;
  input["edges"][3]["from"] = 1;
  input["edges"][3]["to"] = 3;
  input["edges"][3]["weight"] = 2;

  auto res = cli->Post("/Dijkstra", input.dump(), "application/json");
  REQUIRE(res);
  nlohmann::json output = nlohmann::json::parse(res->body);
  REQUIRE_EQUAL(output["result"][0]["distance"], 0);
  REQUIRE_EQUAL(output["result"][1]["distance"], 4);
  REQUIRE_EQUAL(output["result"][2]["distance"], 3);
  REQUIRE_EQUAL(output["result"][3]["distance"], 6);
}
