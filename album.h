/* -*- c++ -*-
 *
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
#ifndef POP74_ALBUM_H
#define POP74_ALBUM_H

#include <string>
#include <set>
#include <iosfwd>

/**
 * Pretending that a directory and its non-directory contents
 * represents an album, or other collection of related audio files.
 *
 */
class Album {
public:
    explicit Album(const std::string& path);
    Album& push_back(const char* name);

    bool empty() const { return names.empty(); }

    std::ostream& put(std::ostream& os) const;
    bool play() const;

private:
    std::string path;
    std::set<std::string> names;

    typedef std::set<std::string>::const_iterator const_iterator;
    const_iterator begin() const { return names.begin(); }
    const_iterator end() const { return names.end(); }
};

std::ostream& operator<< (std::ostream& os, const Album& val);

#endif
