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
#include "info.h"

#include "ogginfo.h"
#include "mp3info.h"
#include "basename.h"

#include <iostream>


TrackInfo info(const std::string& path)
{
    TrackInfo track = ogg(path);
    if(track.kind!=TrackInfo::OGG) {
	track = mp3(path);
    }
    return track;
}


namespace {

    /**
     * The last two components of a path.
     */
    std::string last2(const std::string& path)
    {
	const std::string b = path::basename(path);
	const std::string a = path::basename(path::dirname(path));
	return path::join(a, b);
    }

    bool good(const std::string& s)
    {
	return !s.empty();
    }
}


/**
 * Print information about an album, based on the first song/track to
 * play.
 */
std::ostream& format_album(std::ostream& os, const TrackInfo& info,
			   const std::string& dir)
{
    /* In descending priority:
     *
     * 1. Juliana's Pony: Total System Failure (2000)
     * 2. Juliana's Pony: Total System Failure
     * 3. Juliana's Pony: Total_System_Failure/
     * 4. Juliana_Hatfield/Total_System_Failure/
     */
    if(good(info.artist)) {
	if(good(info.album)) {
	    if(good(info.date)) {
		os << info.artist << ": "
		   << info.album << " (" << info.date << ')';
	    }
	    else {
		os << info.artist << ": " << info.album;
	    }
	}
	else {
	    os << info.artist << ": "
	       << path::basename(dir) << '/';
	}
    }
    else {
	os << last2(dir) << '/';
    }
    return os;
}


/**
 * Print information about a song/track, immediately before playing
 * it.  The album is assumed to have been printed recently.
 */
std::ostream& format_track(std::ostream& os, const TrackInfo& info,
			   const std::string& path)
{
    /* In descending priority:
     *
     * 1. Juliana's Pony: Metal Fume Fever
     * 2. Juliana's Pony: Total System Failure, track 2
     * 3. Juliana's Pony: Total_System_Failure_02.ogg
     * 4. Total_System_Failure/Total_System_Failure_02.ogg
     */
    os << "    ";
    if(good(info.artist)) {
	if(good(info.title)) {
	    os << info.artist << ": " << info.title;
	}
	else if(good(info.album) && good(info.track)) {
	    os << info.artist << ": " << info.album
	       << ", track " << info.track;
	}
	else {
	    os << info.artist << ": " << last2(path);
	}
    }
    else {
	os << last2(path);
    }
    return os;
}
