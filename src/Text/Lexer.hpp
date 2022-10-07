// SPDX-FileCopyrightText: 2022 Admer Šuko
// SPDX-License-Identifier: MIT

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
	public: // Delimiter presets
		// If you're parsing a language with expressions
		static constexpr const char* DelimitersFull = "()[]{}.:,;=+-*/&@'?";
		// If you're parsing simple text data
		static constexpr const char* DelimitersSimple = "()[]{}:,;";
	
		static constexpr const char* DelimitersDefault = DelimitersSimple;

	public:
		Lexer();
		Lexer( Lexer&& other ) noexcept;
		Lexer( const Lexer& other );
		// From files
		Lexer( std::fstream& fileStream );
		Lexer( std::ifstream& fileStream );
		// From raw text
		Lexer( const char* text );
		Lexer( StringView text );
		~Lexer();

		// Wipes the buffer
		void			Clear();
		// Loads the lexer with text
		void			Load( const char* text );
		// Loads the lexer with text
		void			Load( StringView text );
		// Delimiters are individual characters that can separate tokens,
		// they are tokens themselves. Default is Lexer::DelimitersSimple
		void			SetDelimiters( const char* delimiters );

		// Gets the next token and advances
		std::string		Next( bool withDelimiter = false );
		// Compares the next token to expectedToken, optionally
		// advancing the position in the buffer
		bool			Expect( const char* expectedToken, bool advance = false );
		// @returns Whether EOF has been reached or not
		bool			IsEndOfFile() const;

	private:
		bool			CanAdd() const;
		bool			CanAdvance() const;
		
		bool			IsComment() const;
		inline bool		IsEndOfLine() const;
		inline bool		IsDelimiter() const;

		void			NewLine();
		inline void		ToggleQuoteMode();
		inline void		IncrementPosition();

	private:
		size_t			position{ 0 }; // position in the buffer
		bool			inQuote{ false };

		String			buffer;
		StringView		view;
		String			delimiterString{ DelimitersDefault };
	};
}
