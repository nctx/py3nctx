#ifndef _NCTX_PY_LMBD_VSTR
#define _NCTX_PY_LMBD_VSTR
  // https://www.meetingcpp.com/blog/items/boostvariant-and-a-general-generic-visitor-class.html
  #include <boost/variant.hpp>
  #include <type_traits>

  template<class Result, class Func>
  struct forwarding_visitor : boost::static_visitor<Result>
  {
      Func func;
      forwarding_visitor(const Func& f):func(f){}
      forwarding_visitor(Func&& f):func(std::move(f)){}
      template<class Arg>
      Result operator()(Arg && arg) const {
          return func(std::forward<Arg>(arg));
      }
  };

  template<class Result, class Func>
  forwarding_visitor<Result, std::decay_t<Func> > make_forwarding_visitor(Func && func) {
      return {std::forward<Func>(func)};
  }

#endif
