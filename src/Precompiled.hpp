
#pragma once

// C++ STL includes

// Containers
#include <array>
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

// Containers and utilities
#include "Containers/Singleton.hpp" // Singleton wrapper
#include "Containers/InstanceChain.hpp" // Class-wide static linked list
#include "Containers/Dictionary.hpp" // Dictionary/KV pairs

// Time utilities
#include "Time/Timer.hpp" // Scope-based timer

#ifdef WIN32
#define ADM_EXPORT __declspec(dllexport)
#else
#error "Don't know how to do this one yet"
#endif
