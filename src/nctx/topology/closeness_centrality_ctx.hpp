// Copyright 2004 The Trustees of Indiana University.

// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  Authors: Douglas Gregor
//           Andrew Lumsdaine
#ifndef _ALG_CTX_CLSC_H
#define _ALG_CTX_CLSC_H

#include <boost/graph/exterior_property.hpp>
#include <boost/graph/closeness_centrality.hpp>
#include <boost/graph/dijkstra_shortest_paths_no_color_map.hpp>
#include <iostream>

#include "ctx_bfs_visitor.hpp"
using namespace boost;

namespace nctx {
  
  template <typename G, typename Param, typename Tag, typename Rest,
    typename DistanceMap, 
    typename WeightMap, 
    typename PredecessorMap,
    typename DecisionFunction>
  inline void closeness_centrality_ctx_dispatch(const G& g,
                                typename graph_traits<G>::vertex_descriptor start,
                                const bgl_named_params<Param,Tag,Rest>& params, 
                                DistanceMap &distancemap,
                                WeightMap &weightmap,
                                PredecessorMap &pred,
                                typename property_traits<PredecessorMap>::value_type pred_undef,
                                DecisionFunction &clsn_decision_fct){ 
    //~ auto distance_map = b::get_param(params, b::vertex_distance);
    //~ auto weight_map = b::get_param(params, b::edge_weight);
    //~ auto distance_inf = b::get_param(params, b::distance_inf_t());
    
    typedef context_dijkstra_visitor<G, DistanceMap, WeightMap, PredecessorMap> visitor_type;
    
    // can't use size_t inf here. this produces uncorrect centrality values.
    // using the number of vertices is the logical inf here
    visitor_type visitor(start, distancemap, weightmap, clsn_decision_fct, pred, pred_undef, typename property_traits<DistanceMap>::value_type(num_vertices(g)));
    
      
    boost::dijkstra_shortest_paths_no_color_map(g, start, 
        params
        .visitor(visitor));
  }
  
  // DocString: closenness_ctx
/**
 * @brief Closeness centrality with dynamic contextual constraints
 * 
 * Using this function allows obtaining closeness centrality under dynamic contextual constraints. Enforcement of constraints is the task of the given user-defined function.
 * 
 * The function enforcing contextual constraints is evaluated at each node during shortest path traversal. The function needs to evaluate to True or False allowing an edge to be visited or not. As parameters, the current state of the centrality calculation is passed to the function, i.e. the starting node for which a centrality value is being calculated, the current node, and the descending node in question. If the function returns False, the descending node is not being visited. 
 * 
 * The three nodes are passed as indices allowing for access of (external) attribute and other associated information. 
 * 
 * Note that the decision function is evaluated more than once during path traversal. That means, there should not happen any resource-intense computation inside this function. Also, it does not allow to keep track of the status of calculation, e.g. by calculating the visited edges or something similar.
 * 
 * If the decision function simply returns True all the time, this function results in the unaltered betweenness centrality values.
 * 
 * @param g The graph object
 * @param clsn_decision_fct A function enforcing constraints. The signature of the function is ``(vertex index, vertex index, vertex index) -> Bool``.
 * 
 */
  template <typename Graph, typename Param, typename Tag, typename Rest, typename DecisionFunction>
  void closeness_centrality_ctx(const Graph& g,
                                const bgl_named_params<Param,Tag,Rest>& params, 
                                DecisionFunction &clsn_decision_fct){
    typedef exterior_vertex_property<Graph, int> DistanceProperty;
    typedef typename DistanceProperty::matrix_type DistanceMatrix;
    typedef typename DistanceProperty::matrix_map_type DistanceMatrixMap;
    typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
    
    DistanceMatrix distances_matr(b::num_vertices(g));
    DistanceMatrixMap dm(distances_matr, g);
    
    auto edge_weight_map = choose_const_pmap(get_param(params, edge_weight), g, edge_weight);
    
    const int n = b::num_vertices(g);
    
    for(auto start : make_iterator_range(vertices(g))){
      std::vector<size_t> distances(n, 0);
      
      auto distance_map = make_iterator_property_map(distances.begin(), choose_param(get_param(params, vertex_index), dummy_property_map()));
      auto dummy_pred = dummy_property_map();
      
      closeness_centrality_ctx_dispatch(g, start, 
        params
        .distance_map(distance_map),
        distance_map,
        edge_weight_map,
        dummy_pred,
        0,
        clsn_decision_fct);
         
      for(auto u : make_iterator_range(vertices(g))){
        //~ std::cout << start << " -> " << u << ": " << distances[u] << std::endl;
        dm[start][u] = distances[u];
      }
    }
    
    typedef exterior_vertex_property<Graph, float> ClosenessProperty;
    typedef typename ClosenessProperty::container_type ClosenessContainer;
    //~ typedef typename ClosenessProperty::map_type ClosenessMap;
    ClosenessContainer cents(n);
    //~ ClosenessMap cm(cents, g);
    all_closeness_centralities(g, dm, choose_param(get_param(params, vertex_centrality), dummy_property_map()));
  }

} // end namespace boost

#endif // BOOST_GRAPH_BRANDES_BETWEENNESS_CENTRALITY_HPP
