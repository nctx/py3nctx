//  Authors: Mirco Schoenfeld
#ifndef _ALG_CTX_DIJDP_H
#define _ALG_CTX_DIJDP_H

#include <boost/graph/dijkstra_shortest_paths_no_color_map.hpp>

using namespace boost;

namespace nctx {
  
  
  // DocString: dijkstra_ctx
  /**
   * @brief Obtain shortest paths with dynamic contextual constraints
   * 
   * Single-source shortest paths using Dijkstra's algorithm taking into account contextual constraints. Enforcement of constraints is the task of the given user-defined function.
   * 
   * The function enforcing contextual constraints is evaluated at each node during shortest path discovery. The function needs to evaluate to ``true`` or ``false`` allowing an edge to be visited or not. As parameters, the function needs to accept the starting node of path traversal, the current node, and the descending node in question. If the function returns False, the descending node is not being visited. Passing the start vertex is unnecessary here. However, the signature is the same as for the other functions for usability reasons.
   * 
   * The three nodes are passed as indices allowing for access of (external) attribute and other associated information.
   * 
   * Note that the decision function is evaluated more than once during path traversal. That means, there should not happen any resource-intense computation inside this function. Also, it does not allow to keep track of the status of calculation, e.g. by calculating the visited edges or something similar.
   * 
   * If the decision function simply returns True all the time, the set of shortest paths is the unaltered set of shortest paths expected by the classical Dijkstra-implementation.
   * 
   * @param g The graph object
   * @param s The source vertex
   * @param decision_fct A function enforcing constraints. The signature of the function is ``(vertex index, vertex index, vertex index) -> bool``, i.e. the function expects three vertex IDs for the start, the current, and the next vertex. It must evaluate to ``bool``. Passing the start vertex is unnecessary here. However, the signature is the same as for the other functions for usability reasons.
   * 
   */
  template <typename Graph, 
        typename Param, typename Tag, typename Rest,
        typename DistanceMap, 
        typename WeightMap, 
        typename DecisionFunction,
        typename PredecessorMap,
        typename DistanceInfType>
  inline void dijkstra_ctx_dispatch(const Graph& g,
                                typename graph_traits<Graph>::vertex_descriptor start,
                                const bgl_named_params<Param,Tag,Rest>& params, 
                                DistanceMap &distancemap,
                                WeightMap &weightmap,
                                PredecessorMap &pred,
                                typename property_traits<PredecessorMap>::value_type pred_undef,
                                DecisionFunction &decision_fct,
                                DistanceInfType distance_infinity){ 
                                  
    typedef context_dijkstra_visitor<Graph, DistanceMap, WeightMap, PredecessorMap, DistanceInfType> visitor_type;
    
    visitor_type visitor(start, distancemap, weightmap, decision_fct, pred, pred_undef, distance_infinity);
      
    boost::dijkstra_shortest_paths_no_color_map(g, start, 
        params
        .visitor(visitor));
  }

}// end namespace nctx
#endif
