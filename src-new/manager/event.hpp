// This file is part of ProjectDungeon
// Copyright (C) 2026 Life4gal <life4gal@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level directory of this distribution.

#pragma once

#include <entt/signal/dispatcher.hpp>

#define PD_EVENT_DEBUG 1
#if PD_EVENT_DEBUG
#include <prometheus/meta/name.hpp>
#include <spdlog/spdlog.h>
#endif

namespace pd::manager
{
	class Event final
	{
	public:
		using dispatcher_type = entt::dispatcher;

	private:
		inline static dispatcher_type dispatcher_{};

	public:
		template<typename Event, auto Functor>
		static auto subscribe() noexcept -> void //
			requires requires { dispatcher_.sink<Event>().template connect<Functor>(); }
		{
			dispatcher_.sink<Event>().template connect<Functor>();
		}

		template<typename Event, auto Functor, typename Bind>
		static auto subscribe(Bind& bind) noexcept -> void //
			requires requires { dispatcher_.sink<Event>().template connect<Functor, Bind>(bind); }
		{
			dispatcher_.sink<Event>().template connect<Functor, Bind>(bind);
		}

		template<typename Event, auto Functor, typename Bind>
		static auto subscribe(Bind* bind) noexcept -> void //
			requires requires { dispatcher_.sink<Event>().template connect<Functor, Bind>(bind); }
		{
			dispatcher_.sink<Event>().template connect<Functor, Bind>(bind);
		}

		template<typename Event, auto Functor>
		static auto unsubscribe() noexcept -> void //
			requires requires { dispatcher_.sink<Event>().template disconnect<Functor>(); }
		{
			dispatcher_.sink<Event>().template disconnect<Functor>();
		}

		template<typename Event, auto Functor, typename Bind>
		static auto unsubscribe(Bind& bind) noexcept -> void //
			requires requires { dispatcher_.sink<Event>().template disconnect<Functor, Bind>(bind); }
		{
			dispatcher_.sink<Event>().template disconnect<Functor, Bind>(bind);
		}

		template<typename Event, auto Functor, typename Bind>
		static auto unsubscribe(Bind* bind) noexcept -> void //
			requires requires { dispatcher_.sink<Event>().template disconnect<Functor, Bind>(bind); }
		{
			dispatcher_.sink<Event>().template disconnect<Functor, Bind>(bind);
		}

		template<typename Event>
		static auto unsubscribe(const void* bind) noexcept -> void //
			requires requires { dispatcher_.sink<Event>().disconnect(bind); }
		{
			dispatcher_.sink<Event>().disconnect(bind);
		}

		template<typename Event>
		static auto unsubscribe() noexcept -> void //
			requires requires { dispatcher_.sink<Event>().disconnect(); }
		{
			dispatcher_.sink<Event>().disconnect();
		}

		template<typename Bind>
		static auto unsubscribe(Bind& bind) noexcept -> void //
			requires requires { dispatcher_.disconnect(bind); }
		{
			dispatcher_.disconnect(bind);
		}

		template<typename Bind>
		static auto unsubscribe(Bind* bind) noexcept -> void //
			requires requires { dispatcher_.disconnect(bind); }
		{
			dispatcher_.disconnect(bind);
		}

		template<typename Event>
		static auto clear() noexcept -> void //
			requires requires { dispatcher_.sink<Event>().clear(); }
		{
			dispatcher_.sink<Event>().clear();
		}

		template<typename = void>
		static auto clear() noexcept -> void //
			requires requires { dispatcher_.clear(); }
		{
			dispatcher_.clear();
		}

		template<typename Event>
		static auto trigger(Event&& event) noexcept -> void //
			requires requires { dispatcher_.trigger(std::forward<Event>(event)); }
		{
#if PD_EVENT_DEBUG
			SPDLOG_INFO("Event::trigger({})", prometheus::meta::name_of<Event>());
#endif

			dispatcher_.trigger(std::forward<Event>(event));
		}

		template<typename Event>
		static auto enqueue(Event&& event) noexcept -> void //
			requires requires { dispatcher_.enqueue(std::forward<Event>(event)); }
		{
#if PD_EVENT_DEBUG
			SPDLOG_INFO("Event::enqueue({})", prometheus::meta::name_of<Event>());
#endif

			dispatcher_.enqueue(std::forward<Event>(event));
		}

		template<typename Event = void>
		static auto update() noexcept -> void //
			requires
			// update()
			std::conjunction_v<std::is_same<Event, void>, std::bool_constant<requires { dispatcher_.update(); }>> or
			// update<Event>()
			std::conjunction_v<std::negation<std::is_same<Event, void>>, std::bool_constant<requires { dispatcher_.update<Event>(); }>>
		{
			if constexpr (std::is_same_v<Event, void>)
			{
				dispatcher_.update();
			}
			else
			{
				dispatcher_.update<Event>();
			}
		}
	};
}
