#ifndef _NCTX_PY_IO_
#define _NCTX_PY_IO_

#include "../pythonmodule.hpp"
#include <boost/graph/graphml.hpp>
#include <boost/graph/properties.hpp>

namespace nctx { namespace python {

  template<bool directed>
  inline void wrap_io(){
    
    std::string read_doc = std::string("Reads graph specification from a GraphML file and fills a graph object accordingly.\nYou can pass it a list of PropertyMaps to be filled. The names of the PropertyMaps must match the names in the graphml file. Graph properties that are present in the GraphML file but that do not have a matching PropertyMap in pmaps are ignored.\nSee https://en.wikipedia.org/wiki/GraphML for more infos about the file format.\n\nArgs:\n    g (Graph): The Graph object\n    filename (str): Path to a GraphML file\n    pmaps (list): A list of PropertyMaps that will be filled with the graph properties found in the file.\n\nExample:\n    >>> import nctx.")+(directed ? std::string("directed") : std::string("undirected")) + std::string(" as nctx\n    >>> g = nctx.Graph()\n    >>> name_map = nctx.PropertyMapStr(g, 'name')\n    >>> context_map = PropertyMapVecDouble(g, 'context')\n    >>> nctx.read_graphml(g, 'bcde.ctx.")+(directed ? std::string("dir") : std::string("undir")) + std::string(".graphml', [name_map, context_map])");
    
    std::string write_doc = std::string("Reads graph specification from a GraphML file and fills a graph object accordingly.\nYou can pass it a list of PropertyMaps to be filled. The names of the PropertyMaps must match the names in the graphml file. Graph properties that are present in the GraphML file but that do not have a matching PropertyMap in pmaps are ignored.\nSee https://en.wikipedia.org/wiki/GraphML for more infos about the file format.\n\nArgs:\n    g (Graph): The Graph object\n    filename (str): Path to a GraphML file\n    pmaps (list): A list of PropertyMaps that will be filled with the graph properties found in the file.\n\nExample:\n    >>> import nctx.")+(directed ? std::string("directed") : std::string("undirected")) + std::string(" as nctx\n    >>> g = nctx.Graph()\n    >>> name_map = nctx.PropertyMapStr(g, 'name')\n    >>> context_map = PropertyMapVecDouble(g, 'context')\n    >>> nctx.read_graphml(g, 'bcde.ctx.")+(directed ? std::string("dir") : std::string("undir")) + std::string(".graphml', [name_map, context_map])\n    >>> name_map[3] = 'X'\n    >>> context_map[0][2] = .1337\n    >>> nctx.write_graphml(g, 'bcde.ctx.")+(directed ? std::string("dir") : std::string("undir")) + std::string(".modified.graphml', [name_map, context_map])");
    
    py::def("read_graphml", +[](GraphContainer<directed>& gc, const std::string& infile, py::list& pmaps){
      
      b::dynamic_properties dp(boost::ignore_other_properties);
      py::ssize_t n = py::len(pmaps);
      for(py::ssize_t i=0; i<n;i++){
        
        boost::python::extract<typename property_map_types<directed>::variants> objectExtractor(pmaps[i]);
        if(objectExtractor.check()){
          boost::apply_visitor(
              [&dp](auto pmap) { dp.property(pmap.get_name(), pmap.get_map()); },
              objectExtractor()
          );
        }
      }
      
      std::ifstream gmlfile;
      gmlfile.open(infile);
      b::read_graphml(gmlfile, gc.get_graph(), dp);
      gmlfile.close();
      
    }, (py::args("g", "filename", "pmaps")), read_doc.c_str());
    
    py::def("write_graphml", +[](GraphContainer<directed>& gc, const std::string& infile, py::list& pmaps){
      
      b::dynamic_properties dp(boost::ignore_other_properties);
      py::ssize_t n = py::len(pmaps);
      for(py::ssize_t i=0; i<n;i++){
        
        boost::python::extract<typename property_map_types<directed>::variants> objectExtractor(pmaps[i]);
        if(objectExtractor.check()){
          boost::apply_visitor(
              [&dp](auto pmap) { dp.property(pmap.get_name(), pmap.get_map()); },
              objectExtractor()
          );
        }
      }
      
      std::ofstream gmlfile;
      gmlfile.open(infile);
      boost::write_graphml(gmlfile, gc.get_graph(), dp, true);
      gmlfile.close();
      
    }, (py::args("g", "filename", "pmaps")), write_doc.c_str());
    
  }
  
  
}} //nproc::python

#endif
