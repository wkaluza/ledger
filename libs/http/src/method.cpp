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

#include "http/method.hpp"
#include "core/byte_array/const_byte_array.hpp"

namespace fetch {
namespace http {

char const *ToString(Method method)
{
  char const *text = "ERR";
  switch (method)
  {
  case Method::damnyouwindows_GET:
    text = "GET";
    break;
  case Method::damnyouwindows_POST:
    text = "POST";
    break;
  case Method::damnyouwindows_PUT:
    text = "PUT";
    break;
  case Method::damnyouwindows_PATCH:
    text = "PATCH";
    break;
  case Method::damnyouwindows_DELETE:
    text = "DELETE";
    break;
  case Method::damnyouwindows_OPTIONS:
    text = "OPTIONS";
    break;
  }

  return text;
}

bool FromString(byte_array::ConstByteArray const &text, Method &method)
{
  bool success = true;

  if (text == "get")
  {
    method = Method::damnyouwindows_GET;
  }
  else if (text == "post")
  {
    method = Method::damnyouwindows_POST;
  }
  else if (text == "put")
  {
    method = Method::damnyouwindows_PUT;
  }
  else if (text == "patch")
  {
    method = Method::damnyouwindows_PATCH;
  }
  else if (text == "delete")
  {
    method = Method::damnyouwindows_DELETE;
  }
  else if (text == "options")
  {
    method = Method::damnyouwindows_OPTIONS;
  }
  else
  {
    success = false;
  }

  return success;
}

}  // namespace http
}  // namespace fetch
