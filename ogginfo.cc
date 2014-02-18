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
#include "ogginfo.h"
#include "info.h"

#include <vorbis/vorbisfile.h>


namespace {

    const char* find(const char* s, char ch)
    {
	while(*s && *s != ch) {
	    s++;
	}
	return s;
    }
}


TrackInfo ogg(const std::string& path)
{
    TrackInfo track;

    OggVorbis_File vf;
    int err = ov_fopen(path.c_str(), &vf);
    if(!err) {
	track.kind = TrackInfo::OGG;
	const vorbis_comment c = *ov_comment(&vf, -1);

	for(int i=0; i<c.comments; i++) {
	    const char* s = c.user_comments[i];
	    const char* eq = find(s, '=');
	    if(!*eq) continue;
	    const std::string key(s, eq-s);
	    const std::string val(eq+1);
	    if(key=="artist") track.artist = val;
	    else if(key=="album") track.album = val;
	    else if(key=="title") track.title = val;
	    else if(key=="date") track.date = val;
	    else if(key=="tracknumber") track.track = val;
	}

	ov_clear(&vf);
    }

    return track;
}
