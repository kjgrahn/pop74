/*
 * Copyright (c) 2014 Jörgen Grahn
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
#include "album.h"

#include <iostream>


Album::Album(const std::string& path)
    : path(path)
{}


Album& Album::push_back(const char* name)
{
    names.insert(name);
    return *this;
}


std::ostream& Album::put(std::ostream& os) const
{
    return os << path << " [" << names.size() << ']';
}


std::ostream& operator<< (std::ostream& os, const Album& val)
{
    return  val.put(os);
}
