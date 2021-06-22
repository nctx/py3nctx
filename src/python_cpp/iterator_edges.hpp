#ifndef _E_ITRT_H
#define _E_ITRT_H

namespace b = boost;

namespace nctx { namespace python{
  
  template <typename G>
  class Edge 
  {
    typedef typename b::graph_traits<G>::edge_descriptor edgd;
    
    public:
      Edge(edgd e) : ed(e){}
    
      edgd get_edge(){
        return ed;
      }
    
    private:
      edgd ed;
  };
  
  template <typename G, typename iterator_type = typename b::graph_traits<G>::edge_iterator>
  class e_iterator : public b::iterator_facade<e_iterator<G>, Edge<G> const, b::random_access_traversal_tag>
  {
    //~ typedef typename b::graph_traits<G>::edge_iterator edgi;
    typedef iterator_type edgi;
    typedef e_iterator<G> iterator;
    //~ typedef typename b::graph_traits<G>::edge_descriptor edge;
    
    public:
      e_iterator(edgi first, edgi last, size_t n_e) : n_edges(n_e), first(first), last(last), i(first), cur(*first) {
        if(i != last)
          update(cur);
      }
      
      size_t get_size(){
        return n_edges;
      }
      
      Edge<G> const* dereference() const {return cur;} //

      Edge<G> next(){
        if(i == last){
          PyErr_SetString(PyExc_StopIteration, "No more data.");
          py::throw_error_already_set();
        }
        Edge<G> prev(cur);
        increment();
        return prev;
      }

      void increment(){
        i++;
        //~ std::advance(i);
        if(i != last)
          update(cur);
      }
      
      void decrement(){
        i--;
        //~ std::advance(i,-1);
        if(i != first)
          update(cur);
      }
      
      void advance(int n){
        i += n;
        //~ std::advance(i,n);
        if(i >= first && i < last)
          update(cur);
      }
      
      //~ auto distance_to(iterator const &other) const {
        //~ std::cout << "distance to " << other.cur << ": " << this->i - other.i << std::endl;
        //~ return this->i - other.i;
      //~ }
      
      bool equal(iterator const &other) const{
        return this->i == other.i;
      }
      
      Edge<G> operator*() {return cur;} //
      
      bool operator==(const iterator& rhs) const {return i == rhs.i;}
      //~ bool operator!=(const base_iterator& rhs) const {return !(*this == rhs);}
      bool operator!=(const iterator& rhs) const {return i != rhs.last;}
      iterator begin(){ return *this; }
      iterator end(){ return *this; }
      
    private:
      size_t n_edges;
      
      edgi first, last, i;
      Edge<G> cur;
      
      void update(Edge<G>& out){
        out = Edge<G>(*i);
      }
  };
  
  
}}// proc

#endif
