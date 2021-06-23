#ifndef _NCTX_PY_CNTRL_
#define _NCTX_PY_CNTRL_

#include <boost/graph/betweenness_centrality.hpp>
#include <boost/graph/page_rank.hpp>
#include <boost/graph/property_maps/constant_property_map.hpp>

namespace nctx { namespace python {
  template<bool d>
  class CentralityDummy{};

  template <bool directed>
  inline void wrap_centralities(){
    using PMap = PropertyMapValueHolder<double_t,directed>;
    using GraphC = GraphContainer<directed>;
    using Vertex = typename GraphC::vertex;
    using Edge = typename GraphC::edge;

    py::class_<CentralityDummy<directed>>("AlgCentralities", py::no_init)
      //~ .def("page_rank_ctx", +[](GraphC& gc, py::object fct_decision, PMap& rank_map, double_t damping = 0.85, int n_iter = 20) {
        //~ std::function<double_t (Vertex, Vertex)> lmbd_decision = lambda_wrapper_t<double_t, Vertex, Vertex>( fct_decision );
        //~ auto g = gc.get_graph();
        //~ page_rank_ctx(
          //~ g,
          //~ rank_map.get_map(),
          //~ lmbd_decision,
          //~ boost::graph::n_iterations(n_iter),
          //~ damping);
      //~ })
      //~ .def("page_rank_ctx", +[](GraphC& gc, py::object fct_decision, double_t damping = 0.85, int n_iter = 20) -> PMap {
        //~ std::function<double_t (Vertex, Vertex)> lmbd_decision = lambda_wrapper_t<double_t, Vertex, Vertex>( fct_decision );
        //~ auto g = gc.get_graph();
        //~ PMap rank_map(gc, "pagerank_constraint");
        //~ page_rank_ctx(
          //~ g,
          //~ rank_map.get_map(),
          //~ lmbd_decision,
          //~ boost::graph::n_iterations(n_iter),
          //~ damping);
        //~ return rank_map;
      //~ })
      //~ .def("page_rank", +[](GraphC& gc, PMap& rank_map, double_t damping = 0.85, int n_iter = 20) {
        //~ auto g = gc.get_graph();
        //~ page_rank(
          //~ g,
          //~ rank_map.get_map(),
          //~ boost::graph::n_iterations(n_iter),
          //~ damping);
      //~ })
      //~ .def("page_rank", +[](GraphC& gc, double_t damping = 0.85, int n_iter = 20) -> PMap {
        //~ auto g = gc.get_graph();
        //~ PMap rank_map(gc, "pagerank");
        //~ page_rank(
          //~ g,
          //~ rank_map.get_map(),
          //~ boost::graph::n_iterations(n_iter),
          //~ damping);
        //~ return rank_map;
      //~ })
      .def("betweenness_ctx", +[](GraphC& gc, py::object fct_decision, PMap& centr_map) {
        std::function<bool (Vertex, Vertex, Vertex)> lmbd_decision = lambda_wrapper_t<bool, Vertex, Vertex, Vertex>( fct_decision );
        auto g = gc.get_graph();
        auto weight_map = boost::make_constant_property<Edge>(1.0);
        brandes_betweenness_centrality_ctx(
          g,
          centrality_map(centr_map.get_map())
          .vertex_index_map(gc.make_index_map())
          .weight_map(weight_map),
          lmbd_decision);
      },(py::arg("g"), py::arg("betw_decision_fct"), py::arg("outmap")), "Betweenness centrality with dynamic contextual constraints.\n\n\n\nUsing this function allows obtaining betweenness centrality under dynamic contextual constraints. Enforcement of constraints is the task of the given user-defined function.\n\nThe function enforcing contextual constraints is evaluated at each node during shortest path traversal. The function needs to evaluate to True or False allowing an edge to be visited or not. As parameters, the current state of the betweenness calculation is passed to the function, i.e. the starting node for which a centrality value is being calculated, the current node, and the descending node in question. If the function returns False, the descending node is not being visited.\n\nThe three nodes are passed as indices allowing for access of (external) attribute and other associated information.\n\nNote that the decision function is evaluated more than once during path traversal. That means, there should not happen any resource-intense computation inside this function. Also, it does not allow to keep track of the status of calculation, e.g. by calculating the visited edges or something similar.\n\nIf the decision function simply returns True all the time, this function results in the unaltered betweenness centrality values.\n\nObtaining betweenness centrality is based on Brandes' efficient algorithm. At the current stage, the implementation allows for single-core execution only.\n\n\n\nArgs:\n    g The graph object\n    betw_decision_fct A function enforcing constraints. The signature of the function is ``(vertex index, vertex index, vertex index) -> Bool``.\n    outmap (PropertyMapDouble): map in which centrality values will be written\n\nExample:\n    >>> outmap = PropertyMapDouble(g, 'betw_ctx')\n    >>> AlgCentralities.betweenness_ctx(g,lambda _start,_current,_next: (True),outmap)\n\n\n")
      .def("betweenness_ctx", +[](GraphC& gc, py::object fct_decision) -> PMap {
        std::function<bool (Vertex, Vertex, Vertex)> lmbd_decision = lambda_wrapper_t<bool, Vertex, Vertex, Vertex>( fct_decision );
        auto g = gc.get_graph();
        PMap centr_map(gc, "betweenness_constraint");
        auto weight_map = boost::make_constant_property<Edge>(1.0);
        brandes_betweenness_centrality_ctx(
          g,
          centrality_map(centr_map.get_map())
          .vertex_index_map(gc.make_index_map())
          .weight_map(weight_map),
          lmbd_decision);
        return centr_map;
      },(py::arg("g"), py::arg("betw_decision_fct")), "Betweenness centrality with dynamic contextual constraints.\n\n\n\nUsing this function allows obtaining betweenness centrality under dynamic contextual constraints. Enforcement of constraints is the task of the given user-defined function.\n\nThe function enforcing contextual constraints is evaluated at each node during shortest path traversal. The function needs to evaluate to True or False allowing an edge to be visited or not. As parameters, the current state of the betweenness calculation is passed to the function, i.e. the starting node for which a centrality value is being calculated, the current node, and the descending node in question. If the function returns False, the descending node is not being visited.\n\nThe three nodes are passed as indices allowing for access of (external) attribute and other associated information.\n\nNote that the decision function is evaluated more than once during path traversal. That means, there should not happen any resource-intense computation inside this function. Also, it does not allow to keep track of the status of calculation, e.g. by calculating the visited edges or something similar.\n\nIf the decision function simply returns True all the time, this function results in the unaltered betweenness centrality values.\n\nObtaining betweenness centrality is based on Brandes' efficient algorithm. At the current stage, the implementation allows for single-core execution only.\n\n\n\nArgs:\n    g The graph object\n    betw_decision_fct A function enforcing constraints. The signature of the function is ``(vertex index, vertex index, vertex index) -> Bool``.\n\n\nReturns:\n    PropertyMapDouble: list of centrality values named betweenness_constraint\n\nExample:\n    >>> outmap = AlgCentralities.betweenness_ctx(g,lambda _start,_current,_next: (True))\n\n\n")
      .def("closeness_ctx", +[](GraphC& gc, py::object fct_decision, PMap& centr_map) {
        std::function<bool (Vertex, Vertex, Vertex)> lmbd_decision = lambda_wrapper_t<bool, Vertex, Vertex, Vertex>( fct_decision );
        auto g = gc.get_graph();
        auto weight_map = boost::make_constant_property<Edge>(1);

        closeness_centrality_ctx(g,
            b::weight_map(weight_map)
            .centrality_map(centr_map.get_map())
            .vertex_index_map(gc.make_index_map()),
            lmbd_decision);
      },(py::arg("g"), py::arg("clsn_decision_fct") ,py::arg("outmap")), "Closeness centrality with dynamic contextual constraints.\n\n\n\nUsing this function allows obtaining closeness centrality under dynamic contextual constraints. Enforcement of constraints is the task of the given user-defined function.\n\nThe function enforcing contextual constraints is evaluated at each node during shortest path traversal. The function needs to evaluate to True or False allowing an edge to be visited or not. As parameters, the current state of the centrality calculation is passed to the function, i.e. the starting node for which a centrality value is being calculated, the current node, and the descending node in question. If the function returns False, the descending node is not being visited.\n\nThe three nodes are passed as indices allowing for access of (external) attribute and other associated information.\n\nNote that the decision function is evaluated more than once during path traversal. That means, there should not happen any resource-intense computation inside this function. Also, it does not allow to keep track of the status of calculation, e.g. by calculating the visited edges or something similar.\n\nIf the decision function simply returns True all the time, this function results in the unaltered betweenness centrality values.\n\n\n\nArgs:\n    g The graph object\n    clsn_decision_fct A function enforcing constraints. The signature of the function is ``(vertex index, vertex index, vertex index) -> Bool``.\n\n    outmap (PropertyMapDouble): map in which centrality values will be written\n\nExample:\n    >>> outmap = PropertyMapDouble(g, 'clsn_ctx')\n    >>> AlgCentralities.closeness_ctx(g,lambda _start,_current,_next: (True),outmap)\n\n\n")
      .def("closeness_ctx", +[](GraphC& gc, py::object fct_decision) -> PMap {
        std::function<bool (Vertex, Vertex, Vertex)> lmbd_decision = lambda_wrapper_t<bool, Vertex, Vertex, Vertex>( fct_decision );
        auto g = gc.get_graph();
        PMap centr_map(gc, "closeness_constraint");
        auto weight_map = boost::make_constant_property<Edge>(1);

        closeness_centrality_ctx(g,
            b::weight_map(weight_map)
            .centrality_map(centr_map.get_map())
            .vertex_index_map(gc.make_index_map()),
            lmbd_decision);
        return centr_map;
      },(py::arg("g"), py::arg("clsn_decision_fct")), "Closeness centrality with dynamic contextual constraints.\n\n\n\nUsing this function allows obtaining closeness centrality under dynamic contextual constraints. Enforcement of constraints is the task of the given user-defined function.\n\nThe function enforcing contextual constraints is evaluated at each node during shortest path traversal. The function needs to evaluate to True or False allowing an edge to be visited or not. As parameters, the current state of the centrality calculation is passed to the function, i.e. the starting node for which a centrality value is being calculated, the current node, and the descending node in question. If the function returns False, the descending node is not being visited.\n\nThe three nodes are passed as indices allowing for access of (external) attribute and other associated information.\n\nNote that the decision function is evaluated more than once during path traversal. That means, there should not happen any resource-intense computation inside this function. Also, it does not allow to keep track of the status of calculation, e.g. by calculating the visited edges or something similar.\n\nIf the decision function simply returns True all the time, this function results in the unaltered betweenness centrality values.\n\n\n\nArgs:\n    g The graph object\n    clsn_decision_fct A function enforcing constraints. The signature of the function is ``(vertex index, vertex index, vertex index) -> Bool``.\n\n\nReturns:\n    PropertyMapDouble: list of centrality values named closeness_constraint\n\nExample:\n    >>> outmap = AlgCentralities.closeness_ctx(g,lambda _start,_current,_next: (True))\n\n\n")
      .def("betweenness", +[](GraphC& gc, PMap& centr_map) {
        auto g = gc.get_graph();
        auto weight_map = boost::make_constant_property<Edge>(1.0);
        brandes_betweenness_centrality(
          g,
          centrality_map(centr_map.get_map())
          .vertex_index_map(gc.make_index_map())
          .weight_map(weight_map));
      }, (py::arg("g"), py::arg("outmap")), "Obtain betweenness centrality for a graph using Brandes' efficient algorithm.\n\nArgs:\n    g (Graph): Graph object\n    outmap (PropertyMapDouble): map in which centrality values will be written\n\nExample:\n    >>> outmap = PropertyMapDouble(g, 'betw')\n    >>> AlgCentralities.betweenness(g, outmap)\n\n\n")
      .def("betweenness", +[](GraphC& gc) -> PMap {
        auto g = gc.get_graph();
        PMap centr_map(gc, "betweenness");
        auto weight_map = boost::make_constant_property<Edge>(1.0);
        brandes_betweenness_centrality(
          g,
          centrality_map(centr_map.get_map())
          .vertex_index_map(gc.make_index_map())
          .weight_map(weight_map));
        return centr_map;
      }, (py::arg("g")), "Obtain betweenness centrality for a graph using Brandes' efficient algorithm.\n\nArgs:\n    g (Graph): Graph object\n\nReturns:\n    PropertyMapDouble: list of centrality values named betweenness\n\nExample:\n    >>> outmap = AlgCentralities.betweenness(g)\n\n\n")
      .def("closeness", +[](GraphC& gc) -> PMap {
        auto g = gc.get_graph();
        PMap centr_map(gc, "closeness");
        auto weight_map = boost::make_constant_property<Edge>(1);
        std::function<bool (Vertex, Vertex, Vertex)> dummy_decision = [](Vertex s, Vertex u, Vertex v){return true;};

        closeness_centrality_ctx(g,
            b::weight_map(weight_map)
            .centrality_map(centr_map.get_map())
            .vertex_index_map(gc.make_index_map()),
            dummy_decision);

        return centr_map;
      }, (py::arg("g")), "Obtain closeness centrality for a graph.\n\nArgs:\n    g (Graph): Graph object\n\nReturns:\n    PropertyMapDouble: list of centrality values named betweenness\n\nExample:\n    >>> outmap = AlgCentralities.closeness(g)\n\n\n")
      .def("closeness", +[](GraphC& gc, PMap& centr_map) {
        auto g = gc.get_graph();
        auto weight_map = boost::make_constant_property<Edge>(1);
        std::function<bool (Vertex, Vertex, Vertex)> dummy_decision = [](Vertex s, Vertex u, Vertex v){return true;};

        closeness_centrality_ctx(g,
            b::weight_map(weight_map)
            .centrality_map(centr_map.get_map())
            .vertex_index_map(gc.make_index_map()),
            dummy_decision);
      }, (py::arg("g"), py::arg("outmap")), "Obtain closeness centrality for a graph.\n\nArgs:\n    g (Graph): Graph object\n    outmap (PropertyMapDouble): map in which centrality values will be written\n\nExample:\n    >>> outmap = PropertyMapDouble(g, 'clsn')\n    >>> AlgCentralities.closeness(g, outmap)\n\n\n")
      ;
  }


}} //nproc::python

#endif