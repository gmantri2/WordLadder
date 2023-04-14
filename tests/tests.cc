#  ifndef CATCH_CONFIG_MAIN
#  define CATCH_CONFIG_MAIN
#endif

#include "catch.hpp"
#include "word_graph.hpp"

/**************************************************
 * creating adj list
 **************************************************/
TEST_CASE("isEdge", "[is-edge]") {
  WordGraph wg;
  //wg.readFromFile("graph.txt");
  REQUIRE(wg.IsEdge("town","toon") == true);
  REQUIRE(wg.IsEdge("town","ton") == true);
  REQUIRE(wg.IsEdge("town","towns") == true);
  REQUIRE(wg.IsEdge("town","gown") == true);
  REQUIRE(wg.IsEdge("tower","power") == true);
  REQUIRE(wg.IsEdge("power","powder") == true);
  REQUIRE(wg.IsEdge("tower","powders") == false);
  REQUIRE(wg.IsEdge("town","town") == false);
  REQUIRE(wg.IsEdge("a","") == true);
}
TEST_CASE("fill adj list", "[fill-adj]") {
  WordGraph wg;
  wg.ReadFromFile(wg.GetGraphFile());
  REQUIRE(wg.GetAdj().find("apple")->second == std::vector<std::string>{"ample", "apples", "apply"});
  REQUIRE(wg.GetAdj().find("needed")->second == std::vector<std::string>{"seeded"});
  REQUIRE(wg.GetAdj().find("shook")->second == std::vector<std::string>{"hook", "shock", "shoot"});
}