/* -*- c++ -*-
 *
 * Copyright (c) 2013 Jörgen Grahn
 * All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef WAVINFO_BASENAME_H
#define WAVINFO_BASENAME_H

#include <string>

namespace path {

    std::string dirname(const std::string& path);
    std::string dirname(const char* path);

    std::string basename(const std::string& path);
    std::string basename(const char* path);

    std::string join(const std::string& a,
		     const std::string& b);
}

#endif
