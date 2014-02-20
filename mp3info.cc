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
#include "mp3info.h"
#include "info.h"

#include <id3tag.h>

#include <cstdlib>


namespace {

    std::string latin(const id3_latin1_t* s)
    {
	std::string ss;
	if(s) {
	    ss = reinterpret_cast<const char*>(s);
	}
	return ss;
    }

    std::string latin(const id3_ucs4_t* s)
    {
	std::string ss;
	if(s) {
	    id3_latin1_t* c = id3_ucs4_latin1duplicate(s);
	    if(c) {
		ss = reinterpret_cast<char*>(c);
		std::free(c);
	    }
	}
	return ss;
    }

    std::string str(const id3_tag* tag, const char* name)
    {
	std::string s;
	const id3_frame* f = id3_tag_findframe(tag, name, 0);
	if(!f) return s;

	unsigned i=0;
	while(const id3_field* fi = id3_frame_field(f, i++)) {

	    const enum id3_field_type type = id3_field_type(fi);
	    switch(type) {
	    case ID3_FIELD_TYPE_LATIN1:
		s = latin(id3_field_getlatin1(fi));
		break;
	    case ID3_FIELD_TYPE_LATIN1FULL:
		s = latin(id3_field_getfulllatin1(fi));
		break;
	    case ID3_FIELD_TYPE_LATIN1LIST:
		/* XXX huh? */
		break;
	    case ID3_FIELD_TYPE_STRING:
		s = latin(id3_field_getstring(fi));
		break;
	    case ID3_FIELD_TYPE_STRINGFULL:
		s = latin(id3_field_getfullstring(fi));
		break;
	    case ID3_FIELD_TYPE_STRINGLIST:
		s = latin(id3_field_getstrings(fi, 0));
		break;
	    default:
		break;
	    }
	}

	return s;
    }
}


TrackInfo mp3(const std::string& path)
{
    TrackInfo track;

    id3_file* const f = id3_file_open(path.c_str(),
				      ID3_FILE_MODE_READONLY);
    const id3_tag* const t = id3_file_tag(f);
    if(t) {
	track.kind   = TrackInfo::MP3;
	track.artist = str(t, ID3_FRAME_ARTIST);
	track.album  = str(t, ID3_FRAME_ALBUM);
	track.title  = str(t, ID3_FRAME_TITLE);
	track.date   = str(t, ID3_FRAME_YEAR);
	track.track  = str(t, ID3_FRAME_TRACK);
    }

    id3_file_close(f);
    return track;
}
