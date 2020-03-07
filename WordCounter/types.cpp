#include "pch.h"
#include "types.h"

parse_result::parse_result()
{
}

parse_result::parse_result(unsigned in_symbol_amount, std::map<std::wstring, unsigned> in_words_amount)
{
	symbol_amount = in_symbol_amount;
	words_amount = in_words_amount;
}
