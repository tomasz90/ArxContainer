#pragma once

#ifndef ARX_CONTAINER_H
#define ARX_CONTAINER_H

#include "config/has_libstdcplusplus.h"

#ifdef ARDUINO
    #include <Arduino.h>
#endif

#if !defined(_NEW) || !defined(NEW_H)
    #include <new>
#endif

// Make sure std namespace exists
namespace std {}

// Import everything from the std namespace into arx::std, so that
// anything we import rather than define is also available through
// arx::stdx.
// This includes everything yet to be defined, so we can do this early
// (and must do so, to allow e.g. the C++14 additions in the arx::std
// namespace to reference the C++11 stuff from the system headers.
namespace arx {
    namespace stdx {
        using namespace ::std;
    }
}

// Import everything from arx::std back into the normal std namespace.
// This ensures that you can just use `std::foo` everywhere and you get
// the standard library version if it is available, falling back to arx
// versions for things not supplied by the standard library. Only when
// you really need the arx version (e.g. for constexpr numeric_limits
// when also using ArduinoSTL), you need to qualify with arx::stdx::
namespace std {
    using namespace ::arx::stdx;
}

// Initializer_list *must* be defined in std, so take extra care to only
// define it when <initializer_list> is really not available (e.g.
// ArduinoSTL is C++98 but *does* define <initializer_list>) and not
// already defined (e.g. by ArxContainer).
#if ARX_SYSTEM_HAS_INCLUDE(<initializer_list>)
    #include <initializer_list>
#else
    #include "InitializerList.h"
#endif
// Have libstdc++11
#if (ARX_HAVE_LIBSTDCPLUSPLUS >= 201103L) && !USE_ARX_LIB

#include <vector>
#include <array>
#include <map>
#include <set>
#include <functional>
#include <deque>

#else

#include "Vector.h"
#include "Array.h"
#include "Map.h"
#include "Set.h"
#include "Function.h"
#include "Deque.h"

#endif

#endif  // ARX_H
