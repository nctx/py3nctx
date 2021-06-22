#ifndef _N_ITRT_H
#define _N_ITRT_H

#include "iterator_base.hpp"
namespace b = boost;

namespace nctx { namespace python{
  
  template <typename T, typename G=b::adjacency_list<>>
  class a_iterator : public base_iterator<T, typename b::graph_traits<G>::adjacency_iterator, G> {
    typedef typename b::graph_traits<G>::adjacency_iterator adjacent;
    
    public:
      a_iterator(adjacent first, adjacent last) : base_iterator<T,adjacent,G>() {
        this->first = first;
        this->last = last;
        this->init();
      }
  };
  
  
}}// proc

#endif
