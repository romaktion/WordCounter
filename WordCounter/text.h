#pragma once


class text
{
public:
  text(const char* byte_string, const char* encoding);
  text(const wchar_t* wide_string);

  const std::wstring& wide_string() const;
  const std::string& byte_string() const;

private:
  std::string cached_byte_string;
  std::wstring cached_wide_string;
};
