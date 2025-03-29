#pragma once

#ifndef ARX_RINGBUFFER_H
#define ARX_RINGBUFFER_H

#include <limits.h>
#include "ArxContainer.h"
#include "Move.h"

namespace arx {

    template<typename T, size_t N>
    class RingBuffer {
        class Iterator;

        class ConstIterator {
            friend RingBuffer<T, N>;

            const T *ptr{nullptr};  // pointer to the first element
            int pos{0};

            ConstIterator(const T *ptr, int pos) : ptr(ptr), pos(pos) {}

        public:
            ConstIterator() {}

            ConstIterator(const ConstIterator &it) {
                this->ptr = it.ptr;
                this->pos = it.pos;
            }

            ConstIterator(ConstIterator &&it) {
                this->ptr = stdx::move(it.ptr);
                this->pos = stdx::move(it.pos);
            }

            ConstIterator &operator=(const ConstIterator &rhs) {
                this->ptr = rhs.ptr;
                this->pos = rhs.pos;
                return *this;
            }

            ConstIterator &operator=(ConstIterator &&rhs) {
                this->ptr = stdx::move(rhs.ptr);
                this->pos = stdx::move(rhs.pos);
                return *this;
            }

            // const-like conversion ConstIterator => Iterator
            Iterator to_iterator() const {
                return Iterator(this->ptr, this->pos);
            }

        private:
            static int pos_wrap_around(const int pos) {
                if (pos >= 0)
                    return pos % N;
                else
                    return (N - 1) - (abs(pos + 1) % N);
            }

        public:
            int index() const {
                return pos_wrap_around(pos);
            }

            int index_with_offset(const int i) const {
                return pos_wrap_around(pos + i);
            }

            const T &operator*() const {
                return *(ptr + index());
            }

            const T *operator->() const {
                return ptr + index();
            }

            ConstIterator operator+(const int n) const {
                return ConstIterator(this->ptr, this->pos + n);
            }

            int operator-(const ConstIterator &rhs) const {
                return this->pos - rhs.pos;
            }

            ConstIterator operator-(const int n) const {
                return ConstIterator(this->ptr, this->pos - n);
            }

            ConstIterator &operator+=(const int n) {
                this->pos += n;
                return *this;
            }

            ConstIterator &operator-=(const int n) {
                this->pos -= n;
                return *this;
            }

            // prefix increment/decrement
            ConstIterator &operator++() {
                ++pos;
                return *this;
            }

            ConstIterator &operator--() {
                --pos;
                return *this;
            }

            // postfix increment/decrement
            ConstIterator operator++(int) {
                ConstIterator it = *this;
                ++pos;
                return it;
            }

            ConstIterator operator--(int) {
                ConstIterator it = *this;
                --pos;
                return it;
            }

            bool operator==(const ConstIterator &rhs) const {
                return (rhs.ptr == ptr) && (rhs.pos == pos);
            }

            bool operator!=(const ConstIterator &rhs) const {
                return !(*this == rhs);
            }

            bool operator<(const ConstIterator &rhs) const {
                return pos < rhs.pos;
            }

            bool operator<=(const ConstIterator &rhs) const {
                return pos <= rhs.pos;
            }

            bool operator>(const ConstIterator &rhs) const {
                return pos > rhs.pos;
            }

            bool operator>=(const ConstIterator &rhs) const {
                return pos >= rhs.pos;
            }

        private:
            int raw_pos() const {
                return pos;
            }

            void set(const int i) {
                pos = i;
            }

            void reset() {
                pos = 0;
            }
        };

        class Iterator : public ConstIterator {
            friend RingBuffer<T, N>;

            Iterator(const T *ptr, int pos) {
                this->ptr = ptr;
                this->pos = pos;
            }

        public:
            Iterator() = default;

            Iterator(const Iterator &) = default;

            Iterator(Iterator &&) = default;

            Iterator &operator=(const Iterator &) = default;

            Iterator &operator=(Iterator &&) = default;

            T &operator*() {
                return *(const_cast<T *>(this->ptr) + this->index());
            }

            T *operator->() {
                return const_cast<T *>(this->ptr) + this->index();
            }

            // all inherited methods that return ConstIterator must be reimplemented
            Iterator operator+(const int n) const {
                return Iterator(this->ptr, this->pos + n);
            }

            Iterator operator-(const int n) const {
                return Iterator(this->ptr, this->pos - n);
            }

            Iterator &operator+=(const int n) {
                this->pos += n;
                return *this;
            }

            Iterator &operator-=(const int n) {
                this->pos -= n;
                return *this;
            }

            // prefix increment/decrement
            Iterator &operator++() {
                ++(this->pos);
                return *this;
            }

            Iterator &operator--() {
                --(this->pos);
                return *this;
            }

            // postfix increment/decrement
            Iterator operator++(int) {
                Iterator it = *this;
                ++(this->pos);
                return it;
            }

            Iterator operator--(int) {
                Iterator it = *this;
                --(this->pos);
                return it;
            }
        };

    protected:
        friend class Iterator;

        friend class ConstIterator;

        T queue_[N];
        int head_;
        int tail_;

    public:
        using iterator = Iterator;
        using const_iterator = ConstIterator;

        RingBuffer() : queue_(), head_(0), tail_(0) {
        }

        RingBuffer(std::initializer_list<T> lst) : queue_(), head_(0), tail_(0) {
            for (auto it = lst.begin(); it != lst.end(); ++it) {
                push_back(*it);
            }
        }

        // copy
        explicit RingBuffer(const RingBuffer &r)
                : queue_(), head_(r.head_), tail_(r.tail_) {
            const_iterator it = r.begin();
            for (size_t i = 0; i < r.size(); ++i) {
                int pos = it.index_with_offset(i);
                queue_[pos] = r.queue_[pos];
            }
        }

        RingBuffer &operator=(const RingBuffer &r) {
            head_ = r.head_;
            tail_ = r.tail_;
            const_iterator it = r.begin();
            for (size_t i = 0; i < r.size(); ++i) {
                int pos = it.index_with_offset(i);
                queue_[pos] = r.queue_[pos];
            }
            return *this;
        }

        // move
        RingBuffer(RingBuffer &&r) {
            head_ = stdx::move(r.head_);
            tail_ = stdx::move(r.tail_);
            const_iterator it = r.begin();
            for (size_t i = 0; i < r.size(); ++i) {
                int pos = it.index_with_offset(i);
                queue_[pos] = stdx::move(r.queue_[pos]);
            }
        }

        RingBuffer &operator=(RingBuffer &&r) {
            head_ = stdx::move(r.head_);
            tail_ = stdx::move(r.tail_);
            const_iterator it = r.begin();
            for (size_t i = 0; i < r.size(); ++i) {
                int pos = it.index_with_offset(i);
                queue_[pos] = stdx::move(r.queue_[pos]);
            }
            return *this;
        }

        size_t capacity() const { return N; };

        size_t size() const { return tail_ - head_; }

        // data() method better not to use :-(
        // it should point to the 1st item and have enough space for size() readings of items
        // impossible with ringbuffer - either points to the 1st item or has enough space
        // only exception when it works is when head_ pos == 0
        const T *data() const { return reinterpret_cast<const T *>(&(queue_)); }

        T *data() { return reinterpret_cast<T *>(&(queue_)); }

        bool empty() const { return tail_ == head_; }

        void clear() { head_ = tail_ = 0; }

        void pop() {
            pop_front();
        }

        void pop_front() {
            if (size() == 0) return;
            if (size() == 1)
                clear();
            else
                increment_head();
        }

        void pop_back() {
            if (size() == 0) return;
            if (size() == 1)
                clear();
            else
                decrement_tail();
        }

        void push(const T &data) {
            push_back(data);
        }

        void push(T &&data) {
            push_back(data);
        }

        void push_back(const T &data) {
            get(size()) = data;
            increment_tail();
        }

        void push_back(T &&data) {
            get(size()) = data;
            increment_tail();
        }

        void push_front(const T &data) {
            decrement_head();
            get(0) = data;
        }

        void push_front(T &&data) {
            decrement_head();
            get(0) = data;
        }

        void emplace(const T &data) { push(data); }

        void emplace(T &&data) { push(data); }

        void emplace_back(const T &data) { push_back(data); }

        void emplace_back(T &&data) { push_back(data); }

        const T &front() const { return get(0); }

        T &front() { return get(0); }

        const T &back() const { return get(static_cast<int>(size()) - 1); }

        T &back() { return get(static_cast<int>(size()) - 1); }

        const T &operator[](size_t index) const { return get(static_cast<int>(index)); }

        T &operator[](size_t index) { return get(static_cast<int>(index)); }

        iterator begin() { return empty() ? Iterator() : Iterator(queue_, head_); }

        iterator end() { return empty() ? Iterator() : Iterator(queue_, tail_); }

        const_iterator begin() const { return empty() ? ConstIterator() : ConstIterator(queue_, head_); }

        const_iterator end() const { return empty() ? ConstIterator() : ConstIterator(queue_, tail_); }

        // https://en.cppreference.com/w/cpp/container/vector/erase
        iterator erase(const const_iterator &p) {
            if (!is_valid(p)) return end();

            iterator it_last = end() - 1;
            for (iterator it = p.to_iterator(); it != it_last; ++it)
                *it = *(it + 1);
            *it_last = T();
            decrement_tail();
            return empty() ? end() : p.to_iterator();
        }

        void resize(size_t sz) {
            size_t s = size();
            if (sz > s) {
                for (size_t i = 0; i < sz - s; ++i) push(T());
            } else if (sz < s) {
                for (size_t i = 0; i < s - sz; ++i) pop();
            }
        }

        void assign(const_iterator first, const_iterator end) {
            clear();
            while (first != end) push(*(first++));
        }

        void assign(const T *first, const T *end) {
            clear();
            while (first != end) push(*(first++));
        }

        void shrink_to_fit() {
            // dummy
        }

        void reserve(size_t n) {
            (void) n;
            // dummy
        }

        void fill(const T &v) {
            for (iterator it = begin(); it != end(); ++it)
                *it = v;
        }

        // https://en.cppreference.com/w/cpp/container/vector/insert
        void insert(const const_iterator &pos, const const_iterator &first, const const_iterator &last) {
            if (!is_valid(pos) && pos != end())
                return;

            size_t sz = last - first;

            size_t new_sz = size() + sz;
            if (new_sz > capacity())
                new_sz = capacity();

            iterator it = begin() + new_sz - 1;
            while (it != pos) {
                *it = *(it - sz);
                --it;
            }
            for (size_t i = 0; i < sz; ++i) {
                *(it + i) = *(first + i);
                if (size() < capacity() || (it + i) == end())
                    increment_tail();
            }
        }

        void insert(const const_iterator &pos, const T *first, const T *last) {
            if (!is_valid(pos) && pos != end())
                return;

            size_t sz = last - first;

            size_t new_sz = size() + sz;
            if (new_sz > capacity())
                new_sz = capacity();

            iterator it = begin() + new_sz - 1;
            while (it != pos) {
                *it = *(it - sz);
                --it;
            }
            for (size_t i = 0; i < sz; ++i) {
                *(it + i) = *(first + i);
                if (size() < capacity() || (it + i) == end())
                    increment_tail();
            }
        }

        void insert(const const_iterator &pos, const T &val) {
            const T *ptr = &val;
            insert(pos, ptr, ptr + 1);
        }

    private:
        T &get(const iterator &it) {
            return queue_[it.index()];
        }

        const T &get(const const_iterator &it) const {
            return queue_[it.index()];
        }

        T &get(const int index) {
            return queue_[begin().index_with_offset(index)];
        }

        const T &get(const int index) const {
            return queue_[begin().index_with_offset(index)];
        }

        T *ptr(const iterator &it) {
            return queue_ + it.index();
        }

        const T *ptr(const const_iterator &it) const {
            return queue_ + it.index();
        }

        T *ptr(const int index) {
            return queue_ + begin().index_with_offset(index);
        }

        const T *ptr(const int index) const {
            return queue_ + begin().index_with_offset(index);
        }

        void increment_head() {
            ++head_;
            resolve_overflow();
        }

        void increment_tail() {
            ++tail_;
            resolve_overflow();
            if (size() > N)
                increment_head();
        }

        void decrement_head() {
            --head_;
            resolve_overflow();
            if (size() > N)
                decrement_tail();
        }

        void decrement_tail() {
            --tail_;
            resolve_overflow();
        }

        void resolve_overflow() {
            if (empty())
                clear();
            else if (head_ <= (static_cast<int>(INT_MIN) + static_cast<int>(capacity())) \
 || tail_ >= (static_cast<int>(INT_MAX) - static_cast<int>(capacity()))) {
                // +/- capacity(): reserve some space for pointer/iterator arithmetics
                // head_/tail_ pointers are re-set N+1 steps before the overflow occurs
                int len = size();
                head_ = begin().index();
                tail_ = head_ + len;
            }
        }

        bool is_valid(const const_iterator &it) const {
            if (it.ptr != queue_)
                return false; // iterator to a different object
            return (it.raw_pos() >= head_) && (it.raw_pos() < tail_);
        }
    };
} // namespace arx

template<typename T, size_t N>
inline bool operator==(const arx::RingBuffer<T, N> &x, const arx::RingBuffer<T, N> &y) {
    if (x.size() != y.size()) return false;
    for (size_t i = 0; i < x.size(); ++i)
        if (x[i] != y[i]) return false;
    return true;
}

template<typename T, size_t N>
inline bool operator!=(const arx::RingBuffer<T, N> &x, const arx::RingBuffer<T, N> &y) {
    return !(x == y);
}

template<typename T, size_t N>
using ArxRingBuffer = arx::RingBuffer<T, N>;

#endif //ARX_RINGBUFFER_H
