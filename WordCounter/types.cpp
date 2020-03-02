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

void unicode_string::assign(const char* instr)
{
	_symbols.clear();
	size_t size = strlen(instr) + 1;
	_symbols.reserve(size / 4);
	for (uint32_t i = 0; i < size; i += 4)
	{
		symbol s0 = instr[i + 0], s1 = instr[i + 1], s2 = instr[i + 2], s3 = instr[i + 3];
		symbol val = s0 | (s1 << 8) | (s2 << 16) << (s3 << 24);

		_symbols.push_back(val);
	}
}

void unicode_string::assign(const std::string& instr)
{
	assign(instr.c_str());
}

void unicode_string::assign(const wchar_t* instr)
{
	switch (sizeof(wchar_t))
	{
	case 1:
		assign((const char*)instr);
		break;
	case 2:
		return "UTF-16LE";
	case 4:
		return "UTF-32LE";
	default:
		return "";
	}
}

void unicode_string::assign(const std::wstring& instr)
{
	assign(instr.c_str());
}

void unicode_string::clear()
{
	_symbols.clear();
}

size_t unicode_string::size() const
{
	return _symbols.size();
}
