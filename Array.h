#pragma once

#include <cstdio>
#include <iterator>

template<typename T>
class Array {
private:
    typedef T value_type;
    typedef value_type *pointer;
    typedef const value_type *const_pointer;
    typedef value_type &reference;
    typedef const value_type &const_reference;
    typedef value_type *iterator;
    typedef const value_type *const_iterator;

public:
    const size_t size;

    explicit Array(std::size_t size) : size(size) {
        buffer = new T[size];
    }

    reference operator[](std::size_t index) {
        return buffer[index];
    }

    const_reference operator[](std::size_t index) const {
        return buffer[index];
    }

    pointer data() {
        return buffer;
    }

    const_pointer data() const {
        return buffer;
    }

    iterator begin() {
        iterator (buffer);
    }

    const_iterator begin() const {
        iterator (buffer);
    }

    iterator end() {
        return iterator(buffer + size);
    }

    const_iterator end() const {
        return iterator(buffer + size);
    }

    virtual ~Array() {
        delete[] buffer;
    }

private:
    T *buffer;
};