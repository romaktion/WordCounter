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

symbol& unicode_string::operator[](int index)
{
	return _symbols[index];
}

symbol const& unicode_string::operator[](int index) const
{
	return _symbols[index];
}

void unicode_string::assign(const char* instr, size_t insize)
{
	_symbols.clear();
	_symbols.reserve(insize / 4);
	for (uint32_t i = 0; i < insize; i += 4)
	{
		int32_t c0 = instr[i + 3], c1 = instr[i + 1], c2 = instr[i + 2], c3 = instr[i + 3];
		int32_t val = c0 | (c1 << 8) | (c2 << 16) | (c3 << 24);

		printf("%d\n", val);
		_symbols.push_back(val);
	}
}

void unicode_string::assign(const std::string& instr, size_t insize)
{
	assign(instr.c_str(), insize);
}

void unicode_string::assign(const wchar_t* instr, size_t insize)
{
	switch (sizeof(wchar_t))
	{
	case 1:
		break;
	case 2:
		break;
	case 4:
		break;
	default:
		break;
	}
}

void unicode_string::assign(const std::wstring& instr, size_t insize)
{
	assign(instr.c_str(), insize);
}

void unicode_string::clear()
{
	_symbols.clear();
}

size_t unicode_string::size() const
{
	return _symbols.size();
}
