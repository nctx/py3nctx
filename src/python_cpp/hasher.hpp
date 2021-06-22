#ifndef _NCTX_PY_HSHR
#define _NCTX_PY_HSHR

namespace nctx { namespace python {
  template <typename hash_me>
  static std::size_t Hash(const std::shared_ptr<hash_me>& a_ptr) {
      std::hash<std::uintptr_t> hasher;
      return hasher(reinterpret_cast<std::uintptr_t>(a_ptr.get()));
  }
}}

#endif
