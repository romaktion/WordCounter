#pragma once


class text
{
public:
  text() {}
  text(const char* byte_string, const std::string& encoding);
  text(const wchar_t* wide_string);

  //assign
  void assign(const char* byte_string, const std::string& encoding);
  void assign(const wchar_t* wide_string);

  //getters
  const std::wstring& wide_string() const;
  const std::string& byte_string() const;
  const std::u32string& unicode_string() const;

  //operator overloads
  text* operator -> ();
  const char32_t& operator [] (int index) const;
  char32_t& operator [] (int index);

private:
  //iconv
  size_t _iconv(const char* instr, const char* in_encode, std::string& outstr, const char* out_encode) const;
  size_t _iconv(const char* instr, const char* in_encode, std::wstring& outstr, const char* out_encode) const;
  size_t _iconv(const wchar_t* instr, const char* in_encode, std::string& outstr, const char* out_encode) const;
  size_t _iconv(const wchar_t* instr, const char* in_encode, std::wstring& outstr, const char* out_encode) const;
  
  //std::string overloads
  size_t _iconv(const std::string& instr, const char* in_encode, std::string& outstr, const char* out_encode) const {
    return _iconv(instr.c_str(), in_encode, outstr, out_encode); }
  size_t _iconv(const std::string& instr, const char* in_encode, std::wstring& outstr, const char* out_encode) const {
    return _iconv(instr.c_str(), in_encode, outstr, out_encode); }
  size_t _iconv(const std::wstring& instr, const char* in_encode, std::string& outstr, const char* out_encode) const {
    return _iconv(instr.c_str(), in_encode, outstr, out_encode); }
  size_t _iconv(const std::wstring& instr, const char* in_encode, std::wstring& outstr, const char* out_encode) const {
    return _iconv(instr.c_str(), in_encode, outstr, out_encode);
  }

  size_t _iconv_internal(const char* instr, const char* in_encode, size_t& insize,
    char** outstr, const char* out_encode, size_t& outsize) const;

  //UTF-16 to UTF-32 non iconv
  void convert_utf16_to_utf32(const wchar_t* instr, std::u32string& outstr) const;
  void convert_utf16_to_utf32(const std::wstring& instr, std::u32string& outstr) const {
    convert_utf16_to_utf32(instr.c_str(), outstr);}

  bool is_surrogate(const wchar_t& uc) const { return (uc - 0xd800u) < 2048u; }
  bool is_high_surrogate(const wchar_t& uc) const { return (uc & 0xfffffc00) == 0xd800; }
  bool is_low_surrogate(const wchar_t& uc) const { return (uc & 0xfffffc00) == 0xdc00; }
  char32_t surrogate_to_utf32(const wchar_t& high, const wchar_t& low) const { return (high << 10) + low - 0x35fdc00; }

  //cached strings
  mutable std::string cached_byte_string;
  mutable std::wstring cached_wide_string;
  mutable std::u32string cached_unicode_string;

  //init data
  std::string init_byte_string;
  std::string init_byte_encoding;
};
