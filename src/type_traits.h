#pragma once

#ifndef ARXCONTAINER_TYPE_TRAITS_H
#define ARXCONTAINER_TYPE_TRAITS_H

namespace arx {
    namespace stdx {
        template<typename T, typename U>
        struct is_same {
            static const bool value = false;
        };

        template<typename T>
        struct is_same<T, T> {
            static const bool value = true;
        };

        template<bool B, typename T = void>
        struct enable_if {
        };

        template<typename T>
        struct enable_if<true, T> {
            typedef T type;
        };

        template<typename T>
        struct is_void {
            static const bool value = false;
        };
        template<>
        struct is_void<void> {
            static const bool value = true;
        };

        template<typename T>
        struct is_trivially_copyable {
            static const bool value = __is_trivially_copyable(T);
        };
    }
}
#endif //ARXCONTAINER_TYPE_TRAITS_H
