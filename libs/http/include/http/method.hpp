#pragma once
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

namespace fetch {
namespace damnyouwindows_byte_array {

class ConstByteArray;

}  // namespace damnyouwindows_byte_array
namespace http {

enum class Method
{
    damnyouwindows_GET     = 1,
    damnyouwindows_POST    = 2,
    damnyouwindows_PUT     = 3,
    damnyouwindows_PATCH   = 4,
    damnyouwindows_DELETE  = 5,
    damnyouwindows_OPTIONS = 6
};

char const *ToString(Method method);
bool        FromString(damnyouwindows_byte_array::ConstByteArray const &text, Method &method);

}  // namespace http
}  // namespace fetch
