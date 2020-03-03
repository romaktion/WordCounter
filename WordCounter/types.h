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
	void assign(const char* instr, size_t insize);
	void assign(const std::string& instr, size_t insize);
	void assign(const wchar_t* instr, size_t insize);
	void assign(const std::wstring& instr, size_t insize);
	void clear();
	size_t size() const;

	symbol& operator [] (int index);
	const symbol& operator [] (int index) const;
private:
	std::basic_string<symbol> _symbols;
};
