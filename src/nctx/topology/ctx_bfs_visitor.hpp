//  Authors: Mirco Schoenfeld
#ifndef _ALG_CTX_DIJV_H
#define _ALG_CTX_DIJV_H

#include <boost/graph/breadth_first_search.hpp>

using namespace boost;

namespace nctx {
  
  /**
   * @brief A custom visitor passed to Dijkstra's algorithm keeping track of distance information.
   * 
   * Inside this visitor, the user-defined decision function enforcing contextual constraints on path traversal is evaluated. If an edge is relaxed but the decision function evaluates to False, Dijkstra's distance updates are rolled back.
   * 
   * @param s vertex_descriptor The starting point of shortest path discovery
   * @param &distance DistanceMap A pointer to a PropertyMap containing distances to all other nodes in the graph
   * @param &weight WeightMap A pointer to a PropertyMap describing edge weights
   * @param &decision_fct DecisionFunction A pointer to a function ``(size_t start, size_t cur, size_t next)->Bool``. Inside this function, contextual constraints are decided. The first parameter ``start`` contains the start vertex of path discovery, the parameter ``cur`` contains the current vertex, and the last parameter ``next`` is a possible descendant for which constraints are evaluated.
   * @param &pred PredecessorMap A pointer to a PropertyMap keeping track of predecessors of vertices
   * @param pred_undef A value to use to initialize &pred, i.e. for nodes that do not have any predecessors.
   */
  template<typename Graph, 
           typename DistanceMap, 
           typename WeightMap, 
           typename PredecessorMap,
           typename DistanceType = typename property_traits<DistanceMap>::value_type>
  struct context_dijkstra_visitor : public bfs_visitor<>
  {
    typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;
    typedef typename graph_traits<Graph>::edge_descriptor edge_descriptor;
    typedef typename std::function<bool(vertex_descriptor, vertex_descriptor, vertex_descriptor)> DecisionFunction;

    context_dijkstra_visitor(vertex_descriptor s,
                             DistanceMap &distance,
                             const WeightMap &weight,
                             DecisionFunction &decision_fct,
                             PredecessorMap& pred,
                             typename property_traits<PredecessorMap>::value_type pred_undef)
      : s(s), distance(distance), weight(weight),
        decision_fct(decision_fct), pred(pred), pred_undef(pred_undef), distance_inf(0)//distance_inf((std::numeric_limits<DistanceType>::max)())
    { }
    
    context_dijkstra_visitor(vertex_descriptor s,
                             DistanceMap &distance,
                             const WeightMap &weight,
                             DecisionFunction &decision_fct,
                             PredecessorMap& pred,
                             typename property_traits<PredecessorMap>::value_type pred_undef,
                             DistanceType distance_inf)
      : s(s), distance(distance), weight(weight),
        decision_fct(decision_fct), pred(pred), pred_undef(pred_undef), distance_inf(distance_inf)
    { }

    void edge_relaxed(edge_descriptor e, const Graph& g) 
    { 
      vertex_descriptor v = source(e, g), w = target(e, g);
      typedef typename property_traits<WeightMap>::value_type weight_type;
      //~ typedef typename property_traits<DistanceMap>::value_type distance_type;
      typedef typename graph_traits<Graph>::directed_category DirCat;
      bool is_undirected = is_same<DirCat, undirected_tag>::value;

      DistanceType d_v = get(distance, v), d_w = get(distance, w);
      weight_type w_e = get(weight, e);
      closed_plus<DistanceType> combine;
      DistanceType update_w = combine(d_v, w_e),        update_v = combine(d_w, w_e);
      DistanceType old_w = d_w - update_w, old_v = d_v - update_v;
      //~ distance_type inf = (std::numeric_limits<distance_type>::max)();
      
      //~ std::cout << std::boolalpha << v << " -> " << w << " : " << decision_fct(s,v,w) << std::endl;
      if(!decision_fct(s,v,w)){// roll back distance puts
        
        //~ std::cout << s << " : " << v << " -> " << w << "  | d_w " << d_w << "\t old_w " << old_w << std::endl;
        if(d_w == update_w){
          put(distance, w,  old_w <= DistanceType(0) ? distance_inf : old_w);
          put(pred, w, pred_undef);
        }else if (is_undirected && d_v == update_v){
          put(distance, v, old_v <= DistanceType(0) ? distance_inf : old_v);
          put(pred, v, pred_undef); 
        }
        //~ std::cout << "\t" << get(distance, w) << " after update" << std::endl;
      }
    }

    void edge_not_relaxed(edge_descriptor e, const Graph& g){}

    void examine_vertex(vertex_descriptor w, const Graph&){}

  private:
    DistanceMap &distance;
    const WeightMap &weight;
    DecisionFunction &decision_fct;
    vertex_descriptor s;
    DistanceType distance_inf;
    PredecessorMap &pred;
    typename property_traits<PredecessorMap>::value_type pred_undef;
  };
} // end namespace nctx

#endif // BOOST_GRAPH_BRANDES_BETWEENNESS_CENTRALITY_HPP
