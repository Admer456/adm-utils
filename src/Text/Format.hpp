// SPDX-FileCopyrightText: 2021-2022 Admer Šuko
// SPDX-License-Identifier: MIT

#pragma once

namespace adm
{
	constexpr size_t FormatBufferSize = 4096U;
	constexpr size_t FormatBufferIndices = 4U;

	// Variadic string formatting
	// Should be threadsafe, so long as each thread uses a different 'slot'
	template<uint32_t slot = 0, bool DebugFormat = false>
	inline char* format( char* formatString, ... )
	{
		// Hoping the compiler won't optimise this away
		constexpr uint32_t localSlot = slot;

		static char buffer[FormatBufferIndices][FormatBufferSize];
		static int bufferIndex = 0; // for nested formattings - it happens sometimes
		static char* bufferPointer = nullptr;
		va_list arguments;

		bufferPointer = buffer[bufferIndex++ % FormatBufferIndices];

		// Format the string
		va_start( arguments, formatString );
		vsprintf( bufferPointer, formatString, arguments );
		va_end( arguments );

		if constexpr ( DebugFormat )
		{
			snprintf( bufferPointer, FormatBufferSize, "%s::DBG::slot%u", bufferPointer, localSlot );
		}

		return bufferPointer;
	}
}
