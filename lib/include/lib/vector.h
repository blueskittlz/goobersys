#ifndef LIB_VECTOR_H
#define LIB_VECTOR_H

#include <stddef.h>
#include <new>  // for placement new

template<typename T>
class Vector {
private:
    T* data;
    size_t capacity;
    size_t length;

    void resize();

public:
    Vector();
    ~Vector();

    void push_back(const T& val);
    T& operator[](size_t idx);
    size_t size() const;
};

#include "vector.tcc"  // Implementation file for templates

#endif
