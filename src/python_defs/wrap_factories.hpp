#ifndef _NCTX_PY_FAC_
#define _NCTX_PY_FAC_

#include "../pythonmodule.hpp"
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/mpl/at.hpp>

namespace nctx { namespace python {
  
  template<bool directed>
  inline void wrap_factories(){
    
    using G = GraphContainer<directed>;
    
    std::string directed_word = directed ? std::string("directed") : std::string("undirected");
    std::string directed_article = directed ? std::string("a ") : std::string("an ");
    std::string graph_doc = std::string("This class represents ") + directed_article + directed_word + std::string(" graph.\n\nExample:\n    >>> import nctx.") + directed_word + std::string(" as nctx\n    >>> graph = nctx.Graph()");
    
    std::string get_children_doc = std::string("Iterate over the children of a given vertex u.") + 
        (directed ? std::string(" In the case of a directed graph, :py:func:`children <nctx.directed.Graph.children>` provides access to the ancestors of u. ") : std::string(" Since there is no difference between children and parents in an undirected graph, :py:func:`children <nctx.undirected.Graph.children>` yields the same list of vertices as :py:func:`parents <nctx.undirected.Graph.parents>`")) + 
        std::string("\n\nArgs:\n    u (int): ID of the vertex whose children should be iterated.\n\nReturns:\n    v_iter_ids : A vertex iterator\n\nExample:\n    >>> some_vertex = 0\n    >>> for c in g.children(some_vertex):\n    ...   pass");
    
    std::string get_parent_doc = std::string("Iterate over the parents of a given vertex u.") + 
        (directed ? std::string(" In the case of a directed graph, :py:func:`parents <nctx.directed.Graph.parents>` provides access to the predecessors of u. ") : std::string(" Since there is no difference between children and parents in an undirected graph, :py:func:`parents <nctx.undirected.Graph.parents>` yields the same list of vertices as :py:func:`children <nctx.undirected.Graph.children>`")) + 
        std::string("\n\nArgs:\n    u (int): ID of the vertex whose parents should be iterated.\n\nReturns:\n    v_iter_ids : A vertex iterator\n\nExample:\n    >>> some_vertex = 0\n    >>> for p in g.parents(some_vertex):\n    ...   pass");
        
    std::string edges_doc = std::string("Iterate over the edges of the graph. Use :py:func:`source <nctx.")+directed_word+std::string(".Graph.source>` to access the source vertex of an edge and :py:func:`target <nctx.")+directed_word+std::string(".Graph.target>` to access its target.\n")+(directed ? std::string("To handle incoming and outgoing edges separately, use :py:func:`in_edges <nctx.")+directed_word+std::string(".Graph.in_edges>` and :py:func:`out_edges <nctx.")+directed_word+std::string(".Graph.out_edges>`."):std::string("Note that incoming and outgoing edges are not distinguished in an undirected graph, therefore :py:func:`in_edges <nctx.")+directed_word+std::string(".Graph.in_edges>`, :py:func:`out_edges <nctx.")+directed_word+std::string(".Graph.out_edges>`, and this function all yield the same results."))+ std::string("\n\nReturns:\n    e_iter_ids : The edge iterator\n\nExample:\n    >>> for e in g.edges():\n    ...   source_vertex = g.source(e)\n    ...   target_vertex = g.target(e)");
        
    std::string in_edges_doc = std::string("Iterate over the incoming edges of a vertex. Use :py:func:`source <nctx.")+directed_word+std::string(".Graph.source>` to access the source vertex of an edge.\n\nArgs:\n    v (int): ID of the vertex whose in-edges should be iterated.\n\nReturns:\n    e_iter_ids_in : The in-edge iterator\n\nExample:\n    >>> v=4\n    >>> for e in g.in_edges(v):\n    ...   source_vertex = g.source(e)");
        
    std::string out_edges_doc = std::string("Iterate over the outgoing edges of a vertex. Use :py:func:`target <nctx.")+directed_word+std::string(".Graph.target>` to access the target vertex of an edge.\n\nArgs:\n    v (int): ID of the vertex whose out-edges should be iterated.\n\nReturns:\n    e_iter_ids_out : The out-edge iterator\n\nExample:\n    >>> v=4\n    >>> for e in g.in_edges(v):\n    ...   target_vertex = g.target(e)");
    
    std::string source_doc = std::string("Retrieve the source vertex of an edge. See :py:func:`edges <nctx.")+directed_word+std::string(".Graph.edges>` for an example.\n\nArgs:\n    e (Edge): An edge.\n\nReturns:\n    int : The ID of the source vertex");
    
    std::string target_doc = std::string("Retrieve the target vertex of an edge. See :py:func:`edges <nctx.")+directed_word+std::string(".Graph.edges>` for an example.\n\nArgs:\n    e (Edge): An edge.\n\nReturns:\n    int : The ID of the target vertex");
    
    py::class_<G>("Graph", graph_doc.c_str(), py::init<>())
      .def("numVertices", &G::num_vertices, "Retrieve the number of vertices in the graph.\n\nReturns:\n    int : The number of vertices")
      .def("numEdges", &G::num_edges, "Retrieve the number of edges in the graph.\n\nReturns:\n    int : The number of edges")
      .def("edges", &G::get_edge_iterator, edges_doc.c_str())
      .def("in_edges", &G::get_inedge_iterator, in_edges_doc.c_str())
      .def("out_edges", &G::get_outedge_iterator, out_edges_doc.c_str())
      .def("vertices", &G::get_vertex_iterator, "Iterate over the vertices of the graph.\n\nReturns:\n    v_iter_ids : A vertex iterator\n\nExample:\n    >>> for v in g.vertices():\n    ...   pass")
      .def("children", &G::get_children_iterator, (py::arg("u")), get_children_doc.c_str())
      .def("parents", &G::get_parent_iterator, (py::arg("u")), get_parent_doc.c_str())
      .def("source", &G::source, (py::arg("e")), source_doc.c_str())
      .def("target", &G::target, (py::arg("e")), target_doc.c_str())
      .def("add_vertex", &G::add_vertex, "Add a vertex to the graph.\n\nReturns:\n    int : The ID of the added vertex.\n\nExample:\n    >>> from collections import defaultdict\n    >>> name_map = nctx.PropertyMapStr(graph, 'name')\n    >>> vertices = defaultdict(lambda: graph.add_vertex())\n    >>> for i in range(10):\n    ...   v = vertices[i]\n    ...   name_map.set_elem(v, f\"vertex {i}\")")
      .def("add_edge", &G::add_edge, (py::arg("u"), py::arg("v")), "Add an edge that connects u and v.\n\nReturns:\n    bool : Status if adding the edge was successful.\n\nExample:\n    >>> from collections import defaultdict\n    >>> vertices = defaultdict(lambda: graph.add_vertex())\n    >>> for i in range(10):\n    ...   _ = vertices[i]\n    >>> graph.add_edge(vertices[0], vertices[1])\n    True")
      .def("__hash__", Hash<G>);
    
      
  }
  
  
}} //nproc::python

#endif
