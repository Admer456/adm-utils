
#pragma once

// C++ STL includes

// Containers
#include <array>
#include <optional>
#include <unordered_map>
#include <vector>
#include <list>
// Strings
#include <string>
#include <string_view>
#include <sstream>
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
#include <functional>
#include <memory>

// JSON
#include <nlohmann/json.hpp>

// Some type aliases for stylistic consistency
namespace adm
{
	using String = std::string;
	using StringView = std::string_view;

	// Double linked list
	template<class T>
	using LinkedList = std::list<T>;

	template<class T, size_t N>
	using Array = std::array<T, N>;

	// This is a dynamic array, NOT a 2D/3D/4D vector!!!
	template<class T>
	using Vector = std::vector<T>;

	// This is actually an unordered map, but I've never 
	// really needed to use std::map
	template<class TIndex, class TValue>
	using Map = std::unordered_map<TIndex, TValue>;

	template<class T>
	using Optional = std::optional<T>;

	template<class T>
	using UniquePtr = std::unique_ptr<T>;

	template<class T>
	using SharedPtr = std::shared_ptr<T>;

	template<class T>
	using WeakPtr = std::weak_ptr<T>;
}

// The most basic thing of all
// Defines some compile-time constants like adm::Debug
#include "Platform.hpp"

// Text processing
#include "Text/Format.hpp" // Variadic adm::format
#include "Text/Lexer.hpp" // Text parsing
#include "Text/JSON.hpp" // JSON parsing, really just a wrapper around nlohmann_json

// Game maths
#include "Maths/Lerp.hpp"
#include "Maths/Vec3.hpp" // 3D vector
#include "Maths/Plane.hpp"
#include "Maths/Polygon.hpp"
#include "Maths/AABB.hpp"

// Containers and utilities
#include "Containers/NTree.hpp" // N-dimensional tree, quadtree, octree
#include "Containers/Singleton.hpp" // Singleton wrapper
#include "Containers/Chain.hpp" // Class-wide static linked list
#include "Containers/Dictionary.hpp" // Dictionary/KV pairs

// Time utilities
#include "Time/Timer.hpp" // Scope-based timer

// System-interfacing stuff
#include "System/Library.hpp"

// Visual Studio 2017 15.3 supports C++17 AFAIK
#if defined(_MSC_VER) && _MSC_VER > 1911
#define ADM_EXPORT __declspec(dllexport)

// GCC 5 onwards supports C++17
#elif __GNUC__ >= 5 || defined(__clang__)
// https://gcc.gnu.org/wiki/Visibility
#define ADM_EXPORT __attribute__((visibility("default")))

#else
#error "Compiler not supported"
#endif

#define ADM_API extern "C" ADM_EXPORT
