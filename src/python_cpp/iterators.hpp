#ifndef _ITRT_PY_H 
#define _ITRT_PY_H
#include <iterator>

namespace nctx { namespace python{
  
  template <typename Iter, typename T=typename Iter::value_type>
  class base_iterator : public b::iterator_facade<base_iterator<Iter>, T const, b::random_access_traversal_tag> 
  {
    public:
      typedef base_iterator<Iter> iterator;
      //~ typedef typename Iter::value_type T;
      
      T const* dereference() const {return cur;} //

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
      
      auto distance_to(iterator const &other) const {
        std::cout << "distance to " << other.cur << ": " << this->i - other.i << std::endl;
        //~ return std::distance(this->i, other.i);
        return this->i - other.i;
      }
      
      bool equal(iterator const &other) const{
        std::cout << "equal?" << std::endl;
        return this->i == other.i;
      }
      
      T operator*() {return cur;} //
      
      size_t get_size(){
        return last - first;
      }
      
      bool operator==(const iterator& rhs) const {return i == rhs.i;}
      bool operator!=(const iterator& rhs) const {return i != rhs.last;}
      iterator begin(){ return *this; }
      iterator end(){ return *this; }
      
      T next(){
        if(i == last){
          PyErr_SetString(PyExc_StopIteration, "No more data.");
          boost::python::throw_error_already_set();
        }
        T prev(cur);
        increment();
        return prev;
      }
      
    protected:
      Iter first, i, last;
      T cur;
      
      void update(unsigned long& out){
        out = *i;
      }
      
      void init(){
        i = Iter(first);
        if(i != last)
          update(cur);
          
        std::cout << *last << " - " << *first << " = " << *i << " : " << cur << std::endl;
      }
      
      
      base_iterator(){};
  };

}}

#endif
