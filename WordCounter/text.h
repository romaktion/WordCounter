#pragma once


class text
{
public:
  text(const char* byte_string, const char* encoding) {
    _iconv(byte_string, encoding, cached_byte_string, "UTF-8"); }
  text(const wchar_t* wide_string) {
    _iconv(wide_string, WCHAR_T_PLATFORM_ENCODING, cached_byte_string, "UTF-8"); }
  text(const char* byte_string) {
    _iconv(byte_string, "CHAR", cached_wide_string, WCHAR_T_PLATFORM_ENCODING); }

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

  bool is_surrogate(wchar_t uc) const { return (uc - 0xd800u) < 2048u; }
  bool is_high_surrogate(wchar_t uc) const { return (uc & 0xfffffc00) == 0xd800; }
  bool is_low_surrogate(wchar_t uc) const { return (uc & 0xfffffc00) == 0xdc00; }
  char32_t surrogate_to_utf32(wchar_t high, wchar_t low) const { return (high << 10) + low - 0x35fdc00; }

  //cached strings
  mutable std::string cached_byte_string;
  mutable std::wstring cached_wide_string;
  mutable std::u32string cached_unicode_string;
};
