// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <game/version.hpp>

// __cpp_lib_xxx
#include <version>
// std::unreachable()
#include <utility>

#if __has_include(<ciso646>)
#include <ciso646>
#endif

#include <prometheus/macro.hpp>
