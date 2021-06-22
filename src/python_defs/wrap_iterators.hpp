#ifndef _NCTX_PY_ITR_
#define _NCTX_PY_ITR_

#include "../pythonmodule.hpp"
#include "../python_cpp/iterator_edges.hpp"
#include "../python_cpp/iterator_vertices.hpp"
#include "../python_cpp/iterator_adjacent.hpp"

namespace nctx { namespace python {
  
  inline py::object pass_through(py::object const& o) { return o; }
  
  template <bool directed>
  inline void wrap_iterators(){
    //~ boost::mpl::for_each<graph_types::types, boost::mpl::make_identity<boost::mpl::_1>>([](auto arg) { 
        //~ using GraphType = typename decltype(arg)::type;
    using G = typename GraphContainer<directed>::Graph;
    using G_rev = typename GraphContainer<directed>::reverse;
    //~ typedef typename b::graph_traits<G>::edge_descriptor edge;
    //~ using E = Edge<G>;
    
    std::string child_doc = std::string("See :py:func:`Graph.children <nctx.") + (directed ? std::string("directed") : std::string("undirected")) + std::string(".Graph.children>` how to use this to iterate over the children of a node.");
    std::string parents_doc = std::string("See :py:func:`Graph.parents <nctx.") + (directed ? std::string("directed") : std::string("undirected")) + std::string(".Graph.parents>` how to use this to iterate over the parents of a node.");
    std::string vertices_doc = std::string("See :py:func:`Graph.vertices <nctx.") + (directed ? std::string("directed") : std::string("undirected")) + std::string(".Graph.vertices>` how to use this to iterate over the vertices of a graph.");
    std::string edges_doc = std::string("See :py:func:`Graph.edges <nctx.") + (directed ? std::string("directed") : std::string("undirected")) + std::string(".Graph.edges>` how to use this to iterate over the edges of a graph.");
      
    py::class_<a_iterator<size_t,G>>("a_iter_ids", "This is an internal description of the child-iterator.", py::no_init)
      .def("next", &a_iterator<size_t,G>::next, child_doc.c_str())
      .def("__next__", &a_iterator<size_t,G>::next)
      //~ .def("__reversed__", &iterator_py<size_t, ctx_type, v_iterator<size_t, ctx_type>>::previous)
      .def("__iter__", pass_through)
      .def("__len__", &a_iterator<size_t,G>::get_size);
      
    py::class_<a_iterator<size_t,G_rev>>("a_iter_ids_rev", "This is an internal description of the parents-iterator.", py::no_init)
      .def("next", &a_iterator<size_t,G_rev>::next, parents_doc.c_str())
      .def("__next__", &a_iterator<size_t,G_rev>::next)
      //~ .def("__reversed__", &iterator_py<size_t, ctx_type, v_iterator<size_t, ctx_type>>::previous)
      .def("__iter__", pass_through)
      .def("__len__", &a_iterator<size_t,G_rev>::get_size);
    
    py::class_<v_iterator<size_t,G>>("v_iter_ids", "This is an internal description of the vertex-iterator.", py::no_init)
      .def("next", &v_iterator<size_t,G>::next, vertices_doc.c_str())
      .def("__next__", &v_iterator<size_t,G>::next)
      //~ .def("__reversed__", &iterator_py<size_t, ctx_type, v_iterator<size_t, ctx_type>>::previous)
      .def("__iter__", pass_through)
      .def("__len__", &v_iterator<size_t,G>::get_size);
    
    
    py::class_<Edge<G>>("Edge", "This is an internal description of an edge. It can't be instantiated from Python. Just don't bother with it.", py::no_init);
    
    py::class_<e_iterator<G>>("e_iter_ids", "This is an internal description of the edges-iterator.", py::no_init)
      .def("next", &e_iterator<G>::next, edges_doc.c_str())
      .def("__next__", &e_iterator<G>::next)
      .def("__iter__", pass_through)
      .def("__len__", &e_iterator<G>::get_size);
      
    py::class_<e_iterator<G, typename b::graph_traits<G>::in_edge_iterator>>("e_iter_ids_in", "This is an internal description of the in-edges-iterator.", py::no_init)
      .def("next", &e_iterator<G, typename b::graph_traits<G>::in_edge_iterator>::next, edges_doc.c_str())
      .def("__next__", &e_iterator<G, typename b::graph_traits<G>::in_edge_iterator>::next)
      .def("__iter__", pass_through)
      .def("__len__", &e_iterator<G, typename b::graph_traits<G>::in_edge_iterator>::get_size);
      
    py::class_<e_iterator<G, typename b::graph_traits<G>::out_edge_iterator>>("e_iter_ids_out", "This is an internal description of the out-edges-iterator.", py::no_init)
      .def("next", &e_iterator<G, typename b::graph_traits<G>::out_edge_iterator>::next, edges_doc.c_str())
      .def("__next__", &e_iterator<G, typename b::graph_traits<G>::out_edge_iterator>::next)
      .def("__iter__", pass_through)
      .def("__len__", &e_iterator<G, typename b::graph_traits<G>::out_edge_iterator>::get_size);
    
  }
  
}} //nproc::python

#endif
