// The "inih" library is distributed under the New BSD license :
//
// Copyright(c) 2009, Ben Hoyt
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met :
// * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and / or other materials provided with the distribution.
// * Neither the name of Ben Hoyt nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY BEN HOYT ''AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED.IN NO EVENT SHALL BEN HOYT BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "ini/reader.h"

#include "ini/ini.h"

#include <algorithm>
#include <cctype>
#include <cstdlib>

IniReader::IniReader(const std::string& filename)
{
    _error = ini_parse(filename.c_str(), ValueHandler, this);
}

int IniReader::ParseError()
{
    return _error;
}

std::string IniReader::Get(const std::string& section, const std::string& name, const std::string& default_value)
{
    std::string key = MakeKey(section, name);

    return _values.count(key) ? _values[key] : default_value;
}

long IniReader::GetInteger(const std::string& section, const std::string& name, long default_value)
{
    std::string valstr = Get(section, name, "");

    const char* value = valstr.c_str();

    char* end;

    // This parses "1234" (decimal) and also "0x4D2" (hex)
    long n = strtol(value, &end, 0);

    return end > value ? n : default_value;
}

double IniReader::GetReal(const std::string& section, const std::string& name, double default_value)
{
    std::string valstr = Get(section, name, "");

    const char* value = valstr.c_str();

    char* end;

    double n = strtod(value, &end);

    return end > value ? n : default_value;
}

bool IniReader::GetBoolean(const std::string& section, const std::string& name, bool default_value)
{
    std::string valstr = Get(section, name, "");

    // Convert to lower case to make string comparisons case-insensitive
    std::transform(valstr.begin(), valstr.end(), valstr.begin(), ::tolower);

    if (valstr == "true" || valstr == "yes" || valstr == "on" || valstr == "1")
        return true;
    else if (valstr == "false" || valstr == "no" || valstr == "off" || valstr == "0")
        return false;
    else
        return default_value;
}

std::string IniReader::MakeKey(const std::string& section, const std::string& name)
{
    std::string key = section + "=" + name;

    // Convert to lower case to make section/name lookups case-insensitive
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);

    return key;
}

int IniReader::ValueHandler(void* user, const char* section, const char* name, const char* value)
{
    IniReader* reader = (IniReader*)user;

    std::string key = MakeKey(section, name);

    if (reader->_values[key].size() > 0)
        reader->_values[key] += "\n";

    reader->_values[key] += value;

    return 1;
}