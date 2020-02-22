#pragma once

constexpr auto delimiter = L" \n\t";
#define DELIMITER delimiter

constexpr size_t utf8_sequence_maxlen = 6;
#define UTF8_SEQUENCE_MAXLEN utf8_sequence_maxlen

constexpr const char* get_wchar_t_platform_encoding()
{
  switch (sizeof(wchar_t))
  {
  case 1:
    return "UTF-8";
  case 2:
    return "UTF-16LE";
  case 4:
    return "UTF-32LE";
  default:
    return "";
  }
};

#define WCHAR_T_PLATFORM_ENCODING get_wchar_t_platform_encoding()
