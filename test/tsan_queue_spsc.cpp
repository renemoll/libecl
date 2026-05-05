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
#include <iostream>
#include <thread>

using namespace std::chrono_literals;

namespace {
void pin_thread(int cpu_id)
{
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	CPU_SET(cpu_id, &cpuset);
	if (pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset) != 0) {
		std::exit(EXIT_FAILURE);
	}
}
}  // namespace

int main()
{
	constexpr int queue_size = 100'000;
	constexpr int num_items = 100'000'000;

	using namespace libecl::containers;
	auto queue = QueueSpsc<int, queue_size>{};

	auto consumer = std::jthread([&] {
		pin_thread(2);
		for (auto i = 0; i < num_items; ++i) {
			auto value = 0;
			while (!queue.pop(value))
				;
			if (value != i) {
				throw std::runtime_error("Values not matching");
			}
		}
	});

	pin_thread(3);

	auto start = std::chrono::steady_clock::now();
	for (auto i = 0; i < num_items; ++i) {
		while (!queue.push(i))
			;
	}
	while (!queue.empty())
		;
	auto stop = std::chrono::steady_clock::now();

	consumer.join();

	const auto elapsed = stop - start;
	std::cout << (num_items * 1s) / elapsed << " items/s\n";

	return 0;
}
