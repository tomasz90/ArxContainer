#pragma once

#ifndef ARX_SET_H
#define ARX_SET_H

#ifndef ARX_SET_DEFAULT_SIZE
#define ARX_SET_DEFAULT_SIZE 16
#endif  // ARX_SET_DEFAULT_SIZE

#ifdef ARDUINO
    #include <Arduino.h>
#endif
#include "initializer_list.h"

namespace arx {
    namespace stdx {
        template<typename T, size_t N = ARX_SET_DEFAULT_SIZE>
        class set {
        private:
            T data[N];
            size_t count;

        public:
            set() : count(0) {}

            set(std::initializer_list<T> lst) : count(0) {
                for (const auto &item: lst) insert(item);
            }

            ~set() { clear(); }

            bool insert(const T &data_) {
                if (count >= N || exist(data_)) return false;
                size_t low = binary(data_);
                // Insert and shift elements
                data[count++] = data_;
                for (size_t i = count - 1; i > low; i--) {
                    data[i] = data[i - 1];
                }
                data[low] = data_;
                return true;
            }

            T &any() {
                return data[random(count)];
            }

            bool erase(const T &data_) {
                int idx = index(data_);
                if (idx == -1) return false;
                // Shift elements left
                for (size_t i = idx; i < count - 1; i++) {
                    (*this)[i] = (*this)[i + 1];
                }
                --count;
                return true;
            }

            void clear() {
                for (size_t i = 0; i < count; i++) {
                    data[i].~T();
                }
                count = 0;
            }

            size_t size() const {
                return count;
            }

            bool empty() const {
                return count == 0;
            }

            bool exist(const T &data_) const {
                return index(data_) != -1;
            }

            T *begin() { return data; }

            T *end() { return data + count; }

            const T *begin() const { return data; }

            const T *end() const { return data + count; }

            friend bool operator==(const set &a, const set &b) {
                if (a.count != b.count) return false;
                for (size_t i = 0; i < a.count; i++) {
                    if (a[i] != b[i]) return false;
                }
                return true;
            }

            friend bool operator!=(const set &a, const set &b) {
                return !(a == b);
            }

            friend bool operator<(const set &a, const set &b) {
                if (a.count != b.count) return a.count < b.count;

                // If sizes are equal, compare elements
                for (size_t i = 0; i < a.count; i++) {
                    if (a[i] < b[i]) return true;
                    if (b[i] < a[i]) return false;
                }
                return false; // Sets are equal
            }

        private:
            int index(const T &data_) const {
                size_t low = binary(data_);
                return (low < count && (*this)[low] == data_) ? low : -1;
            }

            int binary(const T &data_) const {
                // Find position using binary search
                size_t low = 0;
                size_t high = count;
                while (low < high) {
                    size_t mid = (low + high) / 2;
                    if ((*this)[mid] < data_) {
                        low = mid + 1;
                    } else {
                        high = mid;
                    }
                }
                return low;
            }

            T operator[](size_t index) const {
                return data[index];
            }

            T &operator[](size_t index) {
                return data[index];
            }
        };
    } //  namespace stdx
} // namespace arx

#endif //ARX_SET_H
