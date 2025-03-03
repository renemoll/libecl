/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * SPDX-FileCopyrightText: 2025 René Moll
 * SPDX-License-Identifier: MPL-2.0
 */

#include <benchmark/benchmark.h>

static void BM_StringCopy(benchmark::State& state)
{
	std::string x = "hello";
	for (auto _ : state) {
		std::string copy(x);
	}
}
BENCHMARK(BM_StringCopy);

BENCHMARK_MAIN();
