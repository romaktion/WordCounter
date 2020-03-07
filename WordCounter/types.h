#pragma once

struct parse_result
{
	unsigned symbol_amount = 0;
	std::map<std::wstring, unsigned> words_amount;

	parse_result();
	parse_result(unsigned symbol_amount, std::map<std::wstring, unsigned> words_amount);
};
