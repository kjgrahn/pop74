/*
 * Copyright (c) 2014 J�rgen Grahn
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

    bool play(const std::string& f)
    {
	const char* const argv[] = { "ogg123",
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
	if(track.kind != TrackInfo::OGG) {
	    std::cerr << "error: skipping " << file << ": not ogg\n";
	    continue;
	}
	format(std::cout, track) << std::endl;
	if(!::play(file)) return false;
    }
    return true;
}