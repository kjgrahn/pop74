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
