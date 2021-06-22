#ifndef _GIOHLP_H_
#define _GIOHLP_H_

#include <stdlib.h>
#include <iostream>
#include <iomanip>

namespace std{
   // https://stackoverflow.com/questions/49156829/write-read-vector-of-item-in-xml-using-boostwrite-graphml
   // https://stackoverflow.com/questions/40685196/read-boost-graph-boostread-graphviz-where-vertex-contains-vector/40720868#40720868
  struct Lit { char ch; };
  static inline std::istream& operator>>(std::istream& is, Lit expected) {
    char actual;
    if (is >> actual && actual != expected.ch)
      is.setstate(std::ios::failbit);
    return is;
  }
    
  std::ostream& operator<<(std::ostream& os, const std::vector<double_t>& v) {
    for(auto& elem : v)
      os << elem << ';';
    return os;
  }
  std::istream& operator>>(std::istream& is, std::vector<double_t>& v) {
    double_t elem;
    while (is >> std::skipws >> elem) {
      if (is >> Lit{';'})
        v.push_back(elem);
      else
        return is; // error here is bad
    }
    if (!is.bad()) // just eof this point is ok
      is.clear();
    return is;
  }
    
  std::ostream& operator<<(std::ostream& os, const std::vector<int>& v) {
    for(auto& elem : v)
      os << elem << ';';
    return os;
  }
  std::istream& operator>>(std::istream& is, std::vector<int>& v) {
    int elem;
    while (is >> std::skipws >> elem) {
      if (is >> Lit{';'})
        v.push_back(elem);
      else
        return is; // error here is bad
    }
    if (!is.bad()) // just eof this point is ok
      is.clear();
    return is;
  }
    
  std::ostream& operator<<(std::ostream& os, const std::vector<size_t>& v) {
    for(auto& elem : v)
      os << elem << ';';
    return os;
  }
  std::istream& operator>>(std::istream& is, std::vector<size_t>& v) {
    size_t elem;
    while (is >> std::skipws >> elem) {
      if (is >> Lit{';'})
        v.push_back(elem);
      else
        return is; // error here is bad
    }
    if (!is.bad()) // just eof this point is ok
      is.clear();
    return is;
  }
    
  std::ostream& operator<<(std::ostream& os, const std::vector<std::string>& v) {
    for(auto& elem : v)
      os << elem << ';';
    return os;
  }
  std::istream& operator>>(std::istream& is, std::vector<std::string>& v) {
    std::string elem;
    while (is >> std::skipws >> elem) {
      if (is >> Lit{';'})
        v.push_back(elem);
      else
        return is; // error here is bad
    }
    if (!is.bad()) // just eof this point is ok
      is.clear();
    return is;
  }
}

#endif
