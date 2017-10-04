/**
 *    Copyright 2017 MongoDB, Inc.
 *
 *    This program is free software: you can redistribute it and/or  modify
 *    it under the terms of the GNU Affero General Public License, version 3,
 *    as published by the Free Software Foundation.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Affero General Public License for more details.
 *
 *    You should have received a copy of the GNU Affero General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *    As a special exception, the copyright holders give permission to link the
 *    code of portions of this program with the OpenSSL library under certain
 *    conditions as described in each individual source file and distribute
 *    linked combinations including the program with the OpenSSL library. You
 *    must comply with the GNU Affero General Public License in all respects
 *    for all of the code used other than as permitted herein. If you modify
 *    file(s) with this exception, you may extend this exception to your
 *    version of the file(s), but you are not obligated to do so. If you do not
 *    wish to do so, delete this exception statement from your version. If you
 *    delete this exception statement from all source files in the program,
 *    then also delete it in the license file.
 */

#pragma once

#include <cstdint>
#include <iosfwd>
#include <string>

#include "mongo/base/string_data.h"

namespace mongo {

/**
 * This is a generated class containing a table of error codes and their corresponding error
 * strings. The class is derived from the definitions in src/mongo/base/error_codes.err file and the
 * src/mongo/base/error_codes.tpl.h template.
 *
 * Do not update this file directly. Update src/mongo/base/error_codes.err instead.
 */
class ErrorCodes {
public:
    // Explicitly 32-bits wide so that non-symbolic values,
    // like uassert codes, are valid.
    enum Error : std::int32_t {
        //#for $ec in $codes
        $ec.name = $ec.code,
        //#end for
        MaxError
    };

    static std::string errorString(Error err);

    /**
     * Parses an Error from its "name".  Returns UnknownError if "name" is unrecognized.
     *
     * NOTE: Also returns UnknownError for the string "UnknownError".
     */
    static Error fromString(StringData name);

    /**
     * Casts an integer "code" to an Error.  Unrecognized codes are preserved, meaning
     * that the result of a call to fromInt() may not be one of the values in the
     * Error enumeration.
     */
    static Error fromInt(int code) {
        return static_cast<Error>(code);
    }

    //#for $cat in $categories
    static bool is${cat.name}(Error err);
    //#end for
};

std::ostream& operator<<(std::ostream& stream, ErrorCodes::Error code);

}  // namespace mongo