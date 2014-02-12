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
