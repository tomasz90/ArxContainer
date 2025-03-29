#pragma once

#ifndef ARX_MAP_H
#define ARX_MAP_H

#ifndef ARX_MAP_DEFAULT_SIZE
#define ARX_MAP_DEFAULT_SIZE 16
#endif  // ARX_MAP_DEFAULT_SIZE

#include "Pair.h"
#include "RingBuffer.h"

namespace arx {
    namespace stdx {

        template<class Key, class T, size_t N = ARX_MAP_DEFAULT_SIZE>
        struct map : public RingBuffer<pair<Key, T>, N> {
            using base = RingBuffer<pair<Key, T>, N>;
            using iterator = typename base::iterator;
            using const_iterator = typename base::const_iterator;

            map() : base() {}

            map(std::initializer_list<pair<Key, T> > lst) : base(lst) {}

            // copy
            map(const map &r) : base(r) {}

            map &operator=(const map &r) {
                base::operator=(r);
                return *this;
            }

            // move
            map(map &&r) : base(r) {}

            map &operator=(map &&r) {
                base::operator=(r);
                return *this;
            }

            const_iterator find(const Key &key) const {
                for (const_iterator it = this->begin(); it != this->end(); ++it) {
                    if (key == it->first)
                        return it;
                }
                return this->end();
            }

            iterator find(const Key &key) {
                for (iterator it = this->begin(); it != this->end(); ++it) {
                    if (key == it->first)
                        return it;
                }
                return this->end();
            }

            pair<iterator, bool> insert(const Key &key, const T &t) {
                return insert(::arx::stdx::make_pair(key, t));
            }

            pair<iterator, bool> insert(const pair<Key, T> &p) {
                bool b{false};
                iterator it = find(p.first);
                if (it == this->end()) {
                    this->push(p);
                    b = true;
                    it = this->begin() + this->size() - 1;
                }
                return {it, b};
            }

            pair<iterator, bool> emplace(const Key &key, const T &t) {
                return insert(key, t);
            }

            pair<iterator, bool> emplace(const pair<Key, T> &p) {
                return insert(p);
            }

        private:
            T &empty_value() const {
                static T val;
                val = T(); // fresh empty value every time
                return val;
            }

        public:
            const T &at(const Key &key) const {
                const_iterator it = find(key);
                if (it != this->end()) return it->second;
                return empty_value();
                //return find(key)->second;
            }

            T &at(const Key &key) {
                iterator it = find(key);
                if (it != this->end()) return it->second;
                return empty_value();
                //return find(key)->second;
            }

            iterator erase(const const_iterator &it) {
                iterator i = find(it->first);
                return base::erase(i);
            }

            iterator erase(const Key &key) {
                iterator i = find(key);
                return base::erase(i);
            }

            // erase() will cause compile error if map's Key is 'unsigned int'
            // => collision of this method with erase(const Key&)
            iterator erase(const size_t index) {
                if (index < this->size()) {
                    iterator it = this->begin() + index;
                    return erase(it);
                }
                return this->end();
            }

            T &operator[](const Key &key) {
                iterator it = find(key);
                if (it != this->end()) return it->second;

                insert(::arx::stdx::make_pair(key, T()));
                return this->back().second;
            }

        private:
            using base::assign;
            using base::back;
            using base::capacity;
            using base::data;
            using base::emplace_back;
            using base::front;
            using base::pop;
            using base::pop_back;
            using base::pop_front;
            using base::push;
            using base::push_back;
            using base::push_front;
            using base::resize;
            using base::shrink_to_fit;
            using base::fill;
        };
    } //  namespace stdx
} // namespace arx

#endif //ARX_MAP_H
