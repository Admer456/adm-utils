
#include "Precompiled.hpp"
using namespace adm;

Lexer::Lexer()
{
	Clear();
}

Lexer::Lexer( Lexer&& other ) noexcept
{
	Clear();
	Load( other.buffer );
}

Lexer::Lexer( const Lexer& other )
{
	Clear();
	Load( other.buffer );
}

Lexer::Lexer( std::fstream& fileStream )
{
	std::string line, text;
	std::istringstream sstream;
	while ( std::getline( fileStream, line ) )
	{
		sstream >> line;
	}

	text = sstream.str();
	Load( text );
}

Lexer::Lexer( std::ifstream& fileStream )
{
	std::string line, text;
	std::istringstream sstream;
	while ( std::getline( fileStream, line ) )
	{
		sstream >> line;
	}

	text = sstream.str();
	Load( text );
}

Lexer::~Lexer()
{
	Clear();
}

Lexer::Lexer( const char* text )
{
	Load( text );
}

Lexer::Lexer( std::string_view text )
{
	Load( text );
}

void Lexer::Clear()
{
	position = 0;
	
	buffer.clear();
}

void Lexer::Load( const char* text )
{
	buffer = text;
}

void Lexer::Load( std::string_view text )
{
	buffer = text;
}

std::string Lexer::Next()
{
	std::string result;

	while ( CanAdvance() )
	{
		if ( IsComment() )
		{
			NewLine();
			continue;
		}

		if ( CanAdd() )
		{
			result += view[position];
		}

		position++;
	}

	return result;
}

bool Lexer::Expect( const char* expectedToken)
{
	return Next() == expectedToken;
}

bool Lexer::CanAdd() const
{
	const char& c = view[position];

	return CanAdvance() && c != '"';
}

bool Lexer::CanAdvance() const
{
	const char& c = view[position];

	if ( inQuote )
	{
		return !IsEndOfFile();
	}

	return c != ' ' && c != '\t' && c != '\0' && !IsEndOfLine() && !IsEndOfFile();
}

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

bool Lexer::IsEndOfLine() const
{
	return view[position] == '\n';
}

bool Lexer::IsEndOfFile() const
{
	return position >= buffer.size();
}

void Lexer::NewLine()
{
	size_t newPosition = view.find( '\n', position+1 );
	if ( newPosition == std::string::npos )
	{
		position = view.size() + 1;
		return;
	}

	position = newPosition + 1;
}
