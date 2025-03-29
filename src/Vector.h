#pragma once

#ifndef ARX_VECTOR_H
#define ARX_VECTOR_H

#ifndef ARX_VECTOR_DEFAULT_SIZE
#define ARX_VECTOR_DEFAULT_SIZE 16
#endif  // ARX_VECTOR_DEFAULT_SIZE

#include "RingBuffer.h"

namespace arx {
    namespace stdx {

        template<typename T, size_t N = ARX_VECTOR_DEFAULT_SIZE>
        struct vector : public RingBuffer<T, N> {
            using iterator = typename RingBuffer<T, N>::iterator;
            using const_iterator = typename RingBuffer<T, N>::const_iterator;

            vector() : RingBuffer<T, N>() {}

            vector(std::initializer_list<T> lst) : RingBuffer<T, N>(lst) {}

            // copy
            vector(const vector &r) : RingBuffer<T, N>(r) {}

            vector &operator=(const vector &r) {
                RingBuffer<T, N>::operator=(r);
                return *this;
            }

            // move
            vector(vector &&r) : RingBuffer<T, N>(r) {}

            vector &operator=(vector &&r) {
                RingBuffer<T, N>::operator=(r);
                return *this;
            }

        private:
            using RingBuffer<T, N>::pop;
            using RingBuffer<T, N>::pop_front;
            using RingBuffer<T, N>::push;
            using RingBuffer<T, N>::push_front;
            using RingBuffer<T, N>::emplace;
            using RingBuffer<T, N>::fill;
        };
    } // namespace arx
} // namespace stdx

#endif //ARX_VECTOR_H
