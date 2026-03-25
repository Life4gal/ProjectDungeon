// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <utility/dispatcher.hpp>

namespace pd::manager
{
	class Event final
	{
		using dispatcher_type = utility::Dispatcher;

		dispatcher_type dispatcher_;

	public:
		// 这个接口公开仅为了Game::debug_event_
		[[nodiscard]] static auto instance() noexcept -> Event&;

	private:
		[[nodiscard]] static auto dispatcher() noexcept -> dispatcher_type&
		{
			return instance().dispatcher_;
		}

	public:
		template<typename Event, auto Functor>
		static auto subscribe() noexcept -> void //
			requires requires { dispatcher().subscribe<Event, Functor>(); }
		{
			return dispatcher().subscribe<Event, Functor>();
		}

		template<typename Event, auto Functor, typename Bind>
		static auto subscribe(Bind& bind) noexcept -> void //
			requires requires { dispatcher().subscribe<Event, Functor, Bind>(bind); }
		{
			return dispatcher().subscribe<Event, Functor, Bind>(bind);
		}

		template<typename Event, auto Functor, typename Bind>
		static auto subscribe(Bind* bind) noexcept -> void //
			requires requires { dispatcher().subscribe<Event, Functor, Bind>(bind); }
		{
			return dispatcher().subscribe<Event, Functor, Bind>(bind);
		}

		template<typename Event, auto Functor>
		static auto unsubscribe() noexcept -> void //
			requires requires { dispatcher().unsubscribe<Event, Functor>(); }
		{
			return dispatcher().unsubscribe<Event, Functor>();
		}

		template<typename Event, auto Functor, typename Bind>
		static auto unsubscribe(Bind& bind) noexcept -> void //
			requires requires { dispatcher().unsubscribe<Event, Functor, Bind>(bind); }
		{
			return dispatcher().unsubscribe<Event, Functor, Bind>(bind);
		}

		template<typename Event, auto Functor, typename Bind>
		static auto unsubscribe(Bind* bind) noexcept -> void //
			requires requires { dispatcher().unsubscribe<Event, Functor, Bind>(bind); }
		{
			return dispatcher().unsubscribe<Event, Functor, Bind>(bind);
		}

		template<typename Event>
		static auto unsubscribe(const void* bind) noexcept -> void //
			requires requires { dispatcher().unsubscribe<Event>(bind); }
		{
			return dispatcher().unsubscribe<Event>(bind);
		}

		template<typename Event>
		static auto clear() noexcept -> void //
			requires requires { dispatcher().clear<Event>(); }
		{
			return dispatcher().clear<Event>();
		}

		template<typename = void>
		static auto clear() noexcept -> void //
			requires requires { dispatcher().clear(); }
		{
			return dispatcher().clear();
		}

		template<typename Event>
		static auto trigger(Event&& event) noexcept -> void //
			requires requires { dispatcher().trigger(std::forward<Event>(event)); }
		{
			return dispatcher().trigger(std::forward<Event>(event));
		}

		template<typename Event>
		static auto enqueue(Event&& event) noexcept -> void //
			requires requires { dispatcher().enqueue(std::forward<Event>(event)); }
		{
			return dispatcher().enqueue(std::forward<Event>(event));
		}

		template<typename = void>
		static auto update() noexcept -> void //
			requires requires { dispatcher().update(); }
		{
			return dispatcher().update();
		}
	};
}
