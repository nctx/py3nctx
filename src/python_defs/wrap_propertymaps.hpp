#ifndef _NCTX_PY_PMP_
#define _NCTX_PY_PMP_

#include <typeinfo>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/at.hpp>
#include "../python_cpp/is_container.hpp"

namespace nctx { namespace python {
  
  inline py::object pass_through_prop(py::object const& o) { return o; }

  template<bool directed>
  inline void wrap_propertymaps(){
    
    using G = GraphContainer<directed>;
    
    boost::mpl::for_each<boost::mpl::range_c<int,0,N_PROPERTY_MAP_TYPES_UNIQUE>>([](auto t) { 
      using elem_type = typename boost::mpl::at_c<property_map_value_types, t>::type;
      std::string s = property_map_names[t];
      
      if(is_stl_container<elem_type>::value){
        using value_type = typename boost::mpl::at_c<property_map_element_types, t>::type;
        using PMapCont = PropertyMapVectorHolder<value_type, directed>;
        
        std::string doc = s + std::string(" serves as a container of lists, i.e. it is a list of lists. The nested lists all contain values of a single data type as this is a link to the C++ code base (and C++ is a strongly typed language). This PropertyMap behaves like a python list returning lists as items when iterating over it.\n\nThe allowed data type can be recognized from the class name of the PropertyMap.");
        
        std::string e_type = std::string("int");
        std::string ex_set_list = std::string("");
        std::string ex_set_elem = std::string("");
        std::string ex_contains = std::string("");
        std::string ex_set_item = std::string("");
        std::string ex_init = std::string("");
        std::string hint_set_list = std::string("Please see :py:func:`PropertyMapVecDouble.set_list <nctx.directed.PropertyMapVecDouble.set_list>` for an example of type conversion.");
        std::string hint_set_elem = std::string("Please see :py:func:`PropertyMapVecDouble.set_elem <nctx.directed.PropertyMapVecDouble.set_elem>` for an example of type conversion.");
        if (typeid(value_type) == typeid(int)){
          doc += std::string(" In this case, the underlying type is an integer.");
          ex_set_item = std::string("Example:\n    >>> my_map[2] = [1,2,3,4,5]\n    >>> my_map[2][0] = 42");
          ex_contains = std::string("Example:\n    >>> [1,2,3] in my_map\n    False");
          ex_init = std::string("\n    >>> my_map = ") + s + std::string("(g, 'context1', 0, 10)");
        }else if (typeid(value_type) == typeid(size_t)){
          doc += std::string(" In this case, the underlying type is an unsigned long, i.e. a number > 0.");
          ex_set_item = std::string("Example:\n    >>> my_map[2] = [1,2,3,4,5]\n    >>> my_map[2][0] = 42");
          ex_contains = std::string("Example:\n    >>> [1,2,3] in my_map\n    False");
          ex_init = std::string("\n    >>> my_map = ") + s + std::string("(g, 'context1', 0, 10)");
        }else if (typeid(value_type) == typeid(double_t)){
          doc += std::string(" In this case, the underlying type is a floating point number (double).");
          ex_set_item = std::string("Example:\n    >>> my_map[2] = [.1,.2,.3,.4,.5]\n    >>> my_map[2][0] = 42.0");
          ex_contains = std::string("Example:\n    >>> [.1,.2,.3] in my_map\n    False");
          ex_init = std::string("\n    >>> my_map = ") + s + std::string("(g, 'context1', 0.0, 10)");
          e_type = std::string("float");
          ex_set_list = std::string("Example:\n    Note the type conversion (float to int) from the input list to the output of :py:func:`get_elem <nctx.directed.PropertyMapVecDouble.get_elem>`.\n\n    >>> complex_map = ") + s + std::string("(g, 'context')\n    >>> complex_map.set_list(0, [1,2,3,4,5])\n    >>> complex_map.get_elem(0,2)\n    3.0");
          ex_set_elem = std::string("Example:\n    Note the type conversion (float to int) from the input to the output of :py:func:`get_elem <nctx.directed.PropertyMapVecDouble.get_elem>`.\n\n    >>> complex_map = ") + s + std::string("(g, 'context')\n    >>> complex_map.set_elem(0, 1, 3)\n    >>> complex_map.get_elem(0,1)\n    3.0");
          hint_set_list = std::string("");
          hint_set_elem = std::string("");
        }else if (typeid(value_type) == typeid(std::string)){
          doc += std::string(" In this case, the underlying type is a string.");
          ex_set_item = std::string("Example:\n    >>> my_map[2] = ['1','2','3','4','5']\n    >>> my_map[2][0] = '42'");
          ex_contains = std::string("Example:\n    >>> ['1','2','3'] in my_map\n    False");
          ex_init = std::string("\n    >>> my_map = ") + s + std::string("(g, 'context1', '', 10)");
          e_type = std::string("str");
        }
        doc += std::string("\n\nArgs:\n    g (Graph): The Graph object\n    name (str): Name of the PropertyMap\n    init_val (")+e_type+std::string("): initial value for each inner list\n    init_size (int): size of each inner list.\n\nExample:\n    >>> my_map = ") +
               s + std::string("(g, 'context1')") + ex_init;
               
        std::string doc_get_elem = std::string("Retrieve the jth element of ith list.\n\nArgs:\n    i (int): Index of List\n    j (int): Index of element.\n\nReturns:\n    ") + e_type + std::string(": jth element in ith list.");
        std::string doc_get_list = std::string("Retrieve the ith list.\n\nArgs:\n    i (int): Index of List\n    j (int): Index of element.\n\nReturns:\n    ") + s + std::string(": The list saved at position i.");
        std::string doc_set_elem = std::string("Set the jth element of ith list. Note that the type of the new element needs to be ")+e_type+std::string(" or a type that can be implicitly converted to ")+e_type + std::string(". ")+hint_set_elem+std::string("\n\nArgs:\n    i (int): Index of List\n    j (int): Index of element.\n    newval (") + e_type + std::string("): The new element.\n\n") + ex_set_elem;
        std::string doc_set_list = std::string("Set the ith list to be newlist. ")+hint_set_elem+std::string("\n\nArgs:\n    i (int): Index of List\n    newlist (list): List to be saved at position i. Note that all items of this list will be converted to ")+e_type+std::string(" internally.\n\n") + ex_set_list;
        
        std::string get_item_doc = std::string("This allows to access items using square brackets. See :py:func:`get_list <nctx.")+(directed?std::string("directed."):std::string("undirected."))+s+std::string(".get_list>` for a detailed description of parameters.\n\nExample:\n    >>> _ = my_map[2]\n    >>> _ = my_map[2][0]");
        std::string set_item_doc = std::string("This allows to modify items using square brackets. See :py:func:`set_list <nctx.")+(directed?std::string("directed."):std::string("undirected."))+s+std::string(".set_list>` for a detailed description of parameters.\n\n") + ex_set_item;
        
        std::string get_name_doc = std::string("Retrieve name of the property map.\nThe name is mainly used when exporting the Graph to a file, e.g. using :py:func:`write_graphml <nctx.")+(directed ? std::string("directed") : std::string("undirected")) + std::string(".write_graphml>`.\n\nReturns:\n    string: The name of the list");
        
        std::string contains_doc = std::string("Allows to check for the existence of elements in the PropertyMap.\n\n") + ex_contains;
        
        py::class_< PMapCont >(s.c_str(), doc.c_str(), py::init<G&, const std::string&>())
          .def(py::init<G&, const std::string&, value_type, size_t>())
          .def("get_name", &PMapCont::get_name, get_name_doc.c_str())
          .def("get_elem", &PMapCont::get_elem, (py::arg("i"), py::arg("j")), doc_get_elem.c_str())
          .def("set_elem", &PMapCont::set_elem, (py::arg("i"), py::arg("j"), py::arg("newval")), doc_set_elem.c_str())
          .def("get_list", &PMapCont::get_list, py::return_internal_reference<>(), (py::arg("i")), doc_get_list.c_str())
          .def("set_list", &PMapCont::set_list, (py::arg("i"), py::arg("newlist")), doc_set_list.c_str())
          .def("__getitem__", &PMapCont::get_list, py::return_internal_reference<>(), get_item_doc.c_str())
          .def("__setitem__", &PMapCont::set_list, set_item_doc.c_str())
          .def("__iter__", pass_through_prop)
          .def("__contains__", &PMapCont::contains, contains_doc.c_str())
          .def("next", &PMapCont::next, py::return_internal_reference<>(), "Allows to iterater over the PropertyMap.\n\nExample:\n    >>> for ctx in my_map:\n    ...   pass")
          .def("__next__", &PMapCont::next, py::return_internal_reference<>(), "Allows to iterater over the PropertyMap.\n\nExample:\n    >>> for ctx in my_map:\n    ...   pass")
          .def("__len__",  &PMapCont::get_size, "Allows to retrieve the length of the PropertyMap.\n\nExample:\n    >>> len(my_map)\n    10");
          
      }else{
        std::string doc = s + std::string(" serves as a list of atom items. The list contains values of a single data type only as this is a link to the C++ code base (and C++ is a strongly typed language). This PropertyMap behaves like a python list, i.e. it allows iteration and supports built-in functions append and extend.\n\nThe contained data type can be recognized from the class name of the PropertyMap.");
        
        std::string e_type = std::string("int");
        std::string ex_set_elem = std::string("");
        std::string hint_set_elem = std::string("Please see :py:func:`PropertyMapDouble.set_elem <nctx.directed.PropertyMapDouble.set_elem>` for an example of type conversion.");
        std::string ex_set_item = std::string("");
        std::string ex_contains = std::string("");
        std::string ex_init = std::string("");
        if (typeid(elem_type) == typeid(int)){
          doc += std::string(" In this case, the underlying type is an integer.");
          ex_set_item = std::string("Example:\n    >>> my_map[2] = 1");
          ex_contains = std::string("Example:\n    >>> 1 in my_map\n    False");
          ex_init = std::string("\n    >>> my_map = ") + s + std::string("(g, 'context1', 0)");
        }else if (typeid(elem_type) == typeid(size_t)){
          doc += std::string(" In this case, the underlying type is an unsigned long, i.e. a number > 0.");
          ex_set_item = std::string("Example:\n    >>> my_map[2] = 1");
          ex_contains = std::string("Example:\n    >>> 1 in my_map\n    False");
          ex_init = std::string("\n    >>> my_map = ") + s + std::string("(g, 'context1', 0)");
        }else if (typeid(elem_type) == typeid(double_t)){
          doc += std::string(" In this case, the underlying type is a floating point number (double).");
          ex_set_item = std::string("Example:\n    >>> my_map[2] = .1");
          ex_contains = std::string("Example:\n    >>> .1 in my_map\n    False");
          e_type = std::string("float");
          ex_set_elem = std::string("Example:\n    Note the type conversion (float to int) from the input list to the output of :py:func:`get_elem <nctx.directed.PropertyMapDouble.get_elem>`.\n\n    >>> complex_map = ") + s + std::string("(g, 'context')\n    >>> complex_map.set_elem(0, 3)\n    >>> complex_map.get_elem(0)\n    3.0");
          hint_set_elem = std::string("");
          ex_init = std::string("\n    >>> my_map = ") + s + std::string("(g, 'context1', .0)");
        }else if (typeid(elem_type) == typeid(std::string)){
          ex_set_item = std::string("Example:\n    >>> my_map[2] = '1'");
          ex_contains = std::string("Example:\n    >>> '1' in my_map\n    False");
          ex_init = std::string("\n    >>> my_map = ") + s + std::string("(g, 'context1', '')");
        }
        doc += std::string("\n\nArgs:\n    g (Graph): The Graph object\n    name (str): Name of the PropertyMap\n    init_val (")+e_type+std::string("): initial value\n\nExample:\n    >>> my_map = ") +
               s + std::string("(g, 'context1')")+ex_init;
        
        std::string doc_get_elem = std::string("Retrieve the ith element of this list.n\nArgs:\n    i (int): Index of element.\n\nReturns:\n    ") + e_type + std::string(": ith element.");
        std::string doc_set_elem = std::string("Set the ith element of this list to be newval. Note that the type of newval needs to be ")+e_type+std::string(" or a type that can be implicitly converted to ")+e_type + std::string(". ")+hint_set_elem+std::string("\n\nArgs:\n    i (int): Index of List\n    j (int): Index of element.\n    newval (") + e_type + std::string("): The new element.\n\n") + ex_set_elem;
        
        
        std::string get_name_doc = std::string("Retrieve name of the property map.\n\nThe name is mainly used when exporting the Graph to a file, e.g. using :py:func:`write_graphml <nctx.")+(directed ? std::string("directed") : std::string("undirected")) + std::string(".write_graphml>`.\n\nReturns:\n    string: The name of the list");
        
        std::string get_item_doc = std::string("This allows to access items using square brackets. See :py:func:`get_elem <nctx.")+(directed?std::string("directed."):std::string("undirected."))+s+std::string(".get_elem>` for a detailed description of parameters.\n\nExample:\n    >>> _ = my_map[2]");
        std::string set_item_doc = std::string("This allows to modify items using square brackets. See :py:func:`set_elem <nctx.")+(directed?std::string("directed."):std::string("undirected."))+s+std::string(".set_elem>` for a detailed description of parameters.\n\n") + ex_set_item;
        
        std::string contains_doc = std::string("Allows to check for the existence of elements in the PropertyMap.\n\n") + ex_contains;
        
        using PMap = PropertyMapValueHolder<elem_type, directed>;
        py::class_< PMap >(s.c_str(), doc.c_str(), py::init<G&, const std::string&>())
          .def(py::init<G&, const std::string&, elem_type>())
          .def("get_name", &PMap::get_name, get_name_doc.c_str())
          .def("get_elem", &PMap::get_elem, (py::arg("i")), doc_get_elem.c_str())
          .def("set_elem", &PMap::set_elem, (py::arg("i"), py::arg("newval")), doc_set_elem.c_str())
          .def("__getitem__", &PMap::get_elem, get_item_doc.c_str())
          .def("__setitem__", &PMap::set_elem, set_item_doc.c_str())
          .def("__iter__", pass_through_prop)
          .def("__contains__", &PMap::contains, contains_doc.c_str())
          .def("next", &PMap::next, "Allows to iterater over the PropertyMap.\n\nExample:\n    >>> for ctx in my_map:\n    ...   pass")
          .def("__next__", &PMap::next, "Allows to iterater over the PropertyMap.\n\nExample:\n    >>> for ctx in my_map:\n    ...   pass")
          .def("__len__",  &PMap::get_size, "Allows to retrieve the length of the PropertyMap.\n\nExample:\n    >>> len(my_map)\n    10");
      }
    });
  }
  
  
}} //nproc::python

#endif
