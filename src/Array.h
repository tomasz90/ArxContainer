#pragma once

#ifndef ARX_ARRAY_H
#define ARX_ARRAY_H

#include "RingBuffer.h"

namespace arx {
    namespace stdx {

        template<typename T, size_t N>
        struct array : public RingBuffer<T, N> {
            using iterator = typename RingBuffer<T, N>::iterator;
            using const_iterator = typename RingBuffer<T, N>::const_iterator;

            array() : RingBuffer<T, N>() {}

            array(std::initializer_list<T> lst) : RingBuffer<T, N>(lst) {}

            // copy
            array(const array &r) : RingBuffer<T, N>(r) {}

            array &operator=(const array &r) {
                RingBuffer<T, N>::operator=(r);
                return *this;
            }

            // move
            array(array &&r) : RingBuffer<T, N>(r) {}

            array &operator=(array &&r) {
                RingBuffer<T, N>::operator=(r);
                return *this;
            }

        private:
            using RingBuffer<T, N>::pop;
            using RingBuffer<T, N>::pop_front;
            using RingBuffer<T, N>::push;
            using RingBuffer<T, N>::push_front;
            using RingBuffer<T, N>::emplace;
        };
    } // namespace arx
} // namespace stdx

#endif //ARX_ARRAY_H
