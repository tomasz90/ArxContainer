#ifndef ARXCONTAINER_PAIR_H
#define ARXCONTAINER_PAIR_H

namespace arx {
    namespace stdx {

        template<class T1, class T2>
        struct pair {
            T1 first;
            T2 second;
        };

        template<class T1, class T2>
        inline pair<T1, T2> make_pair(const T1 &t1, const T2 &t2) {
            return {t1, t2};
        };

    } // namespace arx
} // namespace stdx

#endif //ARXCONTAINER_PAIR_H
