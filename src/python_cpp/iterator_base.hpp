#ifndef _BASE_ITRT_H
#define _BASE_ITRT_H

#include <boost/iterator/iterator_facade.hpp>
#include "../pythonmodule.hpp"

namespace nctx { namespace python{
  
  template <typename T, typename Iter, typename G>
  class base_iterator : public b::iterator_facade<base_iterator<T,Iter,G>, T const, b::random_access_traversal_tag> 
  {
    public:
      typedef base_iterator<T, Iter, G> iterator;
      
      T const* dereference() const {return cur;} //

      T next(){
        if(i == last){
          PyErr_SetString(PyExc_StopIteration, "No more data.");
          py::throw_error_already_set();
        }
        T prev(cur);
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
      
      T operator*() {return cur;} //
      
      size_t get_size(){
        return last - first;
      }
      
      bool operator==(const iterator& rhs) const {return i == rhs.i;}
      //~ bool operator!=(const base_iterator& rhs) const {return !(*this == rhs);}
      bool operator!=(const iterator& rhs) const {return i != rhs.last;}
      iterator begin(){ return *this; }
      iterator end(){ return *this; }
      
    protected:
      Iter first, i, last;
      T cur;
      
      base_iterator(){}
      
      void init(){
        i = Iter(first);
        if(i != last)
          update(cur);
      }
      
      void update(T& out){
        out = *i;
      }
  };
  
  
}}// proc

#endif
