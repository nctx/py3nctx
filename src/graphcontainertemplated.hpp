#ifndef _NCTX_PY_GRAPH
#define _NCTX_PY_GRAPH

#include "pythonmodule.hpp"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/reverse_graph.hpp>
#include "python_cpp/iterator_edges.hpp"
#include "python_cpp/iterator_vertices.hpp"
#include "python_cpp/iterator_adjacent.hpp"

namespace nctx { namespace python {
  
  template<bool directed>
  class GraphContainer{
    
    public:
      typedef typename std::conditional<directed, 
          gDirected, 
          gUndirected>::type Graph;
      typedef typename b::graph_traits<Graph>::vertex_descriptor vertex;
      typedef typename b::graph_traits<Graph>::edge_descriptor edge;
      
      typedef typename b::reverse_graph<Graph, Graph&> reverse;
      typedef typename b::graph_traits< reverse >::adjacency_iterator parents;
    
      GraphContainer(){};
      
      size_t num_vertices(){
        return b::num_vertices(g);
      }
      size_t num_edges(){
        return b::num_edges(g);
      }
      
      boost::iterator_range<boost::range_detail::integer_iterator<unsigned long> > make_vertex_iterator(){
        return b::make_iterator_range(b::vertices(g));
      }
      v_iterator<size_t, Graph> get_vertex_iterator(){
        auto p = b::vertices(g);
        return v_iterator<size_t,Graph>(p.first, p.second);
      }
      a_iterator<size_t, Graph> get_children_iterator(const vertex v){
        auto p = b::adjacent_vertices(v,g);
        return a_iterator<size_t, Graph>(p.first, p.second);
      }
      a_iterator<size_t, reverse> get_parent_iterator(const vertex v){
        reverse g_tmp = make_reverse_graph(g);
        auto p = b::adjacent_vertices(v,g_tmp);
        return a_iterator<size_t, reverse>(p.first, p.second);
      }
      e_iterator<Graph> get_edge_iterator(){
        auto p = b::edges(g);
        return e_iterator<Graph>(p.first, p.second, num_edges());
      }
      e_iterator<Graph, typename b::graph_traits<Graph>::in_edge_iterator> get_inedge_iterator(const vertex v){
        auto p = b::in_edges(v, g);
        return e_iterator<Graph, typename b::graph_traits<Graph>::in_edge_iterator>(p.first, p.second, p.second - p.first);
      }
      e_iterator<Graph, typename b::graph_traits<Graph>::out_edge_iterator> get_outedge_iterator(const vertex v){
        auto p = b::out_edges(v, g);
        return e_iterator<Graph, typename b::graph_traits<Graph>::out_edge_iterator>(p.first, p.second, p.second - p.first);
      }
      
      size_t source(Edge<Graph>& e){
        return b::source(e.get_edge(), g);
      }
      size_t target(Edge<Graph>& e){
        return b::target(e.get_edge(), g);
      }
      vertex add_vertex(){
        return b::add_vertex(g);
      }
      bool add_edge(vertex v0, vertex v1){
        std::pair<edge, bool> result = b::add_edge(v0,v1,g);
        return result.second;
      }
      
      auto make_index_map(){
        return b::get(b::vertex_index, g);
      }
      
      Graph& get_graph(){
        return g;
      }
    private:
      Graph g;
  };
  
}}

#endif
