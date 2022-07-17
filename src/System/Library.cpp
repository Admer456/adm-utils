
#include "Precompiled.hpp"
using namespace adm;

#if ADM_PLATFORM == PLATFORM_WINDOWS
#include <Windows.h>
#elif ADM_PLATFORM == PLATFORM_LINUX
#include <dlfcn.h>
#endif

static void* SystemLoadLibrary( const char* path )
{
	void* handle = nullptr;

#if ADM_PLATFORM == PLATFORM_WINDOWS

	handle = LoadLibraryA( path );

#elif ADM_PLATFORM == PLATFORM_LINUX

	handle = dlopen( path, RTLD_NOW | RTLD_LOCAL );

#endif

	return handle;
}

static void* SystemFindFunction( void* handle, const char* functionName )
{
	if ( nullptr == handle )
	{
		return nullptr;
	}

	void* function = nullptr;

#if ADM_PLATFORM == PLATFORM_WINDOWS

	function = GetProcAddress( static_cast<HMODULE>(handle), functionName );

#elif ADM_PLATFORM == PLATFORM_LINUX

	function = dlsym( handle, functionName );
	if ( nullptr == function )
	{
		// Append an underscore for platforms that need it
		String newFunctionName = "_";
		newFunctionName.append( functionName );

		function = dlsym( handle, functionName );
	}

#endif

	return function;
}

static void SystemUnloadLibrary( void* handle )
{
	if ( nullptr != handle )
	{
#if ADM_PLATFORM == PLATFORM_WINDOWS

		FreeLibrary( static_cast<HMODULE>(handle) );

#elif ADM_PLATFORM == PLATFORM_LINUX

		dlclose( handle );

#endif
	}
}

adm::Library::Library( StringView libraryPath )
{
	libraryHandle = SystemLoadLibrary( libraryPath.data() );
}

adm::Library::Library( const char* libraryPath )
	: Library( StringView( libraryPath ) )
{
}

adm::Library::Library( void* handle )
{
	libraryHandle = handle;
}

adm::Library::Library( Library&& library ) noexcept
{
	libraryHandle = library.libraryHandle;
	library.libraryHandle = nullptr;
}

adm::Library::~Library()
{
	Dispose();
}

void adm::Library::Dispose()
{
	if ( nullptr != libraryHandle )
	{
		SystemUnloadLibrary( libraryHandle );
		libraryHandle = nullptr;
	}
}

adm::Library::operator bool() const
{
	return libraryHandle != nullptr;
}

bool adm::Library::operator!() const
{
	return libraryHandle == nullptr;
}

void* adm::Library::GetFunctionInternal( StringView functionName ) const
{
	return SystemFindFunction( libraryHandle, functionName.data() );
}
