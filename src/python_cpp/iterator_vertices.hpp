#ifndef _V_ITRT_H
#define _V_ITRT_H

#include "iterator_base.hpp"
namespace b = boost;

namespace nctx { namespace python{
  
  template <typename T, typename G=b::adjacency_list<>>
  class v_iterator : public base_iterator<T, typename b::graph_traits<G>::vertex_iterator, G> {
    typedef typename b::graph_traits<G>::vertex_iterator verts;
    
    public:
      v_iterator(verts first, verts last) : base_iterator<T,verts,G>() {
        this->first = first;
        this->last = last;
        this->init();
      }
  };
  
  
}}// proc

#endif
