#ifndef AUTOCHECK_SEQUENCE_HPP
#define AUTOCHECK_SEQUENCE_HPP

#include "ostream.hpp"

namespace autocheck {

  template <typename T>
  std::ostream& operator<< (std::ostream& out, const std::vector<T>& seq) {
    out << "[";
    bool first = true;
    for (auto b = seq.begin(), e = seq.end(); b != e; ++b) {
      if (first) {
        first = false;
      } else {
        out << ", ";
      }
      out << *b;
    }
    out << "]";
    return out;
  }

  template <typename T>
  std::ostream& operator<< (std::ostream& out, const std::set<T>& seq) {
    out << "[";
    bool first = true;
    for (auto b = seq.begin(), e = seq.end(); b != e; ++b) {
      if (first) {
        first = false;
      } else {
        out << ", ";
      }
      out << *b;
    }
    out << "]";
    return out;
  }

  template <typename K, typename V>
  std::ostream& operator<< (std::ostream& out, const std::map<K, V>& seq) {
    out << "[";
    bool first = true;
    for (auto b = seq.begin(), e = seq.end(); b != e; ++b) {
      if (first) {
        first = false;
      } else {
        out << ", ";
      }
      out << *b;
    }
    out << "]";
    return out;
  }

  template <typename K, typename V>
  std::ostream& operator<< (std::ostream& out, const std::unordered_map<K, V>& seq) {
    out << "[";
    bool first = true;
    for (auto b = seq.begin(), e = seq.end(); b != e; ++b) {
      if (first) {
        first = false;
      } else {
        out << ", ";
      }
      out << *b;
    }
    out << "]";
    return out;
  }

}

#endif

