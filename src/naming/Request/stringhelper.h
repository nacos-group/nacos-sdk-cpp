#pragma once

//----------------------------------------------------------------------------------------
//	Copyright © 2004 - 2020 Tangible Software Solutions, Inc.
//	This class can be used by anyone provided that the copyright notice remains intact.
//
//	This class is used to replace some string methods, including
//	conversions to or from strings.
//----------------------------------------------------------------------------------------
#include <string>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <cctype>
#include <algorithm>

class StringHelper
{
public:
	static std::wstring toLower(std::wstring source)
	{
		std::transform(source.begin(), source.end(), source.begin(), [](unsigned char c){ return std::tolower(c); });
		return source;
	}

	static std::wstring toUpper(std::wstring source)
	{
		std::transform(source.begin(), source.end(), source.begin(), [](unsigned char c){ return std::toupper(c); });
		return source;
	}

	static std::wstring trimStart(std::wstring source, const std::wstring &trimChars = L" \t\n\r\v\f")
	{
		return source.erase(0, source.find_first_not_of(trimChars));
	}

	static std::wstring trimEnd(std::wstring source, const std::wstring &trimChars = L" \t\n\r\v\f")
	{
		return source.erase(source.find_last_not_of(trimChars) + 1);
	}

	static std::wstring trim(std::wstring source, const std::wstring &trimChars = L" \t\n\r\v\f")
	{
		return trimStart(trimEnd(source, trimChars), trimChars);
	}

	static std::wstring replace(std::wstring source, const std::wstring &find, const std::wstring &replace)
	{
		std::size_t pos = 0;
		while ((pos = source.find(find, pos)) != std::wstring::npos)
		{
			source.replace(pos, find.length(), replace);
			pos += replace.length();
		}
		return source;
	}

	static bool startsWith(const std::wstring &source, const std::wstring &value)
	{
		if (source.length() < value.length())
			return false;
		else
			return source.compare(0, value.length(), value) == 0;
	}

	static bool endsWith(const std::wstring &source, const std::wstring &value)
	{
		if (source.length() < value.length())
			return false;
		else
			return source.compare(source.length() - value.length(), value.length(), value) == 0;
	}

	static std::vector<std::wstring> split(const std::wstring &source, wchar_t delimiter)
	{
		std::vector<std::wstring> output;
		std::wistringstream ss(source);
		std::wstring nextItem;

		while (std::getline(ss, nextItem, delimiter))
		{
			output.push_back(nextItem);
		}

		return output;
	}

	template<typename T>
	static std::wstring toString(const T &subject)
	{
		std::wostringstream ss;
		ss << subject;
		return ss.str();
	}

	template<typename T>
	static T fromString(const std::wstring &subject)
	{
		std::wistringstream ss(subject);
		T target;
		ss >> target;
		return target;
	}

	static bool isEmptyOrWhiteSpace(const std::wstring &source)
	{
		if (source.length() == 0)
			return true;
		else
		{
			for (std::size_t index = 0; index < source.length(); index++)
			{
				if (!std::isspace(source[index]))
					return false;
			}

			return true;
		}
	}

	template<typename T>
	static std::wstring formatSimple(const std::wstring &input, T arg0)
	{
		std::wostringstream ss;
		std::size_t lastCloseBrace = std::wstring::npos;
		std::size_t openBrace = std::wstring::npos;
		while ((openBrace = input.find(L'{', openBrace + 1)) != std::wstring::npos)
		{
			if (openBrace + 1 < input.length())
			{
				if (input[openBrace + 1] == L'{')
				{
					openBrace++;
					continue;
				}

				std::size_t closeBrace = input.find(L'}', openBrace + 1);
				if (closeBrace != std::wstring::npos)
				{
					ss << input.substr(lastCloseBrace + 1, openBrace - lastCloseBrace - 1);
					lastCloseBrace = closeBrace;

					std::wstring index = trim(input.substr(openBrace + 1, closeBrace - openBrace - 1));
					if (index == L"0")
						ss << arg0;
					else
						throw std::runtime_error("Only simple positional format specifiers are handled by the 'formatSimple' helper method.");
				}
			}
		}

		if (lastCloseBrace + 1 < input.length())
			ss << input.substr(lastCloseBrace + 1);

		return ss.str();
	}

	template<typename T1, typename T2>
	static std::wstring formatSimple(const std::wstring &input, T1 arg0, T2 arg1)
	{
		std::wostringstream ss;
		std::size_t lastCloseBrace = std::wstring::npos;
		std::size_t openBrace = std::wstring::npos;
		while ((openBrace = input.find(L'{', openBrace + 1)) != std::wstring::npos)
		{
			if (openBrace + 1 < input.length())
			{
				if (input[openBrace + 1] == L'{')
				{
					openBrace++;
					continue;
				}

				std::size_t closeBrace = input.find(L'}', openBrace + 1);
				if (closeBrace != std::wstring::npos)
				{
					ss << input.substr(lastCloseBrace + 1, openBrace - lastCloseBrace - 1);
					lastCloseBrace = closeBrace;

					std::wstring index = trim(input.substr(openBrace + 1, closeBrace - openBrace - 1));
					if (index == L"0")
						ss << arg0;
					else if (index == L"1")
						ss << arg1;
					else
						throw std::runtime_error("Only simple positional format specifiers are handled by the 'formatSimple' helper method.");
				}
			}
		}

		if (lastCloseBrace + 1 < input.length())
			ss << input.substr(lastCloseBrace + 1);

		return ss.str();
	}

	template<typename T1, typename T2, typename T3>
	static std::wstring formatSimple(const std::wstring &input, T1 arg0, T2 arg1, T3 arg2)
	{
		std::wostringstream ss;
		std::size_t lastCloseBrace = std::wstring::npos;
		std::size_t openBrace = std::wstring::npos;
		while ((openBrace = input.find(L'{', openBrace + 1)) != std::wstring::npos)
		{
			if (openBrace + 1 < input.length())
			{
				if (input[openBrace + 1] == L'{')
				{
					openBrace++;
					continue;
				}

				std::size_t closeBrace = input.find(L'}', openBrace + 1);
				if (closeBrace != std::wstring::npos)
				{
					ss << input.substr(lastCloseBrace + 1, openBrace - lastCloseBrace - 1);
					lastCloseBrace = closeBrace;

					std::wstring index = trim(input.substr(openBrace + 1, closeBrace - openBrace - 1));
					if (index == L"0")
						ss << arg0;
					else if (index == L"1")
						ss << arg1;
					else if (index == L"2")
						ss << arg2;
					else
						throw std::runtime_error("Only simple positional format specifiers are handled by the 'formatSimple' helper method.");
				}
			}
		}

		if (lastCloseBrace + 1 < input.length())
			ss << input.substr(lastCloseBrace + 1);

		return ss.str();
	}
};
