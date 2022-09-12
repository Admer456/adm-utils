
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

// Add X11 and other stuff here

#endif
