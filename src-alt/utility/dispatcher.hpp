// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/signal/dispatcher.hpp>

namespace pd::utility
{
	namespace detail
	{
		template<typename Event, auto Functor>
		concept invokable = requires(Event event)
		{
			std::invoke(Functor, event);
		};

		template<typename Event, auto Functor, typename Bind>
		concept invokable_with_bind_ref = requires(Bind& bind, Event event)
		{
			std::invoke(Functor, bind, event);
		};

		template<typename Event, auto Functor, typename Bind>
		concept invokable_with_bind_ptr = requires(Bind* bind, Event event)
		{
			std::invoke(Functor, bind, event);
		};
	}

	class Dispatcher
	{
	public:
		using dispatcher_type = entt::dispatcher;

	private:
		dispatcher_type dispatcher_;

	public:
		template<typename Event, auto Functor>
			requires detail::invokable<Event, Functor>
		auto subscribe() noexcept -> void
		{
			dispatcher_.sink<Event>().template connect<Functor>();
		}

		template<typename Event, auto Functor, typename Bind>
			requires detail::invokable_with_bind_ref<Event, Functor, Bind>
		auto subscribe(Bind& bind) noexcept -> void
		{
			dispatcher_.sink<Event>().template connect<Functor, Bind>(bind);
		}

		template<typename Event, auto Functor, typename Bind>
			requires detail::invokable_with_bind_ptr<Event, Functor, Bind>
		auto subscribe(Bind* bind) noexcept -> void
		{
			dispatcher_.sink<Event>().template connect<Functor, Bind>(bind);
		}

		template<typename Event, auto Functor>
			requires detail::invokable<Event, Functor>
		auto unsubscribe() noexcept -> void
		{
			dispatcher_.sink<Event>().template disconnect<Functor>();
		}

		template<typename Event, auto Functor, typename Bind>
			requires detail::invokable_with_bind_ref<Event, Functor, Bind>
		auto unsubscribe(Bind& bind) noexcept -> void
		{
			dispatcher_.sink<Event>().template disconnect<Bind>(bind);
		}

		template<typename Event, auto Functor, typename Bind>
			requires detail::invokable_with_bind_ptr<Event, Functor, Bind>
		auto unsubscribe(Bind* bind) noexcept -> void
		{
			dispatcher_.sink<Event>().template disconnect<Bind>(bind);
		}

		template<typename Event>
		auto unsubscribe(const void* bind) noexcept -> void
		{
			dispatcher_.sink<Event>().disconnect(bind);
		}

		template<typename Event>
		auto unsubscribe() noexcept -> void
		{
			dispatcher_.sink<Event>().disconnect();
		}

		template<typename Event>
		auto clear() noexcept -> void
		{
			dispatcher_.sink<Event>().clear();
		}

		auto clear() noexcept -> void
		{
			dispatcher_.clear();
		}

		template<typename Event>
		auto trigger(Event&& event) noexcept -> void
		{
			dispatcher_.trigger(std::forward<Event>(event));
		}

		template<typename Event>
		auto enqueue(Event&& event) noexcept -> void
		{
			dispatcher_.enqueue(std::forward<Event>(event));
		}

		template<typename Event>
		auto update() noexcept -> void
		{
			dispatcher_.update<Event>();
		}

		auto update() const noexcept -> void
		{
			dispatcher_.update();
		}
	};
}
