#ifndef _NCTX_PY_MODULE
#define _NCTX_PY_MODULE

#include <boost/python.hpp>
#include <boost/range/sub_range.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/any.hpp>
namespace py = boost::python;
namespace b = boost;
namespace bg = boost::graph;

#include <memory> // shared_ptr
#include <iterator>
#include <variant> 
#include <boost/variant.hpp> // need both
#include <string>
#include <vector>

#include "nctx/nctx.hpp"

typedef typename b::adjacency_list< b::vecS, b::vecS, b::undirectedS, b::vertex_index_t, b::edge_index_t> gUndirected;
typedef typename b::adjacency_list< b::vecS, b::vecS, b::bidirectionalS, b::vertex_index_t, b::edge_index_t> gDirected;
typedef boost::vec_adj_list_vertex_id_map<boost::vertex_index_t, size_t> IndexMap;
#include "graphcontainertemplated.hpp"
#include "python_cpp/property_map_holders.hpp"

#include "python_defs/definitions_property_maps.hpp"

#include "python_cpp/iterable_converter.hpp"
#include "python_cpp/hasher.hpp"
#include "python_cpp/lambda_visitor.hpp"
#include "python_cpp/lambda_wrapper.hpp"

#include "python_defs/wrap_iterators.hpp"
#include "python_defs/wrap_factories.hpp"
#include "python_defs/wrap_propertymaps.hpp"
#include "python_defs/wrap_io.hpp"
#include "python_defs/wrap_distances.hpp"

#include "python_defs/wrap_algs_centralities.hpp"
#include "python_defs/wrap_algs_shortest_path.hpp"

namespace nctx {namespace python{
  template <bool directed>
  static void wrap(){
    wrap_iterators<directed>();
    wrap_factories<directed>();
    wrap_propertymaps<directed>();
    
    wrap_io<directed>();
    
    wrap_centralities<directed>();
    wrap_pathalgs<directed>();
  }
}}

#endif
