/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2026 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#include "libecl/containers/queue_spsc.hpp"

#include <chrono>
#include <cstddef>
#include <iostream>
#include <thread>

#include "catch2/catch_test_macros.hpp"

using namespace std::chrono_literals;
using namespace libecl::containers;

SCENARIO("QueueSpsc: single producer single consumer")
{
	GIVEN("a queue with a single producer and a single consumer")
	{
		constexpr int queue_size = 100;
		constexpr int num_items = 100'000;

		auto queue = QueueSpsc<int, queue_size>{};

		auto consumer = std::jthread([&] {
			for (auto i = 0; i < num_items; ++i) {
				auto value = 0;
				while (!queue.pop(value)) {
				}
				if (value != i) {
					throw std::runtime_error("Values not matching");
				}
			}
		});

		auto start = std::chrono::steady_clock::now();
		for (auto i = 0; i < num_items; ++i) {
			while (!queue.push(i)) {
			}
		}
		while (!queue.empty()) {
		}
		auto stop = std::chrono::steady_clock::now();

		consumer.join();

		const auto elapsed = stop - start;
		std::cout << (num_items * 1s) / elapsed << " items/s\n";
	}
}
