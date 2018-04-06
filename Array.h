#pragma once

template<typename T>
class Array<T> {
    T* data;

public:
    const int length;

    Array(int length) : length(length) {
        data = new T[length];
    }

    ~Array() {
        delete[] data;
    }
};
