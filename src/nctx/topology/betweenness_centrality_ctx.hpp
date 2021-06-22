// Copyright 2004 The Trustees of Indiana University.

// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#ifndef _ALG_CTX_BWTC_H
#define _ALG_CTX_BWTC_H

#include <stack>
#include <vector>
#include <boost/graph/overloading.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/dijkstra_shortest_paths_no_color_map.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/relax.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/mpl/if.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/named_function_params.hpp>
#include <algorithm>

#include <functional>

using namespace boost;

namespace nctx {

namespace detail { namespace graph {

  /**
   * @brief Dijkstra visitor keeping track of vertices, shortest paths and updates due to decisions on constraints.
   * 
   * This visitor is based on the Boost implementation. The corresponding documentation states:
   * 
   * Customized visitor passed to Dijkstra's algorithm by Brandes'
   * betweenness centrality algorithm. This visitor is responsible for
   * keeping track of the order in which vertices are discovered, the
   * predecessors on the shortest path(s) to a vertex, and the number
   * of shortest paths.
   * 
   * In the extension of nctx, this visitor is also responsible for keeping track of the decisions resulting from the decision function. If the function prohibits traversal to a descending node, the visitor rolls back distance updates by Dijkstra's algorithm.
   */
  template<typename Graph, typename WeightMap, typename IncomingMap,
           typename DistanceMap, typename PathCountMap>
  struct brandes_dijkstra_visitor : public bfs_visitor<>
  {
    typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;
    typedef typename graph_traits<Graph>::edge_descriptor edge_descriptor;
    typedef typename std::function<bool(typename graph_traits<Graph>::vertex_descriptor, typename graph_traits<Graph>::vertex_descriptor, typename graph_traits<Graph>::vertex_descriptor)> decision_function;

    brandes_dijkstra_visitor(std::stack<vertex_descriptor>& ordered_vertices,
                             WeightMap weight,
                             IncomingMap incoming,
                             DistanceMap distance,
                             PathCountMap path_count_constraint,
                             PathCountMap path_count_unconstraint,
                             decision_function &betw_decision_fct,
                             vertex_descriptor s)
      : ordered_vertices(ordered_vertices), weight(weight), 
        incoming(incoming), distance(distance),
        path_count_constraint(path_count_constraint),
        path_count_unconstraint(path_count_unconstraint), betw_decision_fct(betw_decision_fct),
        s(s)
    { }

    /**
     * Whenever an edge e = (v, w) is relaxed, the incoming edge list
     * for w is set to {(v, w)} and the shortest path count of w is set to
     * the number of paths that reach {v}.
     */
    void edge_relaxed(edge_descriptor e, const Graph& g) 
    { 
      vertex_descriptor v = source(e, g), w = target(e, g);
      incoming[w].clear();
      incoming[w].push_back(e);
      put(path_count_unconstraint, w, get(path_count_unconstraint, v));
      if(betw_decision_fct(s,v,w)){
        put(path_count_constraint, w, get(path_count_constraint, v));
      }
    }

    /**
     * If an edge e = (v, w) was not relaxed, it may still be the case
     * that we've found more equally-short paths, so include {(v, w)} in the
     * incoming edges of w and add all of the shortest paths to v to the
     * shortest path count of w.
     */
    void edge_not_relaxed(edge_descriptor e, const Graph& g) 
    {
      typedef typename property_traits<WeightMap>::value_type weight_type;
      typedef typename property_traits<DistanceMap>::value_type distance_type;
      vertex_descriptor v = source(e, g), w = target(e, g);

      distance_type d_v = get(distance, v), d_w = get(distance, w);
      weight_type w_e = get(weight, e);
      closed_plus<distance_type> combine;
      if (d_w == combine(d_v, w_e)) {
        put(path_count_unconstraint, w, get(path_count_unconstraint, w) + get(path_count_unconstraint, v));
        incoming[w].push_back(e);
        if(betw_decision_fct(s,v,w)){
          put(path_count_constraint, w, get(path_count_constraint, w) + get(path_count_constraint, v));
        }
      }
      
    }

    /// Keep track of vertices as they are reached
    void examine_vertex(vertex_descriptor w, const Graph&) 
    { 
      ordered_vertices.push(w);
    }

  private:
    std::stack<vertex_descriptor>& ordered_vertices;
    WeightMap weight;
    IncomingMap incoming;
    DistanceMap distance;
    PathCountMap path_count_constraint;
    PathCountMap path_count_unconstraint;
    decision_function &betw_decision_fct;
    vertex_descriptor s;
  };

  /**
   * Function object that calls Dijkstra's shortest paths algorithm
   * using the Dijkstra visitor for the Brandes betweenness centrality
   * algorithm.
   */
  template<typename WeightMap, typename Graph, typename BetweennessDecisionFunction>
  struct brandes_dijkstra_shortest_paths
  {
    typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;
    //~ typedef typename BetweennessDecisionFunction = std::function<bool(vertex_descriptor, vertex_descriptor, vertex_descriptor)>;
    brandes_dijkstra_shortest_paths(WeightMap weight_map,
               BetweennessDecisionFunction &betw_decision_fct) 
      : weight_map(weight_map), betw_decision_fct(betw_decision_fct) { }

    template<typename IncomingMap, typename DistanceMap, 
             typename PathCountMap, typename VertexIndexMap>
    void 
    operator()(Graph& g, 
               vertex_descriptor s,
               std::stack<vertex_descriptor>& ov,
               IncomingMap incoming,
               DistanceMap distance,
               PathCountMap path_count_constraint,
               PathCountMap path_count_unconstraint,
               VertexIndexMap vertex_index)
    {
      typedef brandes_dijkstra_visitor<Graph, WeightMap, IncomingMap, 
                                       DistanceMap, PathCountMap> visitor_type;
      visitor_type visitor(ov, weight_map, incoming, distance, 
        path_count_constraint, 
        path_count_unconstraint, betw_decision_fct, s);
      
      boost::dijkstra_shortest_paths(g, s, 
                              boost::weight_map(weight_map)
                              .vertex_index_map(vertex_index)
                              .distance_map(distance)
                              .visitor(visitor));
    }

  private:
    BetweennessDecisionFunction betw_decision_fct;
    WeightMap weight_map;
  };

  // When the edge centrality map is a dummy property map, no
  // initialization is needed.
  template<typename Iter>
  inline void 
  init_centrality_map(std::pair<Iter, Iter>, dummy_property_map) { }

  // When we have a real edge centrality map, initialize all of the
  // centralities to zero.
  template<typename Iter, typename Centrality>
  void 
  init_centrality_map(std::pair<Iter, Iter> keys, Centrality centrality_map)
  {
    typedef typename property_traits<Centrality>::value_type 
      centrality_type;
    while (keys.first != keys.second) {
      put(centrality_map, *keys.first, centrality_type(0));
      ++keys.first;
    }
  }

  // When the edge centrality map is a dummy property map, no update
  // is performed.
  template<typename Key, typename T>
  inline void 
  update_centrality(dummy_property_map, const Key&, const T&) { }

  // When we have a real edge centrality map, add the value to the map
  template<typename CentralityMap, typename Key, typename T>
  inline void 
  update_centrality(CentralityMap centrality_map, Key k, const T& x)
  { put(centrality_map, k, get(centrality_map, k) + x); }

  template<typename Iter>
  inline void 
  divide_centrality_by_two(std::pair<Iter, Iter>, dummy_property_map) {}

  template<typename Iter, typename CentralityMap>
  inline void
  divide_centrality_by_two(std::pair<Iter, Iter> keys, 
                           CentralityMap centrality_map)
  {
    typename property_traits<CentralityMap>::value_type two(2);
    while (keys.first != keys.second) {
      put(centrality_map, *keys.first, get(centrality_map, *keys.first) / two);
      ++keys.first;
    }
  }

  template<typename Graph, typename CentralityMap, typename EdgeCentralityMap,
           typename IncomingMap, typename DistanceMap, 
           typename DependencyMap, typename PathCountMap,
           typename VertexIndexMap, typename ShortestPaths> 
  void 
  brandes_betweenness_centrality_impl(const Graph& g, 
                                      CentralityMap centrality,     // C_B
                                      EdgeCentralityMap edge_centrality_map,
                                      IncomingMap incoming, // P
                                      DistanceMap distance,         // d
                                      DependencyMap dependency,     // delta
                                      PathCountMap path_count_constraint,      // sigma
                                      PathCountMap path_count_unconstraint,      // sigma
                                      VertexIndexMap vertex_index,
                                      ShortestPaths shortest_paths)
  {
    typedef typename graph_traits<Graph>::vertex_iterator vertex_iterator;
    typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;
    //~ const BglGraph<T>& g = *(gc.get_graph());
    // Initialize centrality
    init_centrality_map(vertices(g), centrality);
    init_centrality_map(edges(g), edge_centrality_map);

    std::stack<vertex_descriptor> ordered_vertices;
    vertex_iterator s, s_end;
    for (boost::tie(s, s_end) = vertices(g); s != s_end; ++s) {
      // Initialize for this iteration
      vertex_iterator w, w_end;
      for (boost::tie(w, w_end) = vertices(g); w != w_end; ++w) {
        incoming[*w].clear();
        put(path_count_constraint, *w, 0);
        put(path_count_unconstraint, *w, 0);
        put(dependency, *w, 0);
      }
      put(path_count_constraint, *s, 1);
      put(path_count_unconstraint, *s, 1);
      
      // Execute the shortest paths algorithm. This will be either
      // Dijkstra's algorithm or a customized breadth-first search,
      // depending on whether the graph is weighted or unweighted.
      shortest_paths(g, *s, ordered_vertices, incoming, distance,
                     path_count_constraint, 
                     path_count_unconstraint, vertex_index);
      
      while (!ordered_vertices.empty()) {
        vertex_descriptor w = ordered_vertices.top();
        ordered_vertices.pop();
        
        typedef typename property_traits<IncomingMap>::value_type
          incoming_type;
        typedef typename incoming_type::iterator incoming_iterator;
        typedef typename property_traits<DependencyMap>::value_type 
          dependency_type;
        
        for (incoming_iterator vw = incoming[w].begin();
             vw != incoming[w].end(); ++vw) {
          vertex_descriptor v = source(*vw, g);
          //~ dependency_type factor_raw = dependency_type(get(path_count_unconstraint, v))
            //~ / dependency_type(get(path_count_unconstraint, w));
          
          if(dependency_type(get(path_count_constraint, w)) > dependency_type(0)){
            bool ctx_fac = (dependency_type(get(path_count_constraint, v)) != dependency_type(get(path_count_unconstraint, v)))
              && (dependency_type(get(path_count_constraint, w)) != dependency_type(get(path_count_unconstraint, w)));
            
            //~ dependency_type factor = 
              //~ ((dependency_type(get(path_count_unconstraint, v)) + dependency_type(get(path_count_constraint, v))) / dependency_type(2))
              //~ / ((dependency_type(get(path_count_unconstraint, w)) + dependency_type(get(path_count_constraint, w))) / dependency_type(2));
            dependency_type factor = (ctx_fac) ?
              (dependency_type(get(path_count_constraint, v)) / dependency_type(get(path_count_constraint, w)))
              * (dependency_type(get(path_count_unconstraint, v)) / dependency_type(get(path_count_unconstraint, w)))
              :
              dependency_type(get(path_count_unconstraint, v)) / dependency_type(get(path_count_unconstraint, w));
            //~ if(factor_raw == dependency_type(0)){
              //~ std::cout << *s << " | " << v << " -> " << w <<  ": true: " << factor_raw << " \t fac: " << factor << std::endl;
              //~ std::cout << "\t  constraint v: \t" << dependency_type(get(path_count_constraint, v)) << std::endl
                        //~ << "\tunconstraint v: \t" << dependency_type(get(path_count_unconstraint, v)) << std::endl
                        //~ << "\t  constraint w: \t" << dependency_type(get(path_count_constraint, w)) << std::endl
                        //~ << "\tunconstraint w: \t" << dependency_type(get(path_count_unconstraint, w)) << std::endl;
            //~ }
            //~ if(dependency_type(get(path_count_constraint, v)) != dependency_type(get(path_count_unconstraint, v)))
              //~ std::cout << *s << " | " << v << " -> " << w <<  ": true: " << factor_raw << " \t fac: " << factor << std::endl
                        //~ << "\t  constraint v: \t" << dependency_type(get(path_count_constraint, v)) << std::endl
                        //~ << "\tunconstraint v: \t" << dependency_type(get(path_count_unconstraint, v)) << std::endl;
            factor *= (dependency_type(1) + get(dependency, w));
            put(dependency, v, get(dependency, v) + factor);
            update_centrality(edge_centrality_map, *vw, factor);
          }
        }
        
        if (w != *s) {
          update_centrality(centrality, w, get(dependency, w));
        }
      }
    }

    typedef typename graph_traits<Graph>::directed_category directed_category;
    const bool is_undirected = 
      is_convertible<directed_category*, undirected_tag*>::value;
    if (is_undirected) {
      divide_centrality_by_two(vertices(g), centrality);
      divide_centrality_by_two(edges(g), edge_centrality_map);
    }
  }

} } // end namespace detail::graph

template<typename Graph, typename CentralityMap, typename EdgeCentralityMap, 
         typename IncomingMap, typename DistanceMap, 
         typename DependencyMap, typename PathCountMap, 
         typename VertexIndexMap, typename WeightMap,
         typename BetweennessDecisionFunction>    
void 
brandes_betweenness_centrality(const Graph& g, 
                               CentralityMap centrality,     // C_B
                               EdgeCentralityMap edge_centrality_map,
                               IncomingMap incoming, // P
                               DistanceMap distance,         // d
                               DependencyMap dependency,     // delta
                               PathCountMap path_count_constraint,      // sigma
                               PathCountMap path_count_unconstraint,      // sigma
                               VertexIndexMap vertex_index,
                               WeightMap weight_map,
                               BetweennessDecisionFunction &betw_decision_fct
                               BOOST_GRAPH_ENABLE_IF_MODELS_PARM(Graph,vertex_list_graph_tag))
{
  
  detail::graph::brandes_dijkstra_shortest_paths<WeightMap, const Graph, BetweennessDecisionFunction>
    shortest_paths(weight_map,betw_decision_fct);

  detail::graph::brandes_betweenness_centrality_impl(g, centrality, 
                                                     edge_centrality_map,
                                                     incoming, distance,
                                                     dependency, path_count_constraint,
                                                     path_count_unconstraint,
                                                     vertex_index, 
                                                     shortest_paths);
}

namespace detail { namespace graph {
  template<typename Graph, typename CentralityMap, typename EdgeCentralityMap,
           typename WeightMap, typename VertexIndexMap,
           typename BetweennessDecisionFunction>
  void 
  brandes_betweenness_centrality_dispatch2(const Graph& g,
                                           CentralityMap centrality,
                                           EdgeCentralityMap edge_centrality_map,
                                           WeightMap weight_map,
                                           VertexIndexMap vertex_index,
                                           BetweennessDecisionFunction &betw_decision_fct)
  {
    typedef typename graph_traits<Graph>::degree_size_type degree_size_type;
    typedef typename graph_traits<Graph>::edge_descriptor edge_descriptor;
    typedef typename mpl::if_c<(is_same<CentralityMap, 
                                        dummy_property_map>::value),
                                         EdgeCentralityMap, 
                               CentralityMap>::type a_centrality_map;
    typedef typename property_traits<a_centrality_map>::value_type 
      centrality_type;

    typename graph_traits<Graph>::vertices_size_type V = num_vertices(g);
    
    std::vector<std::vector<edge_descriptor> > incoming(V);
    std::vector<centrality_type> distance(V);
    std::vector<centrality_type> dependency(V);
    std::vector<degree_size_type> path_count_constraint(V);
    std::vector<degree_size_type> path_count_unconstraint(V);

    brandes_betweenness_centrality(
      g, centrality, edge_centrality_map,
      make_iterator_property_map(incoming.begin(), vertex_index),
      make_iterator_property_map(distance.begin(), vertex_index),
      make_iterator_property_map(dependency.begin(), vertex_index),
      make_iterator_property_map(path_count_constraint.begin(), vertex_index),
      make_iterator_property_map(path_count_unconstraint.begin(), vertex_index),
      vertex_index,
      weight_map,
      betw_decision_fct);
  }
  

  template<typename Graph, typename CentralityMap, typename EdgeCentralityMap,
           typename VertexIndexMap>
  void 
  brandes_betweenness_centrality_dispatch2(const Graph& g,
                                           CentralityMap centrality,
                                           EdgeCentralityMap edge_centrality_map,
                                           VertexIndexMap vertex_index)
  {
    typedef typename graph_traits<Graph>::degree_size_type degree_size_type;
    typedef typename graph_traits<Graph>::edge_descriptor edge_descriptor;
    typedef typename mpl::if_c<(is_same<CentralityMap, 
                                        dummy_property_map>::value),
                                         EdgeCentralityMap, 
                               CentralityMap>::type a_centrality_map;
    typedef typename property_traits<a_centrality_map>::value_type 
      centrality_type;

    typename graph_traits<Graph>::vertices_size_type V = num_vertices(g);
    
    std::vector<std::vector<edge_descriptor> > incoming(V);
    std::vector<centrality_type> distance(V);
    std::vector<centrality_type> dependency(V);
    std::vector<degree_size_type> path_count(V);

    brandes_betweenness_centrality(
      g, centrality, edge_centrality_map,
      make_iterator_property_map(incoming.begin(), vertex_index),
      make_iterator_property_map(distance.begin(), vertex_index),
      make_iterator_property_map(dependency.begin(), vertex_index),
      make_iterator_property_map(path_count.begin(), vertex_index),
      vertex_index);
  }

  template<typename Graph, typename WeightMap, typename BetweennessDecisionFunction>
  struct brandes_betweenness_centrality_dispatch1
  {
    template<typename CentralityMap, 
             typename EdgeCentralityMap, typename VertexIndexMap>
    static void 
    run(const Graph& g, CentralityMap centrality, 
        EdgeCentralityMap edge_centrality_map, VertexIndexMap vertex_index,
        WeightMap weight_map,
        BetweennessDecisionFunction &betw_decision_fct)
    {
      brandes_betweenness_centrality_dispatch2(g, centrality, edge_centrality_map,
                                               weight_map, vertex_index,
                                               betw_decision_fct);
    }
  };

  template <typename T>
  struct is_bgl_named_params {
    BOOST_STATIC_CONSTANT(bool, value = false);
  };

  template <typename Param, typename Tag, typename Rest>
  struct is_bgl_named_params<bgl_named_params<Param, Tag, Rest> > {
    BOOST_STATIC_CONSTANT(bool, value = true);
  };

} } // end namespace detail::graph

// DocString: betweenness_ctx
/**
 * @brief Betweenness centrality with dynamic contextual constraints
 * 
 * Using this function allows obtaining betweenness centrality under dynamic contextual constraints. Enforcement of constraints is the task of the given user-defined function.
 * 
 * The function enforcing contextual constraints is evaluated at each node during shortest path traversal. The function needs to evaluate to True or False allowing an edge to be visited or not. As parameters, the current state of the betweenness calculation is passed to the function, i.e. the starting node for which a centrality value is being calculated, the current node, and the descending node in question. If the function returns False, the descending node is not being visited. 
 * 
 * The three nodes are passed as indices allowing for access of (external) attribute and other associated information.
 * 
 * Note that the decision function is evaluated more than once during path traversal. That means, there should not happen any resource-intense computation inside this function. Also, it does not allow to keep track of the status of calculation, e.g. by calculating the visited edges or something similar.
 * 
 * If the decision function simply returns True all the time, this function results in the unaltered betweenness centrality values.
 * 
 * Obtaining betweenness centrality is based on Brandes' efficient algorithm. At the current stage, the implementation allows for single-core execution only.
 * 
 * @param g The graph object
 * @param betw_decision_fct A function enforcing constraints. The signature of the function is ``(vertex index, vertex index, vertex index) -> Bool``.
 * 
 */
template<typename Graph, typename Param, typename Tag, typename Rest, typename DecisionFunction>
void 
brandes_betweenness_centrality_ctx(const Graph& g, 
                               const bgl_named_params<Param,Tag,Rest>& params,
                               DecisionFunction &betw_decision_fct
                               BOOST_GRAPH_ENABLE_IF_MODELS_PARM(Graph,vertex_list_graph_tag))
{
  typedef bgl_named_params<Param,Tag,Rest> named_params;
  typedef typename get_param_type<edge_weight_t, named_params>::type ew;
  
  detail::graph::brandes_betweenness_centrality_dispatch1<Graph, ew, DecisionFunction>::run(
    g, 
    choose_param(get_param(params, vertex_centrality), 
                 dummy_property_map()),
    choose_param(get_param(params, edge_centrality), 
                 dummy_property_map()),
    choose_param(get_param(params, vertex_index), 
                 dummy_property_map()),
    get_param(params, edge_weight),
    betw_decision_fct);
    
}

// disable_if is required to work around problem with MSVC 7.1 (it seems to not
// get partial ordering getween this overload and the previous one correct)
//~ template<typename Graph, typename CentralityMap>
//~ typename disable_if<detail::graph::is_bgl_named_params<CentralityMap>,
                    //~ void>::type
//~ brandes_betweenness_centrality(const Graph& g, CentralityMap centrality
                               //~ BOOST_GRAPH_ENABLE_IF_MODELS_PARM(Graph,vertex_list_graph_tag))
//~ {
  //~ detail::graph::brandes_betweenness_centrality_dispatch2(
    //~ g, centrality, dummy_property_map(), get(vertex_index, g)); //TODO INDIZES
//~ }

//~ template<typename Graph, typename CentralityMap, typename EdgeCentralityMap>
//~ void 
//~ brandes_betweenness_centrality(const Graph& g, CentralityMap centrality,
                               //~ EdgeCentralityMap edge_centrality_map
                               //~ BOOST_GRAPH_ENABLE_IF_MODELS_PARM(Graph,vertex_list_graph_tag))
//~ {
  //~ detail::graph::brandes_betweenness_centrality_dispatch2(
    //~ g, centrality, edge_centrality_map, get(vertex_index, g)); //TODO INDIZES
//~ }

/**
 * Converts "absolute" betweenness centrality (as computed by the
 * brandes_betweenness_centrality algorithm) in the centrality map
 * into "relative" centrality. The result is placed back into the
 * given centrality map.
 */
template<typename Graph, typename CentralityMap>
void 
relative_betweenness_centrality(const Graph& g, CentralityMap centrality)
{
  typedef typename graph_traits<Graph>::vertex_iterator vertex_iterator;
  typedef typename property_traits<CentralityMap>::value_type centrality_type;

  typename graph_traits<Graph>::vertices_size_type n = num_vertices(g);
  centrality_type factor = centrality_type(2)/centrality_type(n*n - 3*n + 2);
  vertex_iterator v, v_end;
  for (boost::tie(v, v_end) = vertices(g); v != v_end; ++v) {
    put(centrality, *v, factor * get(centrality, *v));
  }
}

} // end namespace boost

#endif // BOOST_GRAPH_BRANDES_BETWEENNESS_CENTRALITY_HPP
