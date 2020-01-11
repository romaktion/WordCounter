#pragma once

struct parse_result
{
	int symbol_amount;
	std::map<std::string, unsigned> words_amount;

	parse_result();
	parse_result(int symbol_amount, std::map<std::string, unsigned> words_amount);
};
