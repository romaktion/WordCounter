#include "pch.h"
#include "text.h"
#include "iconv.h"
#include <typeinfo>
#include <signal.h>

text::text(const char* byte_string, const char* encoding)
{
  _iconv(byte_string, encoding, cached_byte_string, "UTF-8");
}

text::text(const wchar_t* wide_string)
{
  _iconv(wide_string, WCHAR_T_PLATFORM_ENCODING, cached_byte_string, "UTF-8");
}

text::text(const char* byte_string)
{
  _iconv(byte_string, "", cached_wide_string, WCHAR_T_PLATFORM_ENCODING);
}

const std::wstring& text::wide_string() const
{
  if (cached_wide_string.empty() && !cached_byte_string.empty())
    _iconv(cached_byte_string.c_str(), "", cached_wide_string, WCHAR_T_PLATFORM_ENCODING);

  return cached_wide_string;
}

const std::string& text::byte_string() const
{
  if (cached_byte_string.empty() && !cached_wide_string.empty())
    _iconv(cached_wide_string.c_str(), WCHAR_T_PLATFORM_ENCODING, cached_byte_string, "UTF-8");

  return cached_byte_string;
}

const symbol& text::operator[](int index) const
{
  if (cached_unicode_string.size() <= 0)
  {
    if (cached_byte_string.size() <= 0)
      _iconv(cached_wide_string.c_str(), WCHAR_T_PLATFORM_ENCODING, cached_byte_string_UTF_32, "UTF-32");

    cached_unicode_string.assign(cached_byte_string);
  }
  
  return cached_unicode_string[index];
}

symbol& text::operator[](int index)
{
  if (cached_unicode_string.size() <= 0)
  {
    if (cached_byte_string.size() <= 0)
      _iconv(cached_wide_string.c_str(), WCHAR_T_PLATFORM_ENCODING, cached_byte_string_UTF_32, "UTF-32LE");

    cached_unicode_string.assign(cached_byte_string);
  }
  
  return cached_unicode_string[index];
}

text* text::text::operator->()
{
  cached_byte_string.clear();
  cached_wide_string.clear();
  cached_unicode_string.clear();

  return this;
}

void text::_iconv(const char* instr, const char* in_encode, std::string& outstr, const char* out_encode) const
{
  char* res = nullptr;
  size_t insize = strlen(instr) + 1;
  size_t outsize = insize * UTF8_SEQUENCE_MAXLEN;
  _iconv_internal(instr, in_encode, insize, &res, out_encode, outsize);
  outstr.assign(res);
  delete[] res;
}

void text::_iconv(const char* instr, const char* in_encode, std::wstring& outstr, const char* out_encode) const
{
  char* res = nullptr;
  size_t insize = strlen(instr) + 1;
  size_t outsize = insize * UTF8_SEQUENCE_MAXLEN;
  _iconv_internal(instr, in_encode, insize, &res, out_encode, outsize);
  outstr.assign((wchar_t*)res);
  delete[] res;
}

void text::_iconv(const wchar_t* instr, const char* in_encode, std::string& outstr, const char* out_encode) const
{
  char* res = nullptr;
  size_t insize = (wcslen(instr) + 1) * sizeof(wchar_t);
  size_t outsize = (wcslen(instr) + 1) * UTF8_SEQUENCE_MAXLEN;
  _iconv_internal((const char*)instr, in_encode, insize, &res, out_encode, outsize);
  outstr.assign(res);
  delete[] res;
}

void text::_iconv(const wchar_t* instr, const char* in_encode, std::wstring& outstr, const char* out_encode) const
{
  char* res = nullptr;
  size_t insize = (wcslen(instr) + 1) * UTF8_SEQUENCE_MAXLEN;
  size_t outsize = insize;
  _iconv_internal((const char*)instr, in_encode, insize, &res, out_encode, outsize);
  outstr.assign((wchar_t*)res);
  delete[] res;
}

void text::_iconv_internal(const char* instr, const char* in_encode, size_t& insize, char** outstr, const char* out_encode, size_t& outsize) const
{
  auto result = new char[outsize];

#ifdef __linux__
  auto inptr = (char*)bytestr;
#elif _WIN32
  auto inptr = (const char*)instr;
#else
#endif

  auto cd = iconv_open(out_encode, in_encode);
  if (cd == (iconv_t)-1)
    if (errno == EINVAL)
      std::cerr << "conversion from " << in_encode << " to " << out_encode << " not available\n";
    else
      std::cerr << "iconv_open\n";

  auto outptr = result;
  auto nconv = iconv(cd, &inptr, &insize, &outptr, &outsize);
  if (nconv == (size_t)-1)
    if (errno == EINVAL)
      std::cerr << "This is harmless.\n";
    else
      std::cerr << "It is a real problem!\n";

  iconv_close(cd);

  *outstr = result;
}
