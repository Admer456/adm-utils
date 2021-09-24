
#pragma once

namespace adm
{	
	constexpr size_t NoPosition = ~0U;
	
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

		void			Clear();
		void			Load( const char* text );
		void			Load( std::string_view text );

		// Gets the next token
		std::string		Next();
		bool			Expect( const char* expectedToken );

	private:
		bool			CanAdd() const;
		bool			CanAdvance() const;
		
		bool			IsComment() const;
		bool			IsEndOfLine() const;
		bool			IsEndOfFile() const;

		void			NewLine();

	private:
		size_t			position{ 0 }; // position in the line
		bool			inQuote{ false };

		std::string		buffer;
		std::string_view view;
	};
}
