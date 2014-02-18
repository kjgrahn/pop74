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

#include "info.h"
#include "basename.h"
#include "child.h"

#include <iostream>


namespace {

    bool play(const std::string& f,
	      TrackInfo::Kind kind)
    {
	bool ov = (kind==TrackInfo::OGG);
	const char* const argv[] = { ov ? "ogg123" : "mpg321",
				     "-q",
				     "--",
				     f.c_str() };
	Child ogg(argv, argv+4);
	ogg.fork();
	ogg.wait();
	return !ogg.crashed() && ogg.exit_status()==0;
    }
}


/**
 * Play the album, from start to finish.
 */
bool Album::play() const
{
    for(const_iterator i = begin(); i!=end(); i++) {

	const std::string file = path::join(path, *i);
	const TrackInfo track = info(file);
	switch(track.kind) {
	case TrackInfo::OGG:
	case TrackInfo::MP3:
	    if(i==begin()) {
		format_album(std::cout, track, path) << std::endl;
	    }
	    format_track(std::cout, track, file) << std::endl;
	    if(!::play(file, track.kind)) return false;
	    break;
	default:
	    std::cerr << "error: skipping " << file << ": not playable\n";
	}
    }
    return true;
}
