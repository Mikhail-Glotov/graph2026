/**
 * @file dijkstra_test.cpp
 * @author Glotov Mikhail
 *
 * Тесты для алгоритма Дейкстры.
 */

#include <httplib.h>
#include <vector>
#include <random>
#include <nlohmann/json.hpp>
#include "test_core.hpp"

static void SimpleTest(httplib::Client* cli);
static void RandomTest(httplib::Client* cli);

void TestDijkstra(httplib::Client* cli) {
  TestSuite suite("TestDijkstra");
  RUN_TEST_REMOTE(suite, cli, SimpleTest);
  RUN_TEST_REMOTE(suite, cli, RandomTest);
}

static void SimpleTest(httplib::Client* cli) {
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

static void RandomTest(httplib::Client* cli) {
  const int numTries = 10;
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<size_t> vertexCountDist(2, 10);
  std::uniform_int_distribution<size_t> edgeCountDist(1, 20);
  std::uniform_int_distribution<int> weightDist(1, 50);

  for (int t = 0; t < numTries; ++t) {
    size_t n = vertexCountDist(gen);
    size_t m = edgeCountDist(gen);

    nlohmann::json input;
    input["graph_type"] = "WeightedGraph";
    input["weight_type"] = "int";
    input["start"] = 0;

    for (size_t i = 0; i < n; ++i)
      input["vertices"].push_back(i);

    for (size_t i = 0; i < m; ++i) {
      std::uniform_int_distribution<size_t> vertexDist(0, n - 1);
      size_t u = vertexDist(gen);
      size_t v = vertexDist(gen);
      if (u != v) {
        input["edges"][i]["from"] = u;
        input["edges"][i]["to"] = v;
        input["edges"][i]["weight"] = weightDist(gen);
      }
    }

    auto res = cli->Post("/Dijkstra", input.dump(), "application/json");
    REQUIRE(res);
    nlohmann::json output = nlohmann::json::parse(res->body);
    REQUIRE_EQUAL(output["result"].size(), n);
    REQUIRE_EQUAL(output["result"][0]["distance"], 0);
  }
}
