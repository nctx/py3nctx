#ifndef _NCTX_PY_DISTANCEFCTS_
#define _NCTX_PY_DISTANCEFCTS_

#include <boost/python/stl_iterator.hpp>

namespace nctx { namespace python {

  template< typename T >
  inline
  std::vector< T > to_std_vector( const py::object& iterable )
  {
      return std::vector< T >( py::stl_input_iterator< T >( iterable ),
                               py::stl_input_iterator< T >( ) );
  }

  inline void wrap_distances(){
    py::def("kl_divergence", +[](py::object& list1, py::object& list2){
      auto l1 = to_std_vector<double_t>(list1);
      auto l2 = to_std_vector<double_t>(list2);
      return kl_divergence(l1.begin(), l1.end(), l2.begin(), l2.end());
    }, (py::arg("list1"), py::arg("list2")),"Obtain Kullback Leibler Divergence\n\n\n\nA straightforward implementation of KL Divergence. Note that both lists must be probability distributions, i.e. they sum to one. This is not checked internally and will result in obscure results if not taken care of.\n\nThe KL Divergence does not fulfil triangle inequality and, hence, is no metric. See https://en.wikipedia.org/wiki/Kullback%E2%80%93Leibler_divergence for more information.\nBoth lists feeded into this function must have the same length and contain only numeric values.\n\n\n\n\nArgs:\n    list1 (list): First list\n    list2 (list): Second list\n\nReturns:\n    double: KL Divergence between the two lists\n\nExample:\n    >>> import numpy as np\n    >>> list1 = np.abs(context_map[0])\n    >>> list2 = [.1,.2,.3,.4,.5]\n    >>> list1 = list1/np.sum(list1)\n    >>> list2 = list2/np.sum(list2)\n    >>> kl_divergence(list1, list2)\n    0.38236213356053317\n\n");

    py::def("js_divergence", +[](py::object& list1, py::object& list2){
      auto l1 = to_std_vector<double_t>(list1);
      auto l2 = to_std_vector<double_t>(list2);
      return js_divergence(l1.begin(), l1.end(), l2.begin(), l2.end());
    }, (py::arg("list1"), py::arg("list2")),"Obtain Jenson Shannon Divergence\n\n\n\nA straightforward implementation of JS Divergence. Note that both lists must be probability distributions, i.e. they sum to one. This is not checked internally and will result in obscure results if not taken care of.\n\nThe JS Divergence is based on the KL Divergence. Its square root yields a metric. See https://en.wikipedia.org/wiki/Jensen%E2%80%93Shannon_divergence for more information.\nBoth lists feeded into this function must have the same length and contain only numeric values.\n\n\n\n\nArgs:\n    list1 (list): First list\n    list2 (list): Second list\n\nReturns:\n    double: JS Divergence between the two lists\n\nExample:\n    >>> import numpy as np\n    >>> list1 = np.abs(context_map[0])\n    >>> list2 = [.1,.2,.3,.4,.5]\n    >>> list1 = list1/np.sum(list1)\n    >>> list2 = list2/np.sum(list2)\n    >>> js_divergence(list1, list2)\n    0.11794226754027952\n\n");

    py::def("euclidean_distance", +[](py::object& list1, py::object& list2){
      auto l1 = to_std_vector<double_t>(list1);
      auto l2 = to_std_vector<double_t>(list2);
      return euclidean_distance(l1.begin(), l1.end(), l2.begin(), l2.end());
    }, (py::arg("list1"), py::arg("list2")),"Obtain Euclidean Distance\n\n\n\nA straightforward implementation of Euclidean Distance.\n\nSee https://en.wikipedia.org/wiki/Euclidean_distance for more information.\nBoth lists feeded into this function must have the same length and contain only numeric values.\n\n\n\n\nArgs:\n    list1 (list): First list\n    list2 (list): Second list\n\nReturns:\n    double: Euclidean Distance between the two lists\n\nExample:\n    >>> list1 = context_map.get_list(0)\n    >>> list2 = [.1,.2,.3,.4,.5]\n    >>> euclidean_distance(list1, list2)\n    0.4063153090787592\n\n");

    py::def("cosine_similarity", +[](py::object& list1, py::object& list2){
      auto l1 = to_std_vector<double_t>(list1);
      auto l2 = to_std_vector<double_t>(list2);
      return cosine_similarity(l1.begin(), l1.end(), l2.begin(), l2.end());
    }, (py::arg("list1"), py::arg("list2")),"Obtain Cosine Similarity\n\n\n\nA straightforward implementation of Cosine Similarity.\n\nSee https://en.wikipedia.org/wiki/Cosine_similarity for more information.\nBoth lists feeded into this function must have the same length and contain only numeric values.\n\n\n\n\nArgs:\n    list1 (list): First list\n    list2 (list): Second list\n\nReturns:\n    double: Cosine Similarity between the two lists\n\nExample:\n    >>> list1 = context_map.get_list(0)\n    >>> list2 = [.1,.2,.3,.4,.5]\n    >>> cosine_similarity(list1, list2)\n    0.8368374907292926\n\n");

    py::def("angular_distance", +[](py::object& list1, py::object& list2){
      auto l1 = to_std_vector<double_t>(list1);
      auto l2 = to_std_vector<double_t>(list2);
      return angular_distance(l1.begin(), l1.end(), l2.begin(), l2.end());
    }, (py::arg("list1"), py::arg("list2")),"Obtain Angular Distance\n\n\n\nA straightforward implementation of Angular Distance.\n\nThe Angular Distance is a metric based on the KL Divergence. See https://en.wikipedia.org/wiki/Cosine_similarity#Angular_distance_and_similarity for more information.\nBoth lists feeded into this function must have the same length and contain only numeric values.\n\n\n\n\nArgs:\n    list1 (list): First list\n    list2 (list): Second list\n\nReturns:\n    double: Angular Distance between the two lists\n\nExample:\n    >>> list1 = context_map.get_list(0)\n    >>> list2 = [.1,.2,.3,.4,.5]\n    >>> angular_distance(list1, list2)\n    0.18440190063093045\n\n");

  }


}} //nproc::python

#endif