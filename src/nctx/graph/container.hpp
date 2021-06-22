#ifndef _NCTX_GCONT_
#define _NCTX_GCONT_

#include <boost/graph/graphml.hpp>
#include <boost/core/demangle.hpp>
using namespace boost;


template <typename Graph>
class NctxGraphContainer {
  
  // this is taken from https://stackoverflow.com/questions/55200517/boost-list-the-vertex-and-edge-attributes-specified-in-graphml
  
  public:
  
    typedef typename graph_traits<Graph>::vertex_descriptor Vertex;
    typedef typename graph_traits<Graph>::edge_descriptor Edge;
    
  protected:
    Graph g;
    dynamic_properties dp { [=](auto const&... args) { return detect_properties(args...); } };

    using Name = std::string;
    std::map<Name, std::shared_ptr<void> > _edge_properties;
    std::vector<std::string> _vertex_properties;
    std::vector<std::string> _vertex_properties_types;

    void read(std::istream& graphml) {
        ::boost::read_graphml(graphml, g, dp);
    }
    
  private:
    
    template <typename V>
    auto add_edge_property(std::string const& name) {
        auto map = std::make_shared<std::map<Edge, V> >();
        _edge_properties.emplace(name, map);
        return add_property(name, boost::make_assoc_property_map(*map));
    }

    template <typename V>
    auto add_vertex_property(std::string const& name) {
        // NOTE, if vertex_index isn't present you might want to use
        // make_assoc_property_map as with the edge properties
        auto value_type = core::demangled_name(typeid(V));
        auto res = std::find(_vertex_properties.begin(), _vertex_properties.end(), name);
        if(res == _vertex_properties.end()){
          _vertex_properties.push_back(name);
          _vertex_properties_types.push_back(value_type);
        }else{
          int i = std::distance(_vertex_properties.begin(), res);
          _vertex_properties_types[i] = value_type;
        }
        return add_property(name, boost::make_vector_property_map<V>(get(vertex_index, g)));
    }
    template<typename Map>
    auto add_property(const std::string& name, Map pmap)
    {
        boost::shared_ptr<dynamic_property_map> pm(
                boost::static_pointer_cast<dynamic_property_map>(
                    boost::make_shared<detail::dynamic_property_map_adaptor<Map> >(pmap)));
        //~ dp.insert(name, pm); // doubles everything...
        return pm;
    }


    boost::shared_ptr<dynamic_property_map> detect_properties(Name const& name, boost::any const& key, boost::any const& value) {
        auto value_type = core::demangled_name(value.type());

        if (key.type() == typeid(Graph)) {
            //~ std::cout << "Graph property detected: " << name << ", " << value_type << "\n" << std::flush;
            //dp.property(name, boost::make_vector_property_map<Graph>(identity_property_map{}));
            //return dp.lower_bound(name)->second;
        }
        else if (key.type() == typeid(Edge)) {
            //~ std::cout << "Edge property detected: " << name << ", " << value_type << "\n" << std::flush;
            if (value.type() == typeid(std::string)) {
                return add_edge_property<std::string>(name);
            } else if (value.type() == typeid(double)) {
                return add_edge_property<double>(name);
            } else if (value.type() == typeid(int)) {
                return add_edge_property<int>(name);
            //~ } else {
                //~ std::cerr << "Value type (" << value_type << ") not supported\n";
            }
        }
        else if (key.type() == typeid(Vertex)) {
            //~ std::cout << "Vertex property detected: " << name << ", " << value_type << "\n" << std::flush;
            if (value.type() == typeid(std::string)) {
                return add_vertex_property<std::string>(name);
            } else if (value.type() == typeid(double)) {
                return add_vertex_property<double>(name);
            //~ } else if (value_type.compare("long") == 0) {
                //~ return add_vertex_property<size_t>(name);
            //~ } else {
                //~ std::cerr << "Value type (" << value_type << ") not supported\n";
            } else if (value.type() == typeid(int)) {
                return add_vertex_property<int>(name);
            }
        //~ } else {
            //~ std::cout << "Unknown property (" << core::demangled_name(key.type()) << ") " << name << ", " << value_type << "\n" << std::flush;
        }
        return nullptr;
    }
};

#endif
