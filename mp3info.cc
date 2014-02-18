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

#include <id3/tag.h>
#include <id3/misc_support.h>

namespace {

    std::string str(const char* s)
    {
	if(s) return s;
	return "";
    }
}


TrackInfo mp3(const std::string& path)
{
    TrackInfo track;

    const ID3_Tag tag(path.c_str());
    const ID3_Tag* const p = &tag;

    track.kind   = TrackInfo::MP3;
    track.artist = str(ID3_GetArtist(p));
    track.album  = str(ID3_GetAlbum(p));
    track.title  = str(ID3_GetTitle(p));
    track.date   = str(ID3_GetYear(p));
    track.track  = str(ID3_GetTrack(p));
    return track;
}
