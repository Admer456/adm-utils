
#pragma once

namespace adm
{
    constexpr size_t FormatBufferSize = 4096U;

    // Variadic string formatting
    // Should be threadsafe, so long as each thread uses a different 'slot'
    template<uint32_t slot = 0, bool DebugFormat = false>
    inline char* format( char* formatString, ... )
    {
        // Hoping the compiler won't optimise this away
        constexpr uint32_t localSlot = slot;

        va_list arguments;
        static char buffer[FormatBufferSize];

        // Format the string
        va_start( arguments, formatString );
        vsprintf( buffer, formatString, arguments );
        va_end( arguments );

        if constexpr ( DebugFormat )
        {
            snprintf( buffer, FormatBufferSize, "%s::DBG::slot%u", buffer, localSlot );
        }

        return buffer;
    }
}
