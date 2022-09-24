
namespace adm
{
#if defined( _DEBUG ) || defined( DEBUG )
	constexpr bool Debug = false;
#else
	constexpr bool Debug = true;
#endif

#define PLATFORM_WINDOWS 0
#define PLATFORM_LINUX 1

	// These aren't used in AdmUtils, since the code should be
	// portable to all platforms specified below, BUT, it may be
	// useful to whoever uses this library
	struct Platforms
	{
		static constexpr int Windows = PLATFORM_WINDOWS;
		static constexpr int Linux = PLATFORM_LINUX;

		static constexpr int Undefined = -1;
	};

#ifdef WIN32
#define ADM_PLATFORM PLATFORM_WINDOWS
	constexpr int Platform = Platforms::Windows;
#elif __linux__
#define ADM_PLATFORM PLATFORM_LINUX
	constexpr int Platform = Platforms::Linux;
#else
	static_assert(false, "Unsupported platform");
#endif
}

// You may want to use HINSTANCE or HWND somewhere, so here you go
#if ADM_PLATFORM == PLATFORM_WINDOWS

#define NOMINMAX 1
#include <Windows.h>

// If you specifically need this, just use CreateWindowA or something
#ifdef CreateWindow
#undef CreateWindow
#endif

#ifdef DrawText
#undef DrawText
#endif

#elif ADM_PLATFORM == PLATFORM_LINUX

#if ADM_USE_X11
#include <X11/Xlib.h>
#endif

#if ADM_USE_WAYLAND
#include <wayland-client-core.h>
#endif

#endif

// ADM_EXPORT for DLL exporting stuff
// Very platform-specific matters

// Visual Studio 2017 15.3 supports C++17 AFAIK
#if defined(_MSC_VER) && _MSC_VER > 1911
#define ADM_EXPORT __declspec(dllexport)

// Stupid stupid hack to work around a really really
// stupid MSVC detail
#define ADM_CPP _MSVC_LANG

// GCC 5 onwards supports C++17
#elif __GNUC__ >= 5 || defined(__clang__)
// https://gcc.gnu.org/wiki/Visibility
#define ADM_EXPORT __attribute__((visibility("default")))

#define ADM_CPP __cplusplus

#else

#if defined(_MSC_VER)
#error "Your MSVC compiler is outdated, you need something from 2017 and newer"
#elif __GNUC__ < 5
#error "Your G++ is outdated, you need at least GCC 5 or newer with C++17 support"
#else
#error "Compiler not supported"
#endif

#endif

// ADM_EXPORT void Something() would export, but it'd end up with random
// characters in places and it'd be pretty hard to obtain a function pointer to it
// using the adm::Library utility
// ADM_API void Something() would just export as _Something, however you
// cannot return references or have them as arguments. You're limited to "C-style
// datatypes" if that makes sense, so only primitive datatypes, PODs and pointers

// Export an API without name mangling, so it can be easily looked up by adm::Library
#define ADM_API extern "C" ADM_EXPORT

// SSE stuff
#if ADM_USE_SSE41
#include <immintrin.h>
#endif
