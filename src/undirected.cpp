#include "pythonmodule.hpp"


BOOST_PYTHON_MODULE(undirected)
{
  using namespace nctx::python;
  py::docstring_options docopt;
	docopt.enable_all();
	docopt.disable_cpp_signatures();
  
  //~ // Register iterable conversions for user type
  iterable_converter()
    .from_python< std::vector<int> >()
    .from_python< std::vector<unsigned long> >()
    .from_python< std::vector<size_t> >()
    .from_python< std::vector<double_t> >()
    .from_python< std::vector<std::string> >();
  
  //~ register_pmap_container();
  
  //~ define_pmaps();
  register_pmap_converter<false>();
  wrap<false>();
}
