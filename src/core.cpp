#include "pythonmodule.hpp"


BOOST_PYTHON_MODULE(core)
{

  using namespace nctx::python;
  py::docstring_options docopt;
	docopt.enable_all();
	docopt.disable_cpp_signatures();
  //~ enable_type_object();

  //~ py::class_<std::vector<unsigned long>>("ULongList", py::no_init)
    //~ .def(py::vector_indexing_suite<std::vector<unsigned long>>())
    //~ .def("_max", &std::numeric_limits<unsigned long>::max);
    
  //~ py::class_<std::vector<size_t>>("SizeTList", py::no_init)
    //~ .def(py::vector_indexing_suite<std::vector<size_t>>())
    //~ .def("_max", &std::numeric_limits<size_t>::max);
  
  // Register iterable conversions for user type
  iterable_converter()
    .from_python< std::vector<int> >()
    .from_python< std::vector<unsigned long> >()
    .from_python< std::vector<size_t> >()
    .from_python< std::vector<double_t> >()
    .from_python< std::vector<std::string> >();
  
  register_pmap_container();
  
  wrap_distances();
  
  //~ define_pmaps();
}
