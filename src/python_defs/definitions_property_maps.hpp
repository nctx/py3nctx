#ifndef _NCTX_PY_PMAPDEFS
#define _NCTX_PY_PMAPDEFS

#include <boost/mpl/for_each.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/at.hpp>
#include "../python_cpp/is_container.hpp"

#define N_PROPERTY_MAP_TYPES_UNIQUE 8
#define N_PROPERTY_MAP_ATOM_TYPES_UNIQUE 4

namespace nctx{ namespace python{

  template <bool directed>
  struct property_map_types{
    typedef boost::variant< nctx::python::PropertyMapValueHolder<std::string, directed>,
                            nctx::python::PropertyMapValueHolder<int, directed>,
                            nctx::python::PropertyMapValueHolder<size_t, directed>,
                            nctx::python::PropertyMapValueHolder<double_t, directed>,
                            nctx::python::PropertyMapVectorHolder<std::string, directed>,
                            nctx::python::PropertyMapVectorHolder<int, directed>,
                            nctx::python::PropertyMapVectorHolder<size_t, directed>,
                            nctx::python::PropertyMapVectorHolder<double_t, directed>
                        > variants;
  };

  typedef boost::mpl::vector<std::string, 
                             int,
                             size_t,
                             double_t, 
                             std::vector<std::string>,
                             std::vector<int>,
                             std::vector<size_t>,
                             std::vector<double_t>
                             > property_map_value_types;

  typedef boost::mpl::vector<std::string, 
                             int,
                             size_t,
                             double_t, 
                             std::string,
                             int,
                             size_t,
                             double_t
                             > property_map_element_types;
                             
  static const std::string property_map_names[] = 
    {"PropertyMapStr", 
     "PropertyMapInt",
     "PropertyMapULong", 
     "PropertyMapDouble",
     "PropertyMapVecStr",
     "PropertyMapVecInt", 
     "PropertyMapVecULong", 
     "PropertyMapVecDouble"};
                             
  static const std::string property_map_type_names[] = 
    {"Str",
     "Int",
     "ULong",
     "Double",
     "VecStr", 
     "VecInt",
     "VecULong",
     "VecDouble"};


  template<bool directed>
  struct pmaptype_to_object : boost::static_visitor<PyObject *> {
    static result_type convert(typename property_map_types<directed>::variants const &v) {
      return apply_visitor(pmaptype_to_object(), v);
    }

    template<typename T>
    result_type operator()(T const &t) const {
      return boost::python::incref(boost::python::object(t).ptr());
    }
  };
  
  template <bool directed>
  inline void register_pmap_converter(){
    py::to_python_converter<typename property_map_types<directed>::variants, pmaptype_to_object<directed>>();
    boost::mpl::for_each<typename property_map_types<directed>::variants::types, boost::mpl::make_identity<boost::mpl::_1>>([](auto arg) { 
      using T = typename decltype(arg)::type;
      py::implicitly_convertible<T, typename property_map_types<directed>::variants>(); 
    });
    
  }
  
  inline void register_pmap_container(){
    boost::mpl::for_each<boost::mpl::range_c<int,0,N_PROPERTY_MAP_TYPES_UNIQUE>>([](auto t) { 
      using elem_type = typename boost::mpl::at_c<property_map_value_types, t>::type;
      std::string s = property_map_type_names[t];
      
      if(is_stl_container<elem_type>::value){
        using value_type = typename boost::mpl::at_c<property_map_element_types, t>::type;
        using cont = std::vector<value_type>;
        
        std::string doc = std::string("This PropertyMap serves as a container of a typed list. It is not meant to be instatiated from Python.");
        
        // register underlying type
        if (typeid(value_type) == typeid(std::string)){
          //~ doc += std::string(" In this case, the underlying type is a string.");
          py::class_< cont, 
                    std::shared_ptr< cont >, 
                    boost::noncopyable> (s.c_str(), doc.c_str(), py::no_init)
            .def(py::vector_indexing_suite< cont >())
            .def("clear", &cont::clear,  "Empty the property map");
        }else{
          //~ if (typeid(value_type) == typeid(int)){
            //~ doc += std::string(" In this case, the underlying type is an integer.");
          //~ }else if (typeid(value_type) == typeid(size_t)){
            //~ doc += std::string(" In this case, the underlying type is an unsigned long, i.e. a number > 0.");
          //~ }else if (typeid(value_type) == typeid(double_t)){
            //~ doc += std::string(" In this case, the underlying type is a floating point number (double).");
          //~ }
          py::class_< cont > (s.c_str(), doc.c_str(), py::no_init)
            .def(py::vector_indexing_suite< cont >())
            .def("clear", &cont::clear, "Empty the property map")
            .def("_max", &std::numeric_limits<value_type>::max, "Retrieve the highest possible value of the underlying data type. Important: This number is not neccessarily contained in the list!")
            .def("_min", &std::numeric_limits<value_type>::min, "Retrieve the lowest possible value of the underlying data type. Important: This number is not neccessarily contained in the list!");
        }
      }
    });
    
    
  }

}}

#endif
