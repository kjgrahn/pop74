/*
 * Copyright (c) 2014 Jörgen Grahn
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
