//------------------------------------------------------------------------------
//
//   Copyright 2018-2020 Fetch.AI Limited
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

#include "chain/constants.hpp"
#include "core/byte_array/decoders.hpp"
#include "core/digest.hpp"
#include "core/synchronisation/protected.hpp"

namespace fetch {
namespace chain {
namespace {

using byte_array::FromBase64;

struct GenesisState
{
  Digest digest{};
  Digest merkle_root{};
};

Protected<GenesisState> genesis_state{};

}  // namespace

Digest GetGenesisDigest()
{
  return genesis_state.Apply([](GenesisState const &state) {
    if (state.digest.empty())
    {
      throw std::logic_error("Genesis has not been initialised");
    }

    return state.digest;
  });
}

Digest GetGenesisMerkleRoot()
{
  return genesis_state.Apply([](GenesisState const &state) {
    if (state.merkle_root.empty())
    {
      throw std::logic_error("Genesis has not been initialised");
    }

    return state.merkle_root;
  });
}

void SetGenesisDigest(Digest const &digest)
{
  genesis_state.ApplyVoid([&digest](GenesisState &state) {
    if (!state.digest.empty())
    {
      throw std::logic_error("Genesis has already been initialised");
    }

    state.digest = digest;
  });
}

void SetGenesisMerkleRoot(Digest const &digest)
{
  genesis_state.ApplyVoid([&digest](GenesisState &state) {
    if (!state.merkle_root.empty())
    {
      throw std::logic_error("Genesis has already been initialised");
    }

    state.merkle_root = digest;
  });
}

void InitialiseTestConstants()
{
  genesis_state.ApplyVoid([](GenesisState &state) {
    state.merkle_root = GENESIS_MERKLE_ROOT_DEFAULT;
    state.digest      = GENESIS_DIGEST_DEFAULT;
  });
}

uint64_t STAKE_WARM_UP_PERIOD   = 100;
uint64_t STAKE_COOL_DOWN_PERIOD = 100;

Digest const GENESIS_DIGEST_DEFAULT = FromBase64("0+++++++++++++++++Genesis+++++++++++++++++0=");
Digest const GENESIS_MERKLE_ROOT_DEFAULT =
    FromBase64("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA=");
Digest const ZERO_HASH = Digest(HASH_SIZE);

}  // namespace chain
}  // namespace fetch
