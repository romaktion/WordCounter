#pragma once


class text
{
public:
  text(const char* byte_string, const char* encoding);
  text(const wchar_t* wide_string);
  text(const char* byte_string);

  const std::wstring& wide_string() const;
  const std::string& byte_string() const;

  text* operator -> ();

  symbol& operator [] (int index);
  const symbol& operator [] (int index) const;

private:
  void _iconv(const char* instr, const char* in_encode, std::string& outstr, const char* out_encode);
  void _iconv(const char* instr, const char* in_encode, std::wstring& outstr, const char* out_encode);
  void _iconv(const wchar_t* instr, const char* in_encode, std::string& outstr, const char* out_encode);
  void _iconv(const wchar_t* instr, const char* in_encode, std::wstring& outstr, const char* out_encode);

  void _iconv_internal(const char* instr, const char* in_encode, char** outstr, const char* out_encode);
  void _iconv_internal(const wchar_t* instr, const char* in_encode, char** outstr, const char* out_encode);

  mutable std::string cached_byte_string;
  mutable std::wstring cached_wide_string;
  mutable unicode_string cached_unicode_string;
};
