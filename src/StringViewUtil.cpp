////////////////////////////////////////////////////////////////////////////
//
// This file is part of sc3k-extra-cheats, a DLL Plugin for SimCity 3000
// that adds new cheat codes.
//
// Copyright (c) 2024 Nicholas Hayes
//
// This file is licensed under terms of the MIT License.
// See LICENSE.txt for more information.
//
////////////////////////////////////////////////////////////////////////////

#include "StringViewUtil.h"
#include <charconv>

void StringViewUtil::Split(
	const std::string_view& input,
	std::string_view::value_type delim,
	std::vector<std::string_view>& results)
{
	// The following code is adapted from: https://stackoverflow.com/a/36301144

	const size_t inputLength = input.length();
	bool foundDoubleQuote = false;
	bool foundSingleQuote = false;
	size_t argumentLength = 0;

	for (size_t i = 0; i < inputLength; i++)
	{
		size_t start = i;
		if (input[i] == '\"')
		{
			foundDoubleQuote = true;
		}
		else if (input[i] == '\'')
		{
			foundSingleQuote = true;
		}

		if (foundDoubleQuote)
		{
			i++;
			start++;

			while (i < inputLength && input[i] != '\"')
			{
				i++;
			}

			if (i < inputLength)
			{
				foundDoubleQuote = false;
			}

			argumentLength = i - start;
			i++;
		}
		else if (foundSingleQuote)
		{
			i++;
			start++;

			while (i < inputLength && input[i] != '\'')
			{
				i++;
			}

			if (i < inputLength)
			{
				foundSingleQuote = false;
			}

			argumentLength = i - start;
			i++;
		}
		else
		{
			while (i < inputLength && input[i] != delim)
			{
				i++;
			}
			argumentLength = i - start;
		}

		if (argumentLength > 0)
		{
			results.push_back(input.substr(start, argumentLength));
		}
	}
}

bool StringViewUtil::TryParse(const std::string_view& input, uint32_t& output)
{
	const char* const start = input.data();
	const char* const end = start + input.size();

	const std::from_chars_result result = std::from_chars(start, end, output);

	if (result.ec == std::errc{} && result.ptr == end)
	{
		return true;
	}

	output = 0;
	return false;
}

bool StringViewUtil::TryParse(const std::string_view& input, int64_t& output)
{
	const char* const start = input.data();
	const char* const end = start + input.size();

	const std::from_chars_result result = std::from_chars(start, end, output);

	if (result.ec == std::errc{} && result.ptr == end)
	{
		return true;
	}

	output = 0;
	return false;
}


