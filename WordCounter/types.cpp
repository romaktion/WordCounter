#include "pch.h"
#include "types.h"

parse_result::parse_result() : symbol_amount(0)
{
}

parse_result::parse_result(int in_symbol_amount, std::map<std::string, unsigned> in_words_amount)
{
	symbol_amount = in_symbol_amount;
	words_amount = in_words_amount;
}
