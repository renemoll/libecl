/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2026 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#include <cassert>
#include <concepts>
#include <type_traits>
#include <utility>

#include "catch2/catch_test_macros.hpp"

// #include <atomic>
#include <memory>
// #include <new>
// #include <cstring>
// #include <iostream>
// #include <optional>

template <typename T>
class Message;

template <typename T>
concept MessagePayload =
	std::is_object_v<T> && !std::is_pointer_v<T> && !std::is_reference_v<T> && !std::is_array_v<T> &&
	!std::is_const_v<T> && !std::is_volatile_v<T> && std::default_initializable<T> && std::destructible<T>;

class MessagePool
{
public:
	/*!
	 * \brief
	 * \tparam T
	 * \return
	 *
	 * Todo: ensure T is a pod type that can be safely copied and moved without invoking undefined behavior.
	 *       - static_assert(std::is_trivially_copyable_v<T>, "MessagePool only supports trivially copyable types");
	 *       - static_assert(std::is_trivially_move_constructible_v<T>, "MessagePool only supports trivially move
	 * constructible types");4
	 *       T is not allowed to be a primitive type like int, float, etc. because they don't have a destructor and the
	 * pool needs to call the destructor when deallocating the message.
	 *       - static_assert(!std::is_fundamental_v<T>, "MessagePool does not support fundamental types");
	 *       T is not allowed to be a pointer type because the pool needs to manage the memory of the message and
	 * pointers can lead to double free and other memory management issues.
	 *     - static_assert(!std::is_pointer_v<T>, "MessagePool does not support pointer types");
	 */
	template <MessagePayload T>
	Message<T> allocate()
	{
		// std::size_t size = sizeof(T);
		// todo: placement new on aligned memory, pool maintains ownership of the memory

		auto* obj = new T{};
		return Message<T>{*this, obj};
	}

	template <MessagePayload T>
	void deallocate(T* msg) noexcept
	{
		delete msg;
	}

private:
	// void reserve(std::size_t node_size, std::size_t alignment, std::size_t count);
};

template <typename T>
class ImmutableMessage;

/*!
 * \brief Wraps a type T into a message that can be passed between modules.
 * \tparam T
 */
template <typename T>
class Message
{
private:
	friend class MessagePool;
	friend class ImmutableMessage<T>;

	/*!
	 * \brief
	 * \param pool
	 * \param data
	 *
	 * Messages can only be created by the \a MessagePool, which manages the memory wrapped by the \a Message.
	 */
	Message(MessagePool& pool, T* data)
		: m_data{data, [pool_ptr = &pool](T* ptr) noexcept { pool_ptr->deallocate(ptr); }}
	{
		assert(data != nullptr);
	}

public:
	~Message() noexcept = default;

	Message(const Message& other) noexcept = default;
	Message& operator=(const Message&) noexcept = default;

	Message(Message&& other) noexcept = default;
	Message& operator=(Message&& other) noexcept = default;

	[[nodiscard]] T& data() noexcept
	{
		assert(m_data != nullptr);	// TODO: can this be prevented?
		return *m_data;
	}

	[[nodiscard]] const T& view() const noexcept
	{
		assert(m_data != nullptr);	// TODO: can this be prevented?
		return *m_data;
	}

	[[nodiscard]] bool has_value() const noexcept
	{
		return static_cast<bool>(m_data);
	}

private:
	std::shared_ptr<T> m_data;
};

/*!
 * \brief Immutable variant of \a Message, only allows reading the encapsulated data.
 */
template <typename T>
class ImmutableMessage
{
public:
	explicit ImmutableMessage(Message<T>& msg) noexcept
		: m_data{msg.m_data}
	{
	}

	[[nodiscard]] const T& view() const noexcept
	{
		assert(m_data != nullptr);
		return *m_data;
	}

private:
	std::shared_ptr<T> m_data;
};

SCENARIO("MessagePool: allocate message")
{
	GIVEN("a message pool")
	{
		auto pool = MessagePool();

		WHEN("allocating a message")
		{
			struct MyMessage
			{
				int value;
			};

			auto msg = pool.allocate<MyMessage>();
			msg.data().value = 42;

			THEN("the message contains the correct value")
			{
				REQUIRE(msg.view().value == 42);
			}
		}

		WHEN("allocating multiple messages")
		{
			struct MyMessage
			{
				int value;
			};

			auto msg1 = pool.allocate<MyMessage>();
			auto msg2 = pool.allocate<MyMessage>();

			msg1.data().value = 1;
			msg2.data().value = 2;

			THEN("each message contains its own value")
			{
				REQUIRE(msg1.view().value == 1);
				REQUIRE(msg2.view().value == 2);
			}
		}
	}

	// struct MyMessage1
	// {
	// 	int a;
	// 	int b;
	// };

	// struct MyMessage2
	// {
	// 	int a;
	// 	uint8_t b;
	// 	int16_t c;
	// };
}

SCENARIO("ImmutableMessage: view message")
{
	GIVEN("a message pool and an allocated message")
	{
		auto pool = MessagePool();

		struct MyMessage
		{
			int value;
		};

		auto msg = pool.allocate<MyMessage>();
		msg.data().value = 42;

		WHEN("creating an immutable view of the message")
		{
			auto immutable_msg = ImmutableMessage(msg);

			THEN("the immutable view contains the correct value")
			{
				REQUIRE(immutable_msg.view().value == 42);
			}

			THEN("the original Message can still be accessed and modified")
			{
				msg.data().value = 100;
				REQUIRE(msg.view().value == 100);
				REQUIRE(immutable_msg.view().value == 100);
			}
		}

		WHEN("the original Message is destroyed")
		{
			auto immutable_msg = ImmutableMessage(msg);

			{
				auto temp_msg = std::move(msg);
				REQUIRE_FALSE(msg.has_value());
				// the data originally contained by msg is now owned by temp_msg, and will go out of scope
				// here, leaving the immutable_msg holding the sole reference to the data.
			}

			THEN("the immutable view still contains the correct value")
			{
				REQUIRE(immutable_msg.view().value == 42);
			}
		}
	}
}
