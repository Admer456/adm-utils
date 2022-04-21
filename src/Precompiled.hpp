
#pragma once

// C++ STL includes

// Containers
#include <array>
#include <optional>
#include <unordered_map>
// Strings
#include <string>
#include <string_view>
#include <sstream>
#include <vector>
// Maths
#include <cmath>
#include <algorithm>
// File system
#include <fstream>
#include <filesystem>
// Misc
#include <chrono>
#include <type_traits>
#include <stdarg.h>
#include <thread>

// The most basic thing of all
// Defines some compile-time constants like adm::Debug
#include "Platform.hpp"

// Text processing
#include "Text/Format.hpp" // Variadic adm::format
#include "Text/Lexer.hpp" // Text parsing

// Game maths
#include "Maths/Lerp.hpp"
#include "Maths/Vec3.hpp" // 3D vector
#include "Maths/Plane.hpp"
#include "Maths/Polygon.hpp"

// Containers and utilities
#include "Containers/Singleton.hpp" // Singleton wrapper
#include "Containers/InstanceChain.hpp" // Class-wide static linked list
#include "Containers/Dictionary.hpp" // Dictionary/KV pairs

// Time utilities
#include "Time/Timer.hpp" // Scope-based timer

// Visual Studio 2017 15.3 supports C++17 AFAIK
#if defined(_MSC_VER) && _MSC_VER > 1911
#define ADM_EXPORT __declspec(dllexport)

// GCC 5 onwards supports C++17
#elif __GNUC__ >= 5 || defined(__clang__)
// https://gcc.gnu.org/wiki/Visibility
#define ADM_EXPORT __attribute__ ((visibility ("default")))

#else
#error "Compiler not supported"
#endif
