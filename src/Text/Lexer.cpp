
#include "Precompiled.hpp"
using namespace adm;

// ============================
// Lexer::ctor
// ============================
Lexer::Lexer()
{
	Clear();
}

// ============================
// Lexer::move ctor
// ============================
Lexer::Lexer( Lexer&& other ) noexcept
{
	position = other.position;
	inQuote = other.inQuote;

	buffer = std::move( other.buffer );
	delimiterString = std::move( other.delimiterString );
	view = buffer;
}

// ============================
// Lexer::copy ctor
// ============================
Lexer::Lexer( const Lexer& other )
{
	position = other.position;
	inQuote = other.inQuote;

	Load( other.buffer );
	delimiterString = other.delimiterString;
}

// ============================
// Lexer::ctor for fstream
// ============================
Lexer::Lexer( std::fstream& fileStream )
{
	// There's currently not much of a clean way to do this,
	// as far as I'm aware
	// It'd be nice if I could just grab the sstream buffer directly
	// and pass it as a const char* to Load
	String line, text;
	std::ostringstream sstream;
	while ( std::getline( fileStream, line ) )
	{
		sstream << line.c_str() << '\n';
	}

	text = sstream.str();
	Load( text );
}

// ============================
// Lexer::ctor for ifstream
// ============================
Lexer::Lexer( std::ifstream& fileStream )
{
	String line, text;
	std::ostringstream sstream;
	while ( std::getline( fileStream, line ) )
	{
		sstream << line.c_str() << '\n';
	}

	text = sstream.str();
	Load( text );
}

// ============================
// Lexer::dtor
// ============================
Lexer::~Lexer()
{
	Clear();
}

// ============================
// Lexer::ctor for C strings
// ============================
Lexer::Lexer( const char* text )
{
	Load( text );
}

// ============================
// Lexer::ctor for string_view
// ============================
Lexer::Lexer( StringView text )
{
	Load( text );
}

// ============================
// Lexer::Clear
// ============================
void Lexer::Clear()
{
	position = 0;
	
	buffer.clear();
	delimiterString.clear();
}

// ============================
// Lexer::Load
// ============================
void Lexer::Load( const char* text )
{
	buffer = text;
	view = buffer;
}

// ============================
// Lexer::Load
// ============================
void Lexer::Load( StringView text )
{
	Load( text.data() );
}

// ============================
// Lexer::SetDelimiters
// ============================
void Lexer::SetDelimiters( const char* delimiters )
{
	if ( nullptr == delimiters )
	{
		delimiters = "";
	}

	delimiterString = delimiters;
}

// ============================
// Lexer::Next
// ============================
String Lexer::Next( bool withDelimiter )
{
	String result;

	// This do-while loop is responsible for skipping "empty" lines
	// and advancing to the next ones, until we get a usable token
	do
	{
		// Skip any whitespaces etc. after a token
		while ( !CanAdvance() && !IsDelimiter() && !IsEndOfFile() )
		{
			if ( IsComment() )
			{
				NewLine();
				continue;
			}

			position++;
		}

		// Can't go any further
		if ( IsEndOfFile() )
		{
			if constexpr ( DebugLexer )
			{
				printf( "Lexer::Next: EOF\n" );
			}
			return "";
		}

		// Check for delimiters
		if ( IsDelimiter() )
		{
			if ( withDelimiter )
			{
				result = char( view[position] );
				IncrementPosition();
				return result;
			}

			IncrementPosition();
			continue;
		}

		while ( CanAdvance() )
		{
			// We only support single-line comments, so
			// if a comment is encountered, skip the whole line
			if ( IsComment() )
			{
				NewLine();
				continue;
			}

			// A quotation mark has been encountered while
			// we weren't in quote mode - engage
			if ( view[position] == '"' )
			{
				if constexpr ( DebugLexer )
				{
					printf( "Lexer::Next: found a '\"', toggling quote mode\n" );
				}

				ToggleQuoteMode();
				IncrementPosition();
				continue;
			}

			// In quote mode, we add spaces and tabs too
			if ( CanAdd() )
			{
				result += view[position];
			}

			// Safely increment the position so we don't go out of bounds
			IncrementPosition();
		}
	} while ( result.empty() );

	if constexpr ( DebugLexer )
	{
		printf( "Lexer::Next: token '%s'\n", result.c_str() );
	}

	// Escape from a quote
	if ( inQuote && view[position] == '"' )
	{
		if constexpr ( DebugLexer )
		{
			printf( "Lexer::Next: found a '\"', toggling quote mode\n" );
		}

		ToggleQuoteMode();
		IncrementPosition();
	}

	return result;
}

// ============================
// Lexer::Expect
// ============================
bool Lexer::Expect( const char* expectedToken, bool advance )
{
	size_t oldPosition = position;
	
	String nextToken = Next();
	
	if ( !advance )
	{
		position = oldPosition;
	}

	return nextToken == expectedToken;
}

// ============================
// Lexer::IsEndOfFile
// ============================
bool Lexer::IsEndOfFile() const
{
	return position >= buffer.size();
}

// ============================
// Lexer::CanAdd
// 
// Can the character at the position be added
// to the resulting token?
// ============================
bool Lexer::CanAdd() const
{
	const char& c = view[position];

	return CanAdvance() && c != '"';
}

// ============================
// Lexer::CanAdvance
// 
// Can the position move forward?
// By default, the advancing will stop when a space,
// tab or EOF is hit. However, if we're in quote mode,
// we only look for EOF and the ending quotation mark
// ============================
bool Lexer::CanAdvance() const
{
	if ( IsEndOfFile() )
	{
		return false;
	}

	const char& c = view[position];

	if ( inQuote )
	{
		return !IsEndOfFile() && c != '"';
	}

	if ( IsDelimiter() )
	{
		return false;
	}

	return c != ' ' && c != '\t' && c != '\0' && !IsEndOfLine();
}

// ============================
// Lexer::IsComment
// 
// Checks if the position is
// currently on a comment
// ============================
bool Lexer::IsComment() const
{
	if ( inQuote )
	{
		return false;
	}

	if ( view[position] == '/' )
	{
		if ( position + 1 < buffer.size() )
		{
			if ( view[position + 1] == '/' )
			{
				return true;
			}
		}
	}

	return false;
}

// ============================
// Lexer::IsEndOfLine
// ============================
inline bool Lexer::IsEndOfLine() const
{
	return view[position] == '\n' || view[position] == '\r';
}

// ============================
// Lexer::IsDelimiter
// ============================
inline bool Lexer::IsDelimiter() const
{
	return (delimiterString.find( view[position] ) != String::npos) && !inQuote;
}

// ============================
// Lexer::NewLine
// 
// Jumps to a new line if it 
// can be found
// ============================
void Lexer::NewLine()
{
	size_t newPosition = view.find( '\n', position+1 );
	if ( newPosition == String::npos )
	{
		if constexpr ( DebugLexer )
		{
			printf( "Lexer::NewLine: Didn't find a newline...\n" );
		}

		position = view.size() + 1;
		return;
	}

	if constexpr ( DebugLexer )
	{
		printf( "Lexer::NewLine: Found a newline, jumping...\n" );
	}

	position = newPosition + 1;
}

// ============================
// Lexer::ToggleQuoteMode
// 
// When entering or exiting
// a quote, this is called
// ============================
inline void Lexer::ToggleQuoteMode()
{
	inQuote = !inQuote;
}

// ============================
// Lexer::IncrementPosition
// 
// Safely increments the position
// in the text buffer
// ============================
inline void Lexer::IncrementPosition()
{
	if ( !IsEndOfFile() )
	{
		position++;
	}
}
