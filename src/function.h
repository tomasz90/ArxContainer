#pragma once

#ifndef ARXCONTAINER_FUNCTION_H
#define ARXCONTAINER_FUNCTION_H

#include "Arduino.h"
#include "type_traits.h"

namespace arx {
    namespace stdx {

// --- function implementation ---
        template<typename>
        class function;

        template<typename Res, typename... Args>
        class function<Res(Args...)> {
        private:
            static const size_t BUFFER_SIZE = 16;
            alignas(sizeof(void *)) char buffer[BUFFER_SIZE];

            struct vtable {
                void (*copy)(const void *src, void *dest);
                void (*destroy)(void *obj);
                Res (*invoke)(const void *obj, Args... args);
            };

            const vtable *ops = nullptr;

            template<typename Callable>
            struct is_small {
                static const bool value =
                        sizeof(Callable) <= BUFFER_SIZE &&
                        alignof(Callable) <= alignof(void *) &&
                        is_trivially_copyable<Callable>::value;
            };

        public:
            function() = default;

            // Function pointer constructor
            function(Res (*fptr)(Args...)) {
                if (fptr) {
                    ops = &get_vtable<Res(*)(Args...)>();
                    new(buffer) decltype(fptr)(fptr);
                } else {
                    ops = nullptr; // Treat nullptr as empty
                }
            }

            // Functor constructor
            template<typename Callable>
            function(
                    Callable c,
                    typename enable_if<
                            !is_same<Callable, function>::value &&
                            is_small<Callable>::value
                    >::type * = nullptr
            ) {
                ops = &get_vtable<Callable>();
                new(buffer) Callable(c);
            }

            function(function &&other) noexcept {
                ops = other.ops;
                if (ops) {
                    memcpy(buffer, other.buffer, BUFFER_SIZE);
                    other.ops = nullptr;
                }
            }

            function(const function &other) {
                if (other.ops) {
                    ops = other.ops;
                    ops->copy(other.buffer, buffer);
                } else {
                    ops = nullptr;
                    memset(buffer, 0, BUFFER_SIZE);
                }
            }

            ~function() {
                if (ops && ops->destroy) ops->destroy(buffer);
            }

            Res operator()(Args... args) const {
                if (!ops || !ops->invoke) {
                    return invoke_return(static_cast<Res *>(nullptr));
                }
                return ops->invoke(buffer, args...);
            }

            function &operator=(function &&other) noexcept {
                if (this != &other) {
                    // Destroy current contents
                    if (ops && ops->destroy) {
                        ops->destroy(buffer);
                    }
                    ops = other.ops;
                    if (ops) {
                        memcpy(buffer, other.buffer, BUFFER_SIZE);
                        other.ops = nullptr;
                    } else {
                        memset(buffer, 0, BUFFER_SIZE);
                    }
                }
                return *this;
            }

            function &operator=(decltype(nullptr)) noexcept {
                if (ops && ops->destroy) {
                    ops->destroy(buffer); // Destroy stored callable
                }
                ops = nullptr;
                memset(buffer, 0, BUFFER_SIZE); // Clear buffer
                return *this;
            }

            bool operator!=(decltype(nullptr)) const noexcept {
                return ops != nullptr;
            }

            bool operator==(decltype(nullptr)) const noexcept {
                return ops == nullptr;
            }

            explicit operator bool() const { return ops != nullptr; }

        private:
            // Return type handling
            template<typename T>
            static T invoke_return(T *) { return T(); }
            static void invoke_return(void *) {}

            template<typename Callable>
            static const vtable &get_vtable() {
                static const vtable instance = create_vtable<Callable>();
                return instance;
            }

            template<typename Callable>
            static vtable create_vtable() {
                return {
                        // COPY
                        [](const void *src, void *dest) {
                            new(dest) Callable(*static_cast<const Callable *>(src));
                        },
                        // DESTROY
                        [](void *obj) { static_cast<Callable *>(obj)->~Callable(); },
                        // INVOKE (FIXED)
                        [](const void *obj, Args... args) -> Res {
                            const Callable *callable = static_cast<const Callable *>(obj);
                            if (is_void<Res>::value) {
                                (*callable)(args...);  // No return for void
                            } else {
                                return (*callable)(args...);  // Return value
                            }
                        }
                };
            }
        };
    } // namespace stdx
} // namespace arx

#endif //ARXCONTAINER_FUNCTION_H
