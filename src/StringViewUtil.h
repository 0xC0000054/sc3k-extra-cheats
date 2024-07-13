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

#pragma once
#include <string_view>
#include <vector>

namespace StringViewUtil
{
	void Split(
		const std::string_view& input,
		std::string_view::value_type delim,
		std::vector<std::string_view>& results);

	bool TryParse(
		const std::string_view& input,
		uint32_t& output);

	bool TryParse(
		const std::string_view& input,
		int64_t& output);
}