#ifndef _NCTX_PY_PMHLDR_
#define _NCTX_PY_PMHLDR_


namespace nctx { namespace python{
  
  template <typename T, bool directed>
  class PropertyMapHolder{
    
    public:
      typedef typename std::vector<T> cont_type;
    
    protected:
      typedef typename boost::vector_property_map<T, IndexMap > PropertyMap;
      typedef typename PropertyMap::key_type key_type;
      
    public:
      PropertyMapHolder(GraphContainer<directed> gc, const std::string& _name) : name(_name){
        pmap = b::make_vector_property_map<T>(gc.make_index_map());
      };
        
      std::string get_name(){
        return name;
      }
      
      PropertyMap& get_map(){
        return pmap;
      }
      
      size_t get_size(){
        return pmap.storage_end() - pmap.storage_begin();
      }
      
      bool contains(T find_me){
        return std::find(pmap.storage_begin(), pmap.storage_end(), find_me) != pmap.storage_end();
      }
    
    protected:
      std::string name;
      PropertyMap pmap;
  };
  
  template <typename T, bool directed> 
  class PropertyMapValueHolder : public PropertyMapHolder<T,directed>{
    
    //~ public:
      //~ typedef typename PropertyMapHolder<T,directed>::PropertyMap PropertyMap;
      //~ typedef typename PropertyMapHolder<T,directed>::PropertyMap::key_type key_type;
    
    public:
      typedef typename PropertyMapHolder<T,directed>::cont_type cont_type;
    private:
      using key_type = typename PropertyMapHolder<T,directed>::PropertyMap::key_type;
    
    public:
      PropertyMapValueHolder(GraphContainer<directed> gc, const std::string& _name) : PropertyMapHolder<T,directed>(gc,_name)
      {
        for (key_type i=0; i<gc.num_vertices(); i++)
          this->pmap[i] = T();
      };
      PropertyMapValueHolder(GraphContainer<directed> gc, const std::string& _name, T init_val) : PropertyMapHolder<T,directed>(gc,_name)
      {
        for (key_type i=0; i<gc.num_vertices(); i++)
          this->pmap[i] = init_val;
      };
      
      T get_elem(key_type v){
        return this->pmap[v];
      }
      
      void set_elem(key_type v, T newval){
        this->pmap[v] = newval;
      }
      
      T next(){
        if(i == this->get_size()){
          PyErr_SetString(PyExc_StopIteration, "No more data.");
          boost::python::throw_error_already_set();
        }
        T current(this->pmap[i]);
        i++;
        return current;
      }
      
    private:
      key_type i = 0;
  };
  
  template <typename T, bool directed> 
  class PropertyMapVectorHolder : public PropertyMapHolder<std::vector<T>,directed>{
    
    //~ public:
      //~ typedef typename PropertyMapHolder<T,directed>::PropertyMap PropertyMap;
      //~ typedef typename PropertyMapHolder<T,directed>::PropertyMap::key_type key_type;
    public:
      typedef typename PropertyMapHolder<std::vector<T>,directed>::cont_type cont_type;
    private: 
      using key_type = typename PropertyMapHolder<std::vector<T>,directed>::PropertyMap::key_type;
    
    public:
      PropertyMapVectorHolder(GraphContainer<directed> gc, const std::string& name) : PropertyMapHolder<std::vector<T>,directed>(gc,name)
      {
        for (key_type i=0; i<gc.num_vertices(); i++)
          (this->pmap[i]);
      };
      PropertyMapVectorHolder(GraphContainer<directed> gc, const std::string& name, T init_val, size_t init_size) : PropertyMapHolder<std::vector<T>,directed>(gc,name)
      {
        for (key_type i=0; i<gc.num_vertices(); i++)
          (this->pmap[i]).resize(init_size, init_val);
      };
      
      //~ void init_maps(size_t size_maps){
        //~ std::cout << "init_maps " << size_maps << std::endl;
        //~ for (size_t i=0; i < this->get_size(); i++)
          //~ (this->pmap).push_back(std::vector<T>(size_maps,0));
      //~ }
      
      inline void check(key_type i, key_type j){
        if (j >= (this->pmap[i]).size())
          (this->pmap[i]).resize(j+1, T());
      }
        
      T get_elem(key_type i, key_type j){
        check(i,j);
        return (this->pmap[i])[j];
      }
      
      void set_elem(key_type i, key_type j, T newval){
        check(i,j);
        (this->pmap[i])[j] = newval;
      }
        
      std::vector<T>& get_list(key_type i){
        return this->pmap[i];
      }
      
      void set_list(key_type i, std::vector<T> newlist){
        this->pmap[i] = newlist;
      }
      
      std::vector<T>& next(){
        if(iter == this->get_size()){
          PyErr_SetString(PyExc_StopIteration, "No more data.");
          boost::python::throw_error_already_set();
        }
        key_type current = iter;
        iter++;
        return this->pmap[current];
      }
      
    private:
      key_type iter = 0;
  };
  
}} // nproc::python
#endif
