#pragma once

#include <stack>
#include <string>
#include <string_view>
#include <algorithm>

// @warning - The functions that are taking a string_view aren't supposed to be used in the multi-threaded situation, since string_vie holds the pointer to the string, which may have been deleted
namespace util{
	namespace converter
	{
		namespace __private
		{
			enum class Priority: uint8_t{
				INVALID = 0, LOW, MID, HIGH, HIGHEST, KANEKI	// kaneki is more like infinity here
			};

			inline Priority getPriority(char opr){
				switch (opr)
				{
					case '(': return Priority::KANEKI;
					case '^':	return Priority::HIGHEST;	// considered as `power`
					case '%':
					case '*':
					case '\\':
					case '/':	return Priority::HIGH;
					case '+':
					case '-':	return Priority::MID;
					default:	return Priority::INVALID;
				}
			}

			inline bool isOperator(char ch){
				return ch == '^' || ch == '%' || ch == '*' || ch == '/' || ch == '\\' || ch == '+' || ch == '-';
			}

			// by default spaces will be inserted by this, since that is to be expected for strings
			std::string numeric_inToPost(std::string_view);
		} // namespace __private

		/**
		 * @warning - This function will only process string, considering variables in it it, with single character character names
		 * @tip - You may first pass the string with variable to this, then use std::replace or something to replace the variables with the numerical values you want
		 * @param - string_view: infix expression
		 * 			bool: spaced    (`spaced` is a bool showing, whether spaces should be inserted in string, or not, by default true)
		*/
		std::string inToPost(std::string_view, bool spaced = true);
		// std::string preToPost(std::string_view);
		// std::string postToPre(std::string_view);

	} // namespace converter
	namespace evaluator
	{
		/**
		 * @brief - Gives out an integer value after evaluating the postfix expression passed
		*/
		int32_t evalPostFix( std::string_view );
	} // namespace evaluator
		
}

std::string util::converter::__private::numeric_inToPost( std::string_view infix ){
	std::stack < char > stack;
	std::string postfix;
	postfix.reserve(2*infix.size() - 1);		// size-1    space for spaces

	char ch;
	for (auto i = 0; i < infix.size(); ++i)
	{
		ch = infix[i];
		if( std::isspace(ch) )	continue;	// skip whitespaces

		if( ch == '(' ){
			stack.push(ch);
		}else if ( ch == ')' )
		{
			while ( ! stack.empty() && stack.top() != '(' )
			{
				postfix.push_back( stack.top() );
				postfix.push_back(' ');	// pushing a space
				stack.pop();
			}
			if( stack.top() == '(' )	stack.pop();
		}else if ( __private::isOperator( ch ) )
		{
			while ( ! stack.empty() && ( __private::getPriority( stack.top() ) >= __private::getPriority( ch ) ) && stack.top() != '(' )
			{
				postfix.push_back( stack.top() );
				postfix.push_back(' ');	// pushing a space
				stack.pop();
			}
			stack.push( ch );
		}else
		{	// NUMBER OR operand
			/* @logic of below for loop -> 
				for eg. these three expressions ->    A+65+C		(multiple digit number in b/w)
													  A+65 +C		(space in b/w)
													  (A+65)+C		(parenthesis in b/w)
				What this loop tries to do is... KEEP READING UNTIL YOU GET either a space, parenthesis OR an operator   (so "A" , "65" , "C"   are treated as 3 single units, and hence no space between those, for eg. no space b/w '6' and '5')
													Till then whatever is read is considered as single unit, hence even variable names as well as numbers can be read as well
			*/
			for ( ; infix[i] != '(' && infix[i] != ')' && ( !isspace(infix[i]) && !isOperator(infix[i]) ) && i < infix.size(); ++i ){
				postfix.push_back( infix[i] );
			}
			--i;	// the above for loop MUST DEFINITELY RUN atleast once, or else this line may cause infinite loop

			postfix.push_back(' ');	// pushing a space
		}
	}
	while ( !stack.empty() ){
		postfix.push_back( stack.top() );
		postfix.push_back(' ');	// pushing a space
		stack.pop();
	}
	postfix.pop_back();
	postfix.shrink_to_fit();

	return postfix;
}

std::string util::converter::inToPost( std::string_view infix, bool spaced){
	std::stack < char > stack;
	std::string postfix;
	postfix.reserve(2*infix.size() - 1);		// size-1    space for spaces
	if( std::any_of(infix.begin(), infix.end(), [](char ch){	return ch >= '0' && ch <= '9';	}) ){
		return __private::numeric_inToPost( infix );	// this function only has a 1 LINE DIFFERENCE, but i wanted to use the for range, as well as keep it more segregated
	}

	for (auto &&ch : infix){
		if( std::isspace(ch) )	continue;	// skip whitespaces

		if( ch == '(' ){
			stack.push('(');
		}else if ( ch == ')' )
		{
			while ( ! stack.empty() && stack.top() != '(' )
			{
				postfix.push_back( stack.top() );
				if(spaced) postfix.push_back(' ');	// pushing a space
				stack.pop();
			}
			if( stack.top() == '(' )	stack.pop();
		}else if ( __private::isOperator( ch ) )
		{
			while ( ! stack.empty() && ( __private::getPriority( stack.top() ) >= __private::getPriority( ch ) ) && stack.top() != '(' )
			{
				postfix.push_back( stack.top() );
				if(spaced) postfix.push_back(' ');	// pushing a space
				stack.pop();
			}
			stack.push( ch );
		}else
		{	// operand or NUMBER
			postfix.push_back( ch );
			if(spaced) postfix.push_back(' ');	// pushing a space
		}
	}
	while ( !stack.empty() ){
		postfix.push_back( stack.top() );
		if(spaced) postfix.push_back(' ');	// pushing a space
		stack.pop();
	}

	if(postfix.back() == ' ') postfix.pop_back();	// removing the last space inserted
	postfix.shrink_to_fit();

	return postfix;
}
