// SPDX-FileCopyrightText: 2022 Admer Šuko
// SPDX-License-Identifier: MIT

#pragma once

namespace adm
{
	// ============================
	// Library
	//
	// A utility for loading and using dynamic modules
	// Usage:
	// 
	// Library myLib( "bin/mylib" );
	// if ( myLib )
	//     myLib.FindFunction<
	// ============================
	class Library final
	{
	public:
		Library( StringView libraryPath );
		Library( const char* libraryPath );
		Library( void* handle );
		Library( Library&& library ) noexcept;
		Library( const Library& library ) = delete;
		~Library();

		// Easy way to determine a return type
		// https://stackoverflow.com/questions/53673442/simplest-way-to-determine-return-type-of-function
		template<typename functionType>
		using FunctionReturnType = typename decltype(std::function{ std::declval<functionType>() })::result_type;

		// Parameterless version
		template<typename functionType,
			typename returnType = FunctionReturnType<functionType>>
		inline Optional<returnType> TryExecuteFunction( StringView functionName )
		{
			auto func = FindFunction<functionType>( functionName );
			if ( nullptr == func )
			{
				return {};
			}

			return (*func)();
		}

		template<typename functionType, 
			typename structArgumentType, 
			typename returnType = FunctionReturnType<functionType>>
		inline Optional<returnType> TryExecuteFunction( StringView functionName, const structArgumentType& argument )
		{
			auto func = FindFunction<functionType>( functionName );
			if ( nullptr == func )
			{
				return {};
			}

			return (*func)( argument );
		}

		template<typename functionType>
		functionType* FindFunction( StringView functionName ) const
		{
			return reinterpret_cast<functionType*>(GetFunctionInternal( functionName ));
		}

		void Dispose();

		operator bool() const;

	private:
		void* GetFunctionInternal( StringView functionName ) const;

		void* libraryHandle{ nullptr };
	};
}