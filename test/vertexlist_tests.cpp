#include <catch2/catch.hpp>
#include "csv_routes.hpp"
#include "graph/graph.hpp"
#include "graph/views/vertexlist.hpp"
#include "graph/container/csr_graph.hpp"

using std::ranges::forward_range;
using std::remove_reference_t;
using std::is_const_v;
using std::is_lvalue_reference_v;
using std::forward_iterator;
using std::input_iterator;

using std::graph::vertex_t;
using std::graph::vertex_key_t;
using std::graph::vertex_edge_range_t;
using std::graph::edge_t;
using std::graph::edge_value_t;

using std::graph::graph_value;
using std::graph::vertices;
using std::graph::edges;
using std::graph::vertex_key;
using std::graph::vertex_value;
using std::graph::target_key;
using std::graph::target;
using std::graph::edge_value;
using std::graph::degree;
using std::graph::find_vertex;
using std::graph::find_vertex_edge;

using routes_csr_graph_type = std::graph::container::csr_graph<double, std::string, std::string>;

template <typename G>
constexpr auto find_frankfurt_key(const G& g) {
  return find_city_key(g, "Frankf\xC3\xBCrt");
}

template <typename G>
auto find_frankfurt(G&& g) {
  return find_city(g, "Frankf\xC3\xBCrt");
}

// Things to test
//  csr_graph with VV=void (does it compile?)
//  push_back and emplace_back work correctly when adding city names (applies to csr_graph & dynamic_graph)

TEST_CASE("vertexlist test", "[csr][vertexlist]") {

  init_console();

  using G  = routes_csr_graph_type;
  auto&& g = load_ordered_graph<G>(TEST_DATA_ROOT_DIR "germany_routes.csv", name_order_policy::source_order_found);
  // name_order_policy::source_order_found gives best output with least overlap for germany routes

  const auto frankfurt     = find_frankfurt(g);
  const auto frankfurt_key = find_frankfurt_key(g);

  SECTION("non-const vertexlist_iterator") {
    static_assert(!std::is_const_v<std::remove_reference_t<decltype(g)>>);
    static_assert(!std::is_const_v<G>);

    std::graph::views::vertexlist_iterator<G> i0; // default construction
    std::graph::views::vertexlist_iterator<G> i1(g);
    {
      auto&& [ukey, u] = *i1;
      static_assert(is_const_v<decltype(ukey)>, "vertex key must be const");
      static_assert(!is_const_v<remove_reference_t<decltype(u)>>, "vertex must be non-const");
      REQUIRE(ukey == 0);
    }
    {
      auto&& [ukey, u] = *++i1;
      REQUIRE(ukey == 1);
      auto i1b = i1;
      REQUIRE(i1b == i1);
    }

    std::graph::views::vertexlist_iterator<G> i2(g);
    {
      auto&& [ukey, u] = *i2;
      static_assert(is_const_v<decltype(ukey)>, "vertex key must be const");
      static_assert(is_lvalue_reference_v<decltype(u)>, "vertex must be lvalue reference");
      static_assert(!is_const_v<remove_reference_t<decltype(u)>>, "vertex must be non-const");
      REQUIRE(ukey == 0);
    }
    {
      auto&& [ukey, u] = *++i2;
      REQUIRE(ukey == 1);
      auto i2b = i2;
      REQUIRE(i2b == i2);
    }

    static_assert(std::forward_iterator<decltype(i1)>, "vertexlist_iterator must be a forward_iterator");

    using view_t = decltype(std::graph::views::vertexlist(g));
    static_assert(forward_range<view_t>, "vertexlist(g) is not returning a forward_range");
    size_t cnt = 0;
    for (auto&& [ukey, u] : std::graph::views::vertexlist(g)) {
      ++cnt;
    }
    REQUIRE(cnt == size(vertices(g)));

    //std::graph::views::vertexlist_iterator<const G> j0;
    //j0 = i0;
    //i0 == j0;
  }

  SECTION("const vertexlist_iterator") {
    using G2 = const G;
    G2& g2   = g;
    static_assert(std::is_const_v<std::remove_reference_t<decltype(g2)>>, "graph must be const");

    //std::graph::views::vertexlist_iterator<G2> i0; // default construction
    std::graph::views::vertexlist_iterator<G2> i1(g2);
    {
      auto&& [ukey, u] = *i1;

      static_assert(is_const_v<decltype(ukey)>, "key must be const");
      static_assert(is_lvalue_reference_v<decltype(u)>, "vertex must be lvalue reference");
      static_assert(is_const_v<remove_reference_t<decltype(u)>>, "vertex must be const");
      REQUIRE(ukey == 0);
    }
    {
      auto&& [ukey, u] = *++i1;
      REQUIRE(ukey == 1);
      auto i1b = i1;
      REQUIRE(i1b == i1);
    }

    std::graph::views::vertexlist_iterator<G2> i2(g2);
    {
      auto&& [ukey, u] = *i2;
      static_assert(is_const_v<decltype(ukey)>, "key must be const");
      static_assert(is_const_v<remove_reference_t<decltype(u)>>, "vertex must be const");
      REQUIRE(ukey == 0);
    }
    {
      auto&& [ukey, u] = *++i2;
      REQUIRE(ukey == 1);
      auto i2b = i2;
      REQUIRE(i2b == i2);
    }

    static_assert(std::forward_iterator<decltype(i1)>, "vertexlist_iterator must be a forward_iterator");

    using view_t = decltype(std::graph::views::vertexlist(g2));
    static_assert(forward_range<view_t>, "vertexlist(g) is not returning a forward_range");
    size_t cnt = 0;
    for (auto&& [ukey, u] : std::graph::views::vertexlist(g2)) {
      ++cnt;
    }
    REQUIRE(cnt == size(vertices(g)));
  }
}
