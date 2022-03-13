#include <catch2/catch.hpp>
#include "csv_routes.hpp"
#include "graph/graph.hpp"
#include "graph/views/vertexlist.hpp"
#include "graph/container/csr_graph.hpp"
#include <concepts>

using std::ranges::forward_range;
using std::remove_reference_t;
using std::is_const_v;
using std::is_lvalue_reference_v;
using std::forward_iterator;
using std::input_iterator;

using std::graph::vertex_t;
using std::graph::vertex_key_t;
using std::graph::vertex_value_t;
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
    static_assert(std::forward_iterator<decltype(i1)>, "vertexlist_iterator must be a forward_iterator");
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

    static_assert(std::input_or_output_iterator<decltype(i1)>);
    using _It = std::graph::views::vertexlist_iterator<G>;
    using _Se = std::graph::vertex_iterator_t<G>;
    bool yy   = std::sentinel_for<_Se, _It>;
    bool xx   = std::sized_sentinel_for<_Se, _It>;
    static_assert(std::sized_sentinel_for<_Se, _It> == false);
    auto _Ki =
          std::sized_sentinel_for<_Se, _It> ? std::ranges::subrange_kind::sized : std::ranges::subrange_kind::unsized;

    auto vvf  = [&g](vertex_t<G>& u) -> std::string& { return vertex_value(g, u); };
    using VVF = decltype(vvf);
    std::graph::views::vertexlist_iterator<G, VVF> i3(g, vvf, begin(vertices(g)));
    {
      // The following asserts are used to isolate problem with failing input_or_output_iterator concept for vertexlist_iterator
      static_assert(std::movable<decltype(i3)>, "vertexlist_iterator<G,VVF> is NOT movable");
      static_assert(std::default_initializable<decltype(i3)>,
                    "vertexlist_iterator<G,VVF> is NOT default_initializable");
      //static_assert(std::__detail::__is_signed_integer_like<std::iter_difference_t<decltype(i3)>>, "vertexlist_iterator<G,VVF> is NOT __is_signed_integer_like");
      static_assert(std::weakly_incrementable<decltype(i3)>, "vertexlist_iterator<G,VVF> is NOT weakly_incrementable");
      static_assert(std::input_or_output_iterator<decltype(i3)>,
                    "vertexlist_iterator<G,VVF> is NOT an input_or_output_iterator");

      auto&& [ukey, u, name] = *i3;
      REQUIRE(ukey == 0);
      REQUIRE(name == "Frankf\xC3\xBCrt");
    }
    {
      auto&& [ukey, u, name] = *++i3;
      REQUIRE(ukey == 1);
      REQUIRE(name == "Mannheim");
    }

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
    static_assert(std::forward_iterator<decltype(i1)>, "vertexlist_iterator must be a forward_iterator");
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

    auto vvf  = [&g](vertex_t<G>& u) -> std::string& { return vertex_value(g, u); };
    using VVF = decltype(vvf);
    std::graph::views::vertexlist_iterator<G, VVF> i3(g, vvf, begin(vertices(g)));
    {
      auto&& [ukey, u, name] = *i3;
      REQUIRE(ukey == 0);
      REQUIRE(name == "Frankf\xC3\xBCrt");
    }
    {
      auto&& [ukey, u, name] = *++i3;
      REQUIRE(ukey == 1);
      REQUIRE(name == "Mannheim");
    }
  }

  SECTION("non-const vertexlist") {
    using view_t = decltype(std::graph::views::vertexlist(g));
    static_assert(forward_range<view_t>, "vertexlist(g) is not a forward_range");
    size_t cnt = 0;
    for (auto&& [ukey, u] : std::graph::views::vertexlist(g)) {
      ++cnt;
    }
    REQUIRE(cnt == size(vertices(g)));

    cnt = 0;
    for (auto&& [ukey, u] : std::graph::views::vertexlist(g, begin(vertices(g)), end(vertices(g)))) {
      ++cnt;
    }
    REQUIRE(cnt == size(vertices(g)));
  }

  SECTION("const vertexlist") {
    using G2     = const G;
    G2& g2       = g;
    using view_t = decltype(std::graph::views::vertexlist(g2));
    static_assert(forward_range<view_t>, "vertexlist(g) is not a forward_range");
    size_t cnt = 0;
    for (auto&& [ukey, u] : std::graph::views::vertexlist(g2)) {
      ++cnt;
    }
    REQUIRE(cnt == size(vertices(g)));

    cnt = 0;
    for (auto&& [ukey, u] : std::graph::views::vertexlist(g2, begin(vertices(g2)), end(vertices(g2)))) {
      ++cnt;
    }
    REQUIRE(cnt == size(vertices(g)));
  }

  SECTION("non-const vertexlist with vertex_fn") {
    // Note: must include trailing return type on lambda
    size_t cnt       = 0;
    auto   vertex_fn = [&g](vertex_t<G>& u) -> std::string& { return vertex_value(g, u); };
    for (auto&& [ukey, u, val] : std::graph::views::vertexlist(g, vertex_fn)) {
      ++cnt;
    }
    REQUIRE(cnt == size(vertices(g)));
  }
  SECTION("const vertexlist with vertex_fn") {
    // Note: must include trailing return type on lambda
    using G2   = const G;
    G2&    g2  = g;
    size_t cnt = 0;
    for (auto&& [ukey, u, val] : std::graph::views::vertexlist(
               g2, [&g2](const vertex_t<G>& u) -> const std::string& { return vertex_value(g2, u); })) {
      ++cnt;
    }
    REQUIRE(cnt == size(vertices(g)));
  }
}