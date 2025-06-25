#include "vector.h"

template<typename T>
Vector<T>::Vector()
    : data(nullptr), capacity(0), length(0) {}

template<typename T>
Vector<T>::~Vector() {
    for (size_t i = 0; i < length; i++) {
        data[i].~T();
    }
    operator delete[](data);
}

template<typename T>
void Vector<T>::resize() {
    size_t new_capacity = capacity == 0 ? 4 : capacity * 2;
    T* new_data = static_cast<T*>(operator new[](new_capacity * sizeof(T)));

    for (size_t i = 0; i < length; i++) {
        new (new_data + i) T(data[i]);
        data[i].~T();
    }

    operator delete[](data);
    data = new_data;
    capacity = new_capacity;
}

template<typename T>
void Vector<T>::push_back(const T& val) {
    if (length == capacity) {
        resize();
    }
    new (data + length) T(val);
    length++;
}

template<typename T>
T& Vector<T>::operator[](size_t idx) {
    return data[idx];
}

template<typename T>
size_t Vector<T>::size() const {
    return length;
}
