#pragma once

typedef int32_t symbol;

struct parse_result
{
	unsigned symbol_amount = 0;
	std::map<std::wstring, unsigned> words_amount;

	parse_result();
	parse_result(unsigned symbol_amount, std::map<std::wstring, unsigned> words_amount);
};

struct unicode_string
{
public:
	void assign(const char* instr);
	void assign(const std::string& instr);
	void clear();
	size_t size() const;

	symbol& operator [] (int index);
	const symbol& operator [] (int index) const;
private:
	std::vector<int32_t> _symbols;
};
