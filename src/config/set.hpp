// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <string>
#include <unordered_map>

#include <utility/string_hash.hpp>

namespace pd::config
{
	template<typename Value>
	class Set : public std::unordered_map<std::string, Value, utility::StringHash, std::equal_to<>> {};


}
