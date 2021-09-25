
#pragma once

namespace adm
{	
	constexpr bool DebugLexer = false;
	constexpr size_t NoPosition = ~0U;
	
	// ============================
	// Lexer
	// 
	// A text-parsing utility
	// Usage:
	// 
	// Lexer lex( "buncha text here" );
	// std::string token;
	// 
	// token = lex.Next(); // buncha
	// token = lex.Next(); // text
	// token = lex.Next(); // here
	// ============================
	class Lexer final
	{
	public:
		Lexer();
		Lexer( Lexer&& other ) noexcept;
		Lexer( const Lexer& other );
		Lexer( std::fstream& fileStream );
		Lexer( std::ifstream& fileStream );
		~Lexer();

		Lexer( const char* text );
		Lexer( std::string_view text );

		// Wipes the buffer
		void			Clear();
		// Loads the lexer with text
		void			Load( const char* text );
		// Loads the lexer with text
		void			Load( std::string_view text );

		// Gets the next token and advances
		std::string		Next();
		// Compares the next token to expectedToken, optionally
		// advancing the position in the buffer
		bool			Expect( const char* expectedToken, bool advance = false );
		// @returns Whether EOF has been reached or not
		bool			IsEndOfFile() const;

	private:
		bool			CanAdd() const;
		bool			CanAdvance() const;
		
		bool			IsComment() const;
		bool			IsEndOfLine() const;

		void			NewLine();
		void			ToggleQuoteMode();
		void			IncrementPosition();

	private:
		size_t			position{ 0 }; // position in the buffer
		bool			inQuote{ false };

		std::string		buffer;
		std::string_view view;
	};
}
