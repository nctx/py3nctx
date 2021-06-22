#ifndef _LMBDWR_H
#define _LMBDWR_H

namespace nctx { namespace python {
  template <typename ReturnType, typename... Ts>
  struct lambda_wrapper_t{
    lambda_wrapper_t( py::object callable ) : _callable(callable) {}
    ReturnType operator()(Ts... args){
      return py::extract<ReturnType>(_callable(args...));
    }
    py::object _callable;
  };
}}

#endif
