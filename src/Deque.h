#pragma once

#ifndef ARX_DEQUE_H
#define ARX_DEQUE_H

#ifndef ARX_DEQUE_DEFAULT_SIZE
#define ARX_DEQUE_DEFAULT_SIZE 16
#endif  // ARX_DEQUE_DEFAULT_SIZE

#include "RingBuffer.h"

namespace arx {
    namespace stdx {

        template<typename T, size_t N = ARX_DEQUE_DEFAULT_SIZE>
        struct deque : public RingBuffer<T, N> {
            using iterator = typename RingBuffer<T, N>::iterator;
            using const_iterator = typename RingBuffer<T, N>::const_iterator;

            deque() : RingBuffer<T, N>() {}

            deque(std::initializer_list<T> lst) : RingBuffer<T, N>(lst) {}

            // copy
            deque(const deque &r) : RingBuffer<T, N>(r) {}

            deque &operator=(const deque &r) {
                RingBuffer<T, N>::operator=(r);
                return *this;
            }

            // move
            deque(deque &&r) : RingBuffer<T, N>(r) {}

            deque &operator=(deque &&r) {
                RingBuffer<T, N>::operator=(r);
                return *this;
            }

        private:
            using RingBuffer<T, N>::capacity;
            using RingBuffer<T, N>::pop;
            using RingBuffer<T, N>::push;
            using RingBuffer<T, N>::fill;
        };
    } // namespace arx
} // namespace stdx

#endif //ARX_DEQUE_H
