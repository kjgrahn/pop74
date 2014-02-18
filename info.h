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
#ifndef POP74_INFO_H
#define POP74_INFO_H

#include <string>
#include <iosfwd>


/**
 * Is this a supported soundfile and if so,
 * what's its metadata?
 */
struct TrackInfo {

    enum Kind { UNKNOWN_KIND,
		OGG,
		MP3 };

    TrackInfo()
	: kind(UNKNOWN_KIND)
    {}

    Kind kind;
    std::string artist;
    std::string album;
    std::string title;
    std::string date;
    std::string track;
};


TrackInfo info(const std::string& path);

std::ostream& format_album(std::ostream& os, const TrackInfo& info,
			   const std::string& path);
std::ostream& format_track(std::ostream& os, const TrackInfo& info,
			   const std::string& path);

#endif
