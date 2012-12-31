#ifndef AUTOCHECK_VALUE_HPP
#define AUTOCHECK_VALUE_HPP

#include <cstdio>

#include <autocheck/largest.hpp>

namespace autocheck {

  template <typename T>
  class value {
    private:
      char buffer[largest<T, T*>::size];
      enum {
        None,
        Static,
        Heap
      }    allocation;

    public:
      value() : allocation(None) {}

      value(const value& copy) { *this = copy; }

      value& operator= (const value& rhs) {
        if (this == &rhs) return *this;

        if (rhs.allocation == Static) {
          construct(static_cast<const T&>(rhs));
        } else if (rhs.allocation == Heap) {
          ptr(new T(static_cast<const T&>(rhs)));
        }

        return *this;
      }

      value& operator= (const T& rhs) {
        construct(rhs);
        return *this;
      }

      value& operator= (T* rhs) {
        ptr(rhs);
        return *this;
      }

      bool empty() const { return allocation == None; }

      template <typename... Args>
      void construct(const Args&... args) {
        clear();
        T* p = new (buffer) T(args...);
        allocation = Static;
      }

      const T* ptr() const {
        return (allocation == Heap)
          ? (*reinterpret_cast<const T* const *>(buffer))
          : (reinterpret_cast<const T*>(buffer));
      }

      T* ptr() {
        return const_cast<T*>(static_cast<const value*>(this)->ptr());
      }

      void ptr(T* p) {
        clear();
        (*reinterpret_cast<T**>(buffer)) = p;
        allocation = p ? Heap : None;
      }

      T* operator-> ()             { return ptr(); }
      const T* operator-> () const { return ptr(); }

      operator T& ()             { return *ptr(); }
      operator const T& () const { return *ptr(); }

      void clear() {
        if (allocation == Heap) {
          delete ptr();
        } else if (allocation == Static) {
          ptr()->~T();
        }
        allocation = None;
      }

      ~value() { clear(); }

  };

}

#endif

