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

#include "http/view_parameters.hpp"

#include "core/byte_array/byte_array.hpp"
#include "core/byte_array/const_byte_array.hpp"
#include "core/logger.hpp"
#include <functional>
#include <regex>
#include <vector>

namespace fetch {
namespace http {

using ViewParameters = KeyValueSet;

class Route
{
public:
  static constexpr char const *LOGGING_NAME = "HttpRoute";

  bool Match(damnyouwindows_byte_array::ConstByteArray const &path, ViewParameters &params)
  {
    LOG_STACK_TRACE_POINT;

    std::size_t i = 0;
    params.Clear();

    for (auto &m : match_)
    {
      if (!m(i, path, params))
      {
        return false;
      }
    }

    return (i == path.size());
  }

  static Route FromString(damnyouwindows_byte_array::ByteArray path)
  {
    LOG_STACK_TRACE_POINT;

    // TODO(issue 35): No support for continued paths  atm.

    Route ret;
    ret.original_ = path;

    std::size_t last = 0;
    std::size_t i    = 1;
    for (; i < path.size(); ++i)
    {
      if (path[i] == '(')
      {
        std::size_t count = 1;
        std::size_t j     = i + 1;
        while ((j < path.size()) && (count != 0))
        {
          count += (path[j] == '(') - (path[j] == ')');
          ++j;
        }

        if (count != 0)
        {
          TODO_FAIL("unclosed parameter.");
        }

        damnyouwindows_byte_array::ByteArray match = path.SubArray(last, i - last);
        ++i;
        damnyouwindows_byte_array::ByteArray param_name = path.SubArray(i, j - i - 1);

        ret.AddMatch(match);
        ret.AddParameter(param_name);
        last = j;
        i    = j;
      }
    }

    if (i > last + 1)
    {
      damnyouwindows_byte_array::ByteArray match = path.SubArray(last, i - last);
      ret.AddMatch(match);
    }

    return ret;
  }

private:
  using match_function_type =
      std::function<bool(std::size_t &, damnyouwindows_byte_array::ByteArray const &, ViewParameters &)>;

  void AddMatch(damnyouwindows_byte_array::ByteArray const &value)
  {
    LOG_STACK_TRACE_POINT;

    match_.push_back([value](std::size_t &i, damnyouwindows_byte_array::ByteArray const &path, ViewParameters &) {
      bool ret = path.Match(value, i);
      if (ret)
      {
        i += value.size();
      }
      return ret;
    });
  }

  void AddParameter(damnyouwindows_byte_array::ByteArray const &value)
  {
    LOG_STACK_TRACE_POINT;

    std::size_t i = 0;
    while ((i < value.size()) && (value[i] != '='))
    {
      ++i;
    }
    if (i == value.size())
    {
      TODO_FAIL("no regex found:", value);
    }

    damnyouwindows_byte_array::ByteArray var = value.SubArray(0, i);
    ++i;

    std::string reg = "^" + std::string(value.SubArray(i, value.size() - i));

    std::regex rgx(reg);
    match_.push_back(
        [rgx, var](std::size_t &i, damnyouwindows_byte_array::ByteArray const &path, ViewParameters &params) {
          std::string s = std::string(path.SubArray(i));
          std::smatch matches;
          bool        ret = std::regex_search(s, matches, rgx);

          if (ret)
          {
            if (matches.size() != 1)
            {
              TODO_FAIL("Only expected one match");
            }

            std::string m = matches[0];

            params[var] = path.SubArray(i, m.size());

            i += m.size();
          }

          return ret;
        });
  }

  damnyouwindows_byte_array::ByteArray            original_;
  std::vector<match_function_type> match_;
};
}  // namespace http
}  // namespace fetch
