// Copyright (c) 2017-2020, Lawrence Livermore National Security, LLC and
// other Axom Project Developers. See the top-level COPYRIGHT file for details.
//
// SPDX-License-Identifier: (BSD-3-Clause)

/*!
 *******************************************************************************
 * \file LuaReader.cpp
 *
 * \brief This file contains the class implementation of the LuaReader.
 *******************************************************************************
 */

#include <fstream>

#include "axom/inlet/LuaReader.hpp"

#include "axom/core/utilities/FileUtilities.hpp"
#include "axom/core/utilities/StringUtilities.hpp"
#include "axom/inlet/inlet_utils.hpp"

#include "fmt/fmt.hpp"
#include "axom/slic.hpp"

namespace axom
{
namespace inlet
{
bool LuaReader::parseFile(const std::string& filePath)
{
  if(!axom::utilities::filesystem::pathExists(filePath))
  {
    SLIC_WARNING(
      fmt::format("Inlet: Given Lua input file does not exist: {0}", filePath));
    return false;
  }

  auto script = m_lua.script_file(filePath);
  if(!script.valid())
  {
    SLIC_WARNING(
      fmt::format("Inlet: Given Lua input file is invalid: {0}", filePath));
  }
  return script.valid();
}

bool LuaReader::parseString(const std::string& luaString)
{
  if(luaString.empty())
  {
    SLIC_WARNING("Inlet: Given an empty Lua string to parse.");
    return false;
  }
  m_lua.script(luaString);
  return true;
}

// TODO allow alternate delimiter at sidre level
#define SCOPE_DELIMITER '/'

bool LuaReader::getBool(const std::string& id, bool& value)
{
  return getValue(id, value);
}

bool LuaReader::getDouble(const std::string& id, double& value)
{
  return getValue(id, value);
}

bool LuaReader::getInt(const std::string& id, int& value)
{
  return getValue(id, value);
}

bool LuaReader::getString(const std::string& id, std::string& value)
{
  return getValue(id, value);
}

bool LuaReader::getIntMap(const std::string& id,
                          std::unordered_map<int, int>& values)
{
  return getMap(id, values, sol::type::number);
}

bool LuaReader::getDoubleMap(const std::string& id,
                             std::unordered_map<int, double>& values)
{
  return getMap(id, values, sol::type::number);
}

bool LuaReader::getBoolMap(const std::string& id,
                           std::unordered_map<int, bool>& values)
{
  return getMap(id, values, sol::type::boolean);
}

bool LuaReader::getStringMap(const std::string& id,
                             std::unordered_map<int, std::string>& values)
{
  return getMap(id, values, sol::type::string);
}

bool LuaReader::getIntMap(const std::string& id,
                          std::unordered_map<VariantKey, int>& values)
{
  return getMap(id, values, sol::type::number);
}

bool LuaReader::getDoubleMap(const std::string& id,
                             std::unordered_map<VariantKey, double>& values)
{
  return getMap(id, values, sol::type::number);
}

bool LuaReader::getBoolMap(const std::string& id,
                           std::unordered_map<VariantKey, bool>& values)
{
  return getMap(id, values, sol::type::boolean);
}

bool LuaReader::getStringMap(const std::string& id,
                             std::unordered_map<VariantKey, std::string>& values)
{
  return getMap(id, values, sol::type::string);
}

template <typename Iter>
bool LuaReader::traverseToTable(Iter begin, Iter end, sol::table& table)
{
  // Nothing to traverse
  if(begin == end)
  {
    return true;
  }

  if(!m_lua[*begin].valid())
  {
    return false;
  }

  table = m_lua[*begin];  // Use the first one to index into the global lua state
  ++begin;

  // Then use the remaining keys to walk down to the requested table
  for(auto curr = begin; curr != end; ++curr)
  {
    auto key = *curr;
    auto as_int = checkedConvertToInt(key);
    if(as_int.second && table[as_int.first].valid())
    {
      table = table[as_int.first];
    }
    else if(table[key].valid())
    {
      table = table[key];
    }
    else
    {
      return false;
    }
  }
  return true;
}

bool LuaReader::getIndices(const std::string& id, std::vector<int>& indices)
{
  return getIndicesInternal(id, indices);
}

bool LuaReader::getIndices(const std::string& id, std::vector<VariantKey>& indices)
{
  return getIndicesInternal(id, indices);
}

template <typename T>
bool LuaReader::getValue(const std::string& id, T& value)
{
  std::vector<std::string> tokens;
  axom::utilities::string::split(tokens, id, SCOPE_DELIMITER);

  if(tokens.size() == 1)
  {
    if(m_lua[tokens[0]].valid())
    {
      value = m_lua[tokens[0]];
      return true;
    }
    return false;
  }

  sol::table t;
  // Don't traverse through the last token as it doesn't contain a table
  if(!traverseToTable(tokens.begin(), tokens.end() - 1, t))
  {
    return false;
  }

  if(t[tokens.back()].valid())
  {
    value = t[tokens.back()];
    return true;
  }

  return false;
}

namespace detail
{
/*!
 *******************************************************************************
 * \brief Extracts an object from sol into a concrete type, implemented to support
 * extracting to a VariantKey
 * 
 * \tparam T The type to extract to
 *******************************************************************************
 */
template <typename T>
T extractAs(const sol::object& obj)
{
  // By default, just ask sol to cast it
  return obj.as<T>();
}
/// \overload
template <>
VariantKey extractAs(const sol::object& obj)
{
  // FIXME: Floating-point indices?
  if(obj.get_type() == sol::type::number)
  {
    return obj.as<int>();
  }
  else
  {
    return obj.as<std::string>();
  }
}

/*!
 *******************************************************************************
 * \brief Retrieves the InletType of a sol object
 * \param [in] obj The object to retrieve the type from
 *******************************************************************************
 */
InletType getInletType(const sol::object& obj)
{
  switch(obj.get_type())
  {
  case sol::type::string:
    return InletType::String;

  case sol::type::number:
    // Integers?
    return InletType::Double;

  case sol::type::boolean:
    return InletType::Bool;

    // FIXME: Functions
    // case sol::type::function::
    //   return InletType::Function;

  case sol::type::table:
  {
    std::vector<sol::type> keyTypes;
    std::vector<sol::type> valueTypes;
    for(const auto& entry : obj.as<sol::table>())
    {
      keyTypes.push_back(entry.first.get_type());
      valueTypes.push_back(entry.second.get_type());
    }
    // Inlet only allows string- and integer-keyed containers
    bool allLegalKeys =
      std::all_of(keyTypes.begin(), keyTypes.end(), [](const sol::type type) {
        return type == sol::type::number || type == sol::type::string;
      });
    // Should we warn here?
    if(!allLegalKeys)
    {
      return InletType::Nothing;
    }

    bool isDict =
      std::any_of(keyTypes.begin(), keyTypes.end(), [](const sol::type type) {
        return type == sol::type::string;
      });

    // Check if table entries are of homogenous type
    bool valuesAllSameType = std::all_of(valueTypes.begin() + 1,
                                         valueTypes.end(),
                                         [&valueTypes](const sol::type type) {
                                           return type == valueTypes.front();
                                         });

    if(!valuesAllSameType)
    {
      return isDict ? InletType::MixedDictionary : InletType::MixedArray;
    }

    // Note that "object" tables will be classified as Dictionaries
    switch(valueTypes.front())
    {
    case sol::type::boolean:
      return isDict ? InletType::BoolDictionary : InletType::BoolArray;
    case sol::type::string:
      return isDict ? InletType::StringDictionary : InletType::StringArray;
    case sol::type::number:
      // Integers?
      return isDict ? InletType::DoubleDictionary : InletType::DoubleArray;
    case sol::type::table:
      // Lua does not distinguish foo.bar from foo['bar'] so we say that a dictionary
      // of objects is just an object
      return isDict ? InletType::Object : InletType::ObjectArray;
    default:
      return InletType::Nothing;
    }
  }

  default:
    // Should we warn here?
    return InletType::Nothing;
  }
}

}  // end namespace detail

InletType LuaReader::getType(const std::string& id)
{
  std::vector<std::string> tokens;
  axom::utilities::string::split(tokens, id, SCOPE_DELIMITER);
  if(tokens.size() == 1)
  {
    if(m_lua[tokens[0]].valid())
    {
      return detail::getInletType(m_lua[tokens[0]]);
    }
    else
    {
      return InletType::Nothing;
    }
  }

  sol::table t;
  if(!tokens.empty() && traverseToTable(tokens.begin(), tokens.end() - 1, t))
  {
    if(t[tokens.back()].valid())
    {
      return detail::getInletType(t[tokens.back()]);
    }
  }
  return InletType::Nothing;
}

template <typename Key, typename Val>
bool LuaReader::getMap(const std::string& id,
                       std::unordered_map<Key, Val>& values,
                       sol::type type)
{
  values.clear();
  std::vector<std::string> tokens;
  axom::utilities::string::split(tokens, id, SCOPE_DELIMITER);

  sol::table t;
  if(tokens.empty() || !traverseToTable(tokens.begin(), tokens.end(), t))
  {
    return false;
  }

  // Allows for filtering out keys of incorrect type
  const auto is_correct_key_type = [](const sol::type type) {
    bool is_number = type == sol::type::number;
    // Arrays only
    if(std::is_same<Key, int>::value)
    {
      return is_number;
    }
    // Dictionaries can have both string-valued and numeric keys
    else
    {
      return is_number || (type == sol::type::string);
    }
  };

  for(const auto& entry : t)
  {
    // Gets only indexed items in the table.
    if(is_correct_key_type(entry.first.get_type()) &&
       entry.second.get_type() == type)
    {
      values[detail::extractAs<Key>(entry.first)] =
        detail::extractAs<Val>(entry.second);
    }
  }
  return true;
}

template <typename T>
bool LuaReader::getIndicesInternal(const std::string& id, std::vector<T>& indices)
{
  std::vector<std::string> tokens;
  axom::utilities::string::split(tokens, id, SCOPE_DELIMITER);

  sol::table t;

  if(tokens.empty() || !traverseToTable(tokens.begin(), tokens.end(), t))
  {
    return false;
  }

  indices.clear();

  // std::transform ends up being messier here
  for(const auto& entry : t)
  {
    indices.push_back(detail::extractAs<T>(entry.first));
  }
  return true;
}

}  // end namespace inlet
}  // end namespace axom
