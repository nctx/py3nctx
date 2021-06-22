#ifndef _NCTX_PY_PTHS_
#define _NCTX_PY_PTHS_
//~ #include <boost/graph/property_maps/constant_property_map.hpp>
//~ #include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/dijkstra_shortest_paths_no_color_map.hpp>

namespace nctx { namespace python {
  template<bool d>
  class SpDummy{};

  template <bool directed>
  inline py::list build_path( typename GraphContainer<directed>::vertex start,
                          typename GraphContainer<directed>::vertex goal,
                          PropertyMapValueHolder<size_t,directed>& predecessors,
                          size_t pred_undef){
    using GraphC = GraphContainer<directed>;
    using Vertex = typename GraphC::vertex;
    py::list path;
    // extract path
    Vertex current = goal;
    //~ std::cout << "extract path: " << start << " -> " << goal << std::endl;
    do {
        auto const pred = predecessors.get_elem(current);

        //~ std::cout << "extract path: " << current << " <- " << pred << " path length now " << py::len(path) << std::endl;

        if(pred == pred_undef || current == pred)
            break;

        current = pred;

        path.append(current);
    } while(current != start);

    if(py::len(path) > 1){
      path.reverse();
      path.append(goal);
    }
    return path;
  }

  template <bool directed>
  inline void wrap_pathalgs(){
    using PMap = PropertyMapValueHolder<size_t,directed>;
    using PMatrix = PropertyMapVectorHolder<size_t, directed>;
    using GraphC = GraphContainer<directed>;
    using Vertex = typename GraphC::vertex;
    using Edge = typename GraphC::edge;

    std::string apsp_doc = std::string("Obtain all-pairs-shortest-paths using Dijkstra's algorithm taking into account contextual constraints.\n\nThis is basically a wrapper around :py:func:`dijkstra_ctx <nctx.") + (directed ? std::string(""):std::string("un")) + std::string("directed.AlgPaths.dijkstra_ctx>`. Please see the mentioned function for more information regarding the contextual constraints.\n\nArgs:\n    g (Graph): The Graph object\n    decision_fct (function): The decision function enforcing contextual constraints. The signature of the function is ``(vertex index, vertex index, vertex index) -> Bool``\n     distances (PropertyMapVecULong): matrix in which distances will be written\n\nExample:\n    >>> distmap = PropertyMapVecULong(g, 'distances_ctx')\n    >>> AlgPaths.dijkstra_apsp_ctx(g,lambda _start,_current,_next: (True), distmap)\n\n\n");
    std::string apsp_ret_doc = std::string("Obtain all-pairs-shortest-paths using Dijkstra's algorithm taking into account contextual constraints.\n\nThis is basically a wrapper around :py:func:`dijkstra_ctx <nctx.") + (directed ? std::string(""):std::string("un")) + std::string("directed.AlgPaths.dijkstra_ctx>`. Please see the mentioned function for more information regarding the contextual constraints.\n\nArgs:\n    g (Graph): The Graph object\n    decision_fct (function): The decision function enforcing contextual constraints. The signature of the function is ``(vertex index, vertex index, vertex index) -> Bool``\n\nReturns:\n    PropertyMapVecULong: matrix of distances named apsp_constraint\n\nExample:\n    >>> distmap = AlgPaths.dijkstra_apsp_ctx(g,lambda _start,_current,_next: (True))\n\n\n");

    std::string fpath_doc = std::string("Obtain a shortest paths from a start vertex to a target vertex using Dijkstra's algorithm taking into account contextual constraints.\n\nThis is basically a wrapper around :py:func:`dijkstra_ctx <nctx.") + (directed ? std::string(""):std::string("un")) + std::string("directed.AlgPaths.dijkstra_ctx>`. Please see the mentioned function for more information regarding the contextual constraints.\n\nArgs:\n    g (Graph): The Graph object\n    decision_fct (function): The decision function enforcing contextual constraints. The signature of the function is ``(vertex index, vertex index, vertex index) -> Bool``\n     start (int): Start vertex\n    target (int): Target vertex\n\nReturns:\n    list: A list of vertex indices describing the shortest path from start to target.\n\nExample:\n    >>> start = 3\n    >>> target = 10\n    >>> path = AlgPaths.find_path_ctx(g,start,target,lambda _start,_current,_next: (True))\n\n\n");

    py::class_<SpDummy<directed>>("AlgPaths", py::no_init)
      .def("dijkstra_ctx", +[](GraphC& gc, Vertex start, py::object fct_decision, PMap& distances) {
        std::function<bool (Vertex, Vertex, Vertex)> lmbd_decision = lambda_wrapper_t<bool, Vertex, Vertex, Vertex>( fct_decision );
        const size_t dist_inf = (std::numeric_limits<size_t>::max)();
        auto g = gc.get_graph();
        auto weight_map = boost::make_constant_property<Edge>(1.0);
        auto dummy_pred = b::dummy_property_map();
        dijkstra_ctx_dispatch(g, start,
                              b::weight_map(weight_map)
                              .distance_map(distances.get_map()),
                              distances.get_map(),
                              weight_map,
                              dummy_pred,
                              0,
                              lmbd_decision,
                              dist_inf);
      },(py::arg("g"), py::arg("s"), py::arg("decision_fct"), py::arg("distances")), "Obtain shortest paths with dynamic contextual constraints\n\n\n\nSingle-source shortest paths using Dijkstra's algorithm taking into account contextual constraints. Enforcement of constraints is the task of the given user-defined function.\n\nThe function enforcing contextual constraints is evaluated at each node during shortest path discovery. The function needs to evaluate to ``true`` or ``false`` allowing an edge to be visited or not. As parameters, the function needs to accept the starting node of path traversal, the current node, and the descending node in question. If the function returns False, the descending node is not being visited. Passing the start vertex is unnecessary here. However, the signature is the same as for the other functions for usability reasons.\n\nThe three nodes are passed as indices allowing for access of (external) attribute and other associated information.\n\nNote that the decision function is evaluated more than once during path traversal. That means, there should not happen any resource-intense computation inside this function. Also, it does not allow to keep track of the status of calculation, e.g. by calculating the visited edges or something similar.\n\nIf the decision function simply returns True all the time, the set of shortest paths is the unaltered set of shortest paths expected by the classical Dijkstra-implementation.\n\n\n\nArgs:\n    g The graph object\n    s The source vertex\n    decision_fct A function enforcing constraints. The signature of the function is ``(vertex index, vertex index, vertex index) -> bool``, i.e. the function expects three vertex IDs for the start, the current, and the next vertex. It must evaluate to ``bool``. Passing the start vertex is unnecessary here. However, the signature is the same as for the other functions for usability reasons.\n    distances (PropertyMapULong): map in which distances will be written\n\nExample:\n    >>> start = 3\n    >>> distmap = PropertyMapULong(g, 'distances_ctx')\n    >>> AlgPaths.dijkstra_ctx(g, start, lambda _start,_current,_next: (True), distmap)\n\n\n")

      .def("dijkstra_ctx", +[](GraphC& gc, Vertex start, py::object fct_decision) -> PMap {
        std::function<bool (Vertex, Vertex, Vertex)> lmbd_decision = lambda_wrapper_t<bool, Vertex, Vertex, Vertex>( fct_decision );
        const size_t dist_inf = (std::numeric_limits<size_t>::max)();
        auto g = gc.get_graph();
        PMap distances(gc, "dijkstra_constraint");
        auto weight_map = boost::make_constant_property<Edge>(1.0);
        auto dummy_pred = b::dummy_property_map();
        dijkstra_ctx_dispatch(g, start,
                              b::weight_map(weight_map)
                              .distance_map(distances.get_map()),
                              distances.get_map(),
                              weight_map,
                              dummy_pred,
                              0,
                              lmbd_decision,
                              dist_inf);
        return distances;
      },(py::arg("g"), py::arg("s"), py::arg("decision_fct")), "Obtain shortest paths with dynamic contextual constraints\n\n\n\nSingle-source shortest paths using Dijkstra's algorithm taking into account contextual constraints. Enforcement of constraints is the task of the given user-defined function.\n\nThe function enforcing contextual constraints is evaluated at each node during shortest path discovery. The function needs to evaluate to ``true`` or ``false`` allowing an edge to be visited or not. As parameters, the function needs to accept the starting node of path traversal, the current node, and the descending node in question. If the function returns False, the descending node is not being visited. Passing the start vertex is unnecessary here. However, the signature is the same as for the other functions for usability reasons.\n\nThe three nodes are passed as indices allowing for access of (external) attribute and other associated information.\n\nNote that the decision function is evaluated more than once during path traversal. That means, there should not happen any resource-intense computation inside this function. Also, it does not allow to keep track of the status of calculation, e.g. by calculating the visited edges or something similar.\n\nIf the decision function simply returns True all the time, the set of shortest paths is the unaltered set of shortest paths expected by the classical Dijkstra-implementation.\n\n\n\nArgs:\n    g The graph object\n    s The source vertex\n    decision_fct A function enforcing constraints. The signature of the function is ``(vertex index, vertex index, vertex index) -> bool``, i.e. the function expects three vertex IDs for the start, the current, and the next vertex. It must evaluate to ``bool``. Passing the start vertex is unnecessary here. However, the signature is the same as for the other functions for usability reasons.\n\n\nReturns:\n    PropertyMapULong: map containing distances named dijkstra_constraint\n\nExample:\n    >>> start = 3\n    >>> distmap = AlgPaths.dijkstra_ctx(g, start, lambda _start,_current,_next: (True))\n\n\n")

      .def("dijkstra_apsp_ctx", +[](GraphC& gc, py::object fct_decision, PMatrix& dmap) {
        std::function<bool (Vertex, Vertex, Vertex)> lmbd_decision = lambda_wrapper_t<bool, Vertex, Vertex, Vertex>( fct_decision );
        const size_t dist_inf = (std::numeric_limits<size_t>::max)();
        auto g = gc.get_graph();
        auto weight_map = boost::make_constant_property<Edge>(1.0);

        for(auto start : make_iterator_range(vertices(g))){
          std::vector<size_t> distances(b::num_vertices(g), 0);
          auto distance_map = make_iterator_property_map(distances.begin(), b::get(b::vertex_index, g));
          auto dummy_pred = b::dummy_property_map();
          dijkstra_ctx_dispatch(g, start,
                                b::weight_map(weight_map)
                                .distance_map(distance_map),
                                distance_map,
                                weight_map,
                                dummy_pred,
                                0,
                                lmbd_decision,
                                dist_inf);

          for(auto u : make_iterator_range(b::vertices(g)))
            dmap.set_elem(start,u,distances[u]);
        }
      },(py::arg("g"), py::arg("decision_fct"), py::arg("distances")), apsp_doc.c_str())

      .def("dijkstra_apsp_ctx", +[](GraphC& gc, py::object fct_decision) -> PMatrix {
        std::function<bool (Vertex, Vertex, Vertex)> lmbd_decision = lambda_wrapper_t<bool, Vertex, Vertex, Vertex>( fct_decision );
        const size_t dist_inf = (std::numeric_limits<size_t>::max)();
        auto g = gc.get_graph();
        auto weight_map = boost::make_constant_property<Edge>(1.0);
        PMatrix dmap(gc, "apsp_constraint");
        auto dummy_pred = b::dummy_property_map();

        for(auto start : make_iterator_range(vertices(g))){
          std::vector<size_t> distances(b::num_vertices(g), 0);
          auto distance_map = make_iterator_property_map(distances.begin(), b::get(b::vertex_index, g));
          dijkstra_ctx_dispatch(g, start,
                                b::weight_map(weight_map)
                                .distance_map(distance_map),
                                distance_map,
                                weight_map ,
                                dummy_pred,
                                0,
                                lmbd_decision,
                                dist_inf);

          for(auto u : make_iterator_range(b::vertices(g)))
            dmap.set_elem(start,u,distances[u]);
        }
        return dmap;
      },(py::arg("g"), py::arg("decision_fct")), apsp_ret_doc.c_str())

      .def("find_path_ctx", +[](GraphC& gc, Vertex start, Vertex goal, py::object fct_decision) -> py::list {
        std::function<bool (Vertex, Vertex, Vertex)> lmbd_decision = lambda_wrapper_t<bool, Vertex, Vertex, Vertex>( fct_decision );
        const size_t dist_inf = (std::numeric_limits<size_t>::max)();
        const int pred_init = (std::numeric_limits<int>::min)();
        auto g = gc.get_graph();
        PMap distances(gc, "dijkstra_constraint");
        PMap predecessors(gc, "predecessors_constraint", pred_init);
        auto weight_map = boost::make_constant_property<Edge>(1.0);

        dijkstra_ctx_dispatch(g, start,
                                b::weight_map(weight_map)
                                .distance_map(distances.get_map())
                                .predecessor_map(predecessors.get_map()),
                                distances.get_map(),
                                weight_map,
                                predecessors.get_map(),
                                pred_init,
                                lmbd_decision,
                                dist_inf);

        // extract path
        return build_path(start, goal, predecessors, pred_init);
      },(py::arg("g"), py::arg("start"), py::arg("target"), py::arg("decision_fct")), fpath_doc.c_str())

      .def("dijkstra", +[](GraphC& gc, Vertex start, PMap& distances) {
        auto g = gc.get_graph();
        auto weight_map = boost::make_constant_property<Edge>(1.0);
        boost::dijkstra_shortest_paths_no_color_map(g, start,
                                b::weight_map(weight_map)
                                .distance_map(distances.get_map()));
      },(py::arg("g"), py::arg("start"), py::arg("distances")), "Conventional single-source shortest paths discovery using Dijkstra's algorithm.\n\nArgs:\n    g (Graph): The Graph object\n    start (int): Source vertex\n    distances (PropertyMapULong): PropertyMap in which distances will be written.\n")

      .def("dijkstra", +[](GraphC& gc, Vertex start) -> PMap {
        auto g = gc.get_graph();
        PMap distances(gc, "dijkstra");
        auto weight_map = boost::make_constant_property<Edge>(1.0);
        boost::dijkstra_shortest_paths_no_color_map(g, start,
                                b::weight_map(weight_map)
                                .distance_map(distances.get_map()));
        return distances;
      },(py::arg("g"), py::arg("start")), "Conventional single-source shortest paths discovery using Dijkstra's algorithm.\n\nArgs:\n    g (Graph): The Graph object\n    start (int): Source vertex\n\nReturns:\n    PropertyMapULong: PropertyMap with distances.\n")

      .def("find_path", +[](GraphC& gc, Vertex start, Vertex goal) -> py::list {
        auto g = gc.get_graph();
        const int pred_init = (std::numeric_limits<int>::min)();
        PMap distances(gc, "dijkstra");
        PMap predecessors(gc, "predecessors", pred_init);
        auto weight_map = boost::make_constant_property<Edge>(1.0);

        boost::dijkstra_shortest_paths_no_color_map(g, start,
                                b::weight_map(weight_map)
                                .distance_map(distances.get_map())
                                .predecessor_map(predecessors.get_map()));


        return build_path(start, goal, predecessors, pred_init);
      }, (py::arg("g"), py::arg("start"), py::arg("target")), "Conventional shortest path discovery using Dijkstra's algorithm.\n\nArgs:\n    g (Graph): The Graph object\n    start (int): Source vertex\n    target (int): Target vertex\n\nReturns:\n    list: A list of vertex indices describing the shortest path from start to target.\n");
  }


}} //nproc::python

#endif