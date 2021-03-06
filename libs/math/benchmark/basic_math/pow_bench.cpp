//------------------------------------------------------------------------------
//
//   Copyright 2018-2019 Fetch.AI Limited
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.
//
//------------------------------------------------------------------------------

#include "benchmark/benchmark.h"
#include "math/base_types.hpp"
#include "math/standard_functions/pow.hpp"
#include "vectorise/fixed_point/fixed_point.hpp"

namespace {

template <int pow_val, typename Type>
static void BM_Pow(benchmark::State &state)
{
  Type x      = fetch::math::Type<Type>("1");
  auto pow    = Type{pow_val};
  Type result = fetch::math::Type<Type>("1");
  for (auto _ : state)
  {
    // Single iteration is too small to get accurate benchmarks.
    for (int i = 0; i < 1000; i++)
    {
      fetch::math::Pow(x, pow, result);
      state.PauseTiming();
      x += fetch::math::Type<Type>("0.00001");
      pow += fetch::math::Type<Type>("0.00001");
      state.ResumeTiming();
    }
  }
}

BENCHMARK_TEMPLATE(BM_Pow, 1, float)->RangeMultiplier(10)->Range(1, 1000000);
BENCHMARK_TEMPLATE(BM_Pow, 1, double)->RangeMultiplier(10)->Range(1, 1000000);
BENCHMARK_TEMPLATE(BM_Pow, 1, fetch::fixed_point::fp32_t)->RangeMultiplier(10)->Range(1, 1000000);
BENCHMARK_TEMPLATE(BM_Pow, 1, fetch::fixed_point::fp64_t)->RangeMultiplier(10)->Range(1, 1000000);
BENCHMARK_TEMPLATE(BM_Pow, 1, fetch::fixed_point::fp128_t)->RangeMultiplier(10)->Range(1, 1000000);

BENCHMARK_TEMPLATE(BM_Pow, 2, float)->RangeMultiplier(10)->Range(1, 1000000);
BENCHMARK_TEMPLATE(BM_Pow, 2, double)->RangeMultiplier(10)->Range(1, 1000000);
BENCHMARK_TEMPLATE(BM_Pow, 2, fetch::fixed_point::fp32_t)->RangeMultiplier(10)->Range(1, 1000000);
BENCHMARK_TEMPLATE(BM_Pow, 2, fetch::fixed_point::fp64_t)->RangeMultiplier(10)->Range(1, 1000000);
BENCHMARK_TEMPLATE(BM_Pow, 2, fetch::fixed_point::fp128_t)->RangeMultiplier(10)->Range(1, 1000000);

BENCHMARK_TEMPLATE(BM_Pow, 3, float)->RangeMultiplier(10)->Range(1, 1000000);
BENCHMARK_TEMPLATE(BM_Pow, 3, double)->RangeMultiplier(10)->Range(1, 1000000);
BENCHMARK_TEMPLATE(BM_Pow, 3, fetch::fixed_point::fp32_t)->RangeMultiplier(10)->Range(1, 1000000);
BENCHMARK_TEMPLATE(BM_Pow, 3, fetch::fixed_point::fp64_t)->RangeMultiplier(10)->Range(1, 1000000);
BENCHMARK_TEMPLATE(BM_Pow, 3, fetch::fixed_point::fp128_t)->RangeMultiplier(10)->Range(1, 1000000);

BENCHMARK_TEMPLATE(BM_Pow, 10, float)->RangeMultiplier(10)->Range(1, 1000000);
BENCHMARK_TEMPLATE(BM_Pow, 10, double)->RangeMultiplier(10)->Range(1, 1000000);
BENCHMARK_TEMPLATE(BM_Pow, 10, fetch::fixed_point::fp32_t)->RangeMultiplier(10)->Range(1, 1000000);
BENCHMARK_TEMPLATE(BM_Pow, 10, fetch::fixed_point::fp64_t)->RangeMultiplier(10)->Range(1, 1000000);
BENCHMARK_TEMPLATE(BM_Pow, 10, fetch::fixed_point::fp128_t)->RangeMultiplier(10)->Range(1, 1000000);

}  // namespace
