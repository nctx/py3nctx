// Copyright 2004-5 The Trustees of Indiana University.
// Copyright 2002 Brad King and Douglas Gregor

// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine

#ifndef _ALG_PAGE_RANK_HPP
#define _ALG_PAGE_RANK_HPP

#include <boost/property_map/property_map.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/iteration_macros.hpp>
#include <boost/graph/overloading.hpp>
#include <boost/graph/page_rank.hpp>
#include <vector>

namespace boost { namespace graph {

//~ struct n_iterations
//~ {
  //~ explicit n_iterations(std::size_t n) : n(n) { }

  //~ template<typename RankMap, typename Graph>
  //~ bool 
  //~ operator()(const RankMap&, const Graph&)
  //~ {
    //~ return n-- == 0;
  //~ }

 //~ private:
  //~ std::size_t n;
//~ };

namespace detail {
  template<typename Graph, typename RankMap, typename RankMap2>
  void page_rank_step(const Graph& g, RankMap from_rank, RankMap2 to_rank,
                      typename property_traits<RankMap>::value_type damping,
                      std::function<typename property_traits<RankMap>::value_type (typename graph_traits<Graph>::vertex_descriptor, typename graph_traits<Graph>::vertex_descriptor)> decision_fct,
                      incidence_graph_tag)
  {
    typedef typename property_traits<RankMap>::value_type rank_type;

    // Set new rank maps 
    BGL_FORALL_VERTICES_T(v, g, Graph) put(to_rank, v, rank_type(1 - damping));

    BGL_FORALL_VERTICES_T(u, g, Graph) {
      rank_type u_rank_out = damping * get(from_rank, u) / out_degree(u, g);
      BGL_FORALL_ADJ_T(u, v, g, Graph){
        rank_type ctx_factor = decision_fct(u, v);
        put(to_rank, v, get(to_rank, v) + u_rank_out * ctx_factor);
      }
    }
  }

  template<typename Graph, typename RankMap, typename RankMap2>
  void page_rank_step(const Graph& g, RankMap from_rank, RankMap2 to_rank,
                      typename property_traits<RankMap>::value_type damping,
                      std::function<typename property_traits<RankMap>::value_type (typename graph_traits<Graph>::vertex_descriptor, typename graph_traits<Graph>::vertex_descriptor)> decision_fct,
                      bidirectional_graph_tag)
  {
    typedef typename property_traits<RankMap>::value_type damping_type;
    BGL_FORALL_VERTICES_T(v, g, Graph) {
      typename property_traits<RankMap>::value_type rank(0);
      BGL_FORALL_INEDGES_T(v, e, g, Graph){
        damping_type ctx_factor = decision_fct(v, source(e, g));
        rank += get(from_rank, source(e, g)) / out_degree(source(e, g), g) * ctx_factor;
      }
      put(to_rank, v, (damping_type(1) - damping) + damping * rank);
    }
  }
} // end namespace detail

template<typename Graph, typename RankMap, typename Done, typename RankMap2>
void
page_rank(const Graph& g, RankMap rank_map,
          std::function<typename property_traits<RankMap>::value_type (typename graph_traits<Graph>::vertex_descriptor, typename graph_traits<Graph>::vertex_descriptor)> decision_fct, 
          Done done, 
          typename property_traits<RankMap>::value_type damping,
          typename graph_traits<Graph>::vertices_size_type n,
          RankMap2 rank_map2
          BOOST_GRAPH_ENABLE_IF_MODELS_PARM(Graph, vertex_list_graph_tag))
{
  typedef typename property_traits<RankMap>::value_type rank_type;

  rank_type initial_rank = rank_type(rank_type(1) / n);
  BGL_FORALL_VERTICES_T(v, g, Graph) put(rank_map, v, initial_rank);

  bool to_map_2 = true;
  while ((to_map_2 && !done(rank_map, g)) ||
         (!to_map_2 && !done(rank_map2, g))) {
    typedef typename graph_traits<Graph>::traversal_category category;

    if (to_map_2) {
      detail::page_rank_step(g, rank_map, rank_map2, damping, decision_fct, category());
    } else {
      detail::page_rank_step(g, rank_map2, rank_map, damping, decision_fct, category());
    }
    to_map_2 = !to_map_2;
  }

  if (!to_map_2) {
    BGL_FORALL_VERTICES_T(v, g, Graph) put(rank_map, v, get(rank_map2, v));
  }
}

template<typename Graph, typename RankMap, typename Done>
void
page_rank(const Graph& g, RankMap rank_map,
          std::function<typename property_traits<RankMap>::value_type (typename graph_traits<Graph>::vertex_descriptor, typename graph_traits<Graph>::vertex_descriptor)> decision_fct, 
          Done done, 
          typename property_traits<RankMap>::value_type damping,
          typename graph_traits<Graph>::vertices_size_type n)
{
  typedef typename property_traits<RankMap>::value_type rank_type;

  std::vector<rank_type> ranks2(num_vertices(g));
  page_rank(g, rank_map, decision_fct, done, damping, n,
            make_iterator_property_map(ranks2.begin(), get(vertex_index, g)));
}

template<typename Graph, typename RankMap, typename Done>
inline void
page_rank_ctx(const Graph& g, RankMap rank_map,
          std::function<typename property_traits<RankMap>::value_type (typename graph_traits<Graph>::vertex_descriptor, typename graph_traits<Graph>::vertex_descriptor)> decision_fct, 
          Done done, 
          typename property_traits<RankMap>::value_type damping = 0.85)
{
  page_rank(g, rank_map, decision_fct, done, damping, num_vertices(g));
}

template<typename Graph, typename RankMap>
inline void
page_rank_ctx(const Graph& g, RankMap rank_map,
          std::function<typename property_traits<RankMap>::value_type (typename graph_traits<Graph>::vertex_descriptor, typename graph_traits<Graph>::vertex_descriptor)> decision_fct)
{
  page_rank_ctx(g, rank_map, decision_fct, n_iterations(20));
}


} } // end namespace boost::graph

#ifdef BOOST_GRAPH_USE_MPI
#  include <boost/graph/distributed/page_rank.hpp>
#endif

#endif // BOOST_GRAPH_PAGE_RANK_HPP
