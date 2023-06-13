/* SPDX-License-Identifier: BSD-3-Clause */
/* Copyright © 2023 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH
 *                  Matthias Kretz <m.kretz@gsi.de>
 */

#include "interleave.h"
#include "permute.h"

static_assert(
  all_of(std::get<0>(std::interleave(std::iota_v<std::simd<int>>))
	   == std::iota_v<std::simd<int>>));

static_assert(
  all_of(std::get<0>(std::interleave(std::simd<int>(0), std::simd<int>(1)))
	   == (std::iota_v<std::simd<int>> & 1)));

static_assert(
  all_of(std::get<1>(std::interleave(std::simd<int>(0), std::simd<int>(1)))
	   == (std::iota_v<std::simd<int>> & 1)));

static_assert(
  all_of(std::permute(std::iota_v<std::simd<int>>, std::simd_permutations::duplicate_even)
	   == std::iota_v<std::simd<int>> / 2 * 2));

static_assert(
  all_of(std::permute(std::iota_v<std::simd<int>>, std::simd_permutations::duplicate_odd)
	   == std::iota_v<std::simd<int>> / 2 * 2 + 1));

static_assert(
  all_of(std::permute(std::iota_v<std::simd<int>>, std::simd_permutations::swap_neighbors<1>)
	   == std::simd<int>([](int i) { return i ^ 1; })));

static_assert(
  all_of(std::permute(std::iota_v<std::fixed_size_simd<int, 8>>,
		      std::simd_permutations::swap_neighbors<2>)
	   == std::fixed_size_simd<int, 8>(std::array{2, 3, 0, 1, 6, 7, 4, 5}.begin())));

static_assert(
  all_of(std::permute(std::iota_v<std::fixed_size_simd<int, 12>>,
		      std::simd_permutations::swap_neighbors<3>)
	   == std::fixed_size_simd<int, 12>(
		std::array{3, 4, 5, 0, 1, 2, 9, 10, 11, 6, 7, 8}.begin())));

static_assert(
  all_of(std::permute(std::iota_v<std::simd<int>>, std::simd_permutations::broadcast<1>)
	   == std::simd<int>(1)));

static_assert(
  all_of(std::permute(std::iota_v<std::simd<int>>, std::simd_permutations::broadcast_first)
	   == std::simd<int>(0)));

static_assert(
  all_of(std::permute(std::iota_v<std::simd<int>>, std::simd_permutations::broadcast_last)
	   == std::simd<int>(int(std::simd_size_v<int> - 1))));

static_assert(
  all_of(std::permute(std::iota_v<std::simd<int>>, std::simd_permutations::reverse)
	   == std::simd<int>([](int i) { return int(std::simd_size_v<int>) - 1 - i; })));

static_assert(
  all_of(std::permute(std::iota_v<std::simd<int>>, std::simd_permutations::rotate<1>)
	   == (std::iota_v<std::simd<int>> + 1) % int(std::simd_size_v<int>)));

static_assert(
  all_of(std::permute(std::iota_v<std::simd<int>>, std::simd_permutations::rotate<2>)
	   == (std::iota_v<std::simd<int>> + 2) % int(std::simd_size_v<int>)));

static_assert(
  all_of(std::permute(std::iota_v<std::simd<int>>, std::simd_permutations::rotate<-2>)
	   == (std::iota_v<std::simd<int>> - 2) % int(std::simd_size_v<int>)));
