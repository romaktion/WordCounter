#include "pch.h"
#include "text.h"
#include "iconv.h"

#define UTF8_SEQUENCE_MAXLEN 6

text::text(const char* byte_string, const char* encoding)
{
  size_t srclen = strlen(byte_string) + 1;
  size_t outlen = srclen * UTF8_SEQUENCE_MAXLEN;
  auto utf8 = new char[outlen];

#ifdef __linux__
  auto inptr = (char*)byte_string;
#elif _WIN32
  auto inptr = (const char*)byte_string;
#else

#endif

  auto outptr = utf8;

  auto cd = iconv_open("UTF-8", encoding);
  if (cd == (iconv_t)-1)
  {
    if (errno == EINVAL)
      std::cerr << "conversion from " << encoding <<" to UTF-8 not available";
    else
      perror("iconv_open");
  }
  
  auto nconv = iconv(cd, &inptr, &srclen, &outptr, &outlen);
  if (nconv == (size_t)-1)
  {
    if (errno == EINVAL)
      std::cerr << "This is harmless.";
    else
      std::cerr << "It is a real problem!";
  }
  iconv_close(cd);

  cached_byte_string.assign(utf8);

  delete[] utf8;
}

text::text(const wchar_t* wide_string)
{
  auto inenc = std::make_unique<char*>();

  std::string in_encoding;

  switch (sizeof(wchar_t))
  {
  case 1:
  {
    in_encoding = "UTF-8";
    break;
  }
  case 2:
  {
    in_encoding = "UTF-16LE";
    break;
  }
  case 4:
  {
    in_encoding = "UTF-32LE";
    break;
  }
  default:
    break;
  }

  size_t srclen = (wcslen(wide_string) + 1) * sizeof(wchar_t);
  size_t outlen = (wcslen(wide_string) + 1) * UTF8_SEQUENCE_MAXLEN;
  
  auto utf8 = new char[outlen];

#ifdef __linux__
  auto inptr = (char*)wide_string;
#elif _WIN32
  auto inptr = (const char*)wide_string;
#else

#endif

  auto outptr = (char*)&utf8[0];

  auto cd = iconv_open("UTF-8", in_encoding.c_str());
  if (cd == (iconv_t)-1)
  {
    if (errno == EINVAL)
      std::cerr << "conversion from " << in_encoding.c_str() << " WCCHAR_T to UTF-8 not available";
    else
      perror("iconv_open");
  }

  auto nconv = iconv(cd, &inptr, &srclen, &outptr, &outlen);
  if (nconv == (size_t)-1)
  {
    if (errno == EINVAL)
      std::cerr << "This is harmless.";
    else
      std::cerr << "It is a real problem!";
  }
  iconv_close(cd);

  cached_byte_string.assign(utf8);

  delete[] utf8;
}

const std::wstring& text::wide_string() const
{
  return cached_wide_string;
}

const std::string& text::byte_string() const
{
  return cached_byte_string;
}
