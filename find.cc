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
#include "find.h"
#include "basename.h"

#include <iostream>
#include <deque>

#include <sys/types.h>
#include <dirent.h>

using std::string;
using std::vector;
using std::deque;


namespace {

    struct Entry {
	explicit Entry(struct dirent* e)
	    : name(e->d_name),
	      dir(e->d_type==DT_DIR)
	{}
	string name;
	bool dir;
    };


    std::ostream& operator<< (std::ostream& os, const Entry& val)
    {
	const char* s = val.dir ? "/" : "";
	return os << val.name << s;
    }


    /**
     * Advance the state machine one step by picking one thing off the
     * backlog.  Might also add to the backlog.
     */
    void find(vector<Album>& v, deque<string>& backlog)
    {
	const string path = backlog.front();
	backlog.pop_front();

	DIR* dh = opendir(path.c_str());
	if(!dh) return;

	Album a(path);

	while(struct dirent* const e = readdir(dh)) {
	    if(e->d_name[0]=='.') continue;

	    switch(e->d_type) {
	    case DT_DIR:
		backlog.push_back(path::join(path, e->d_name));
		break;
	    case DT_REG:
		a.push_back(e->d_name);
		break;
	    default:
		break;
	    }
	}

	closedir(dh);

	if(!a.empty()) {
	    v.push_back(a);
	}
    }
}


/**
 * Kind of like find(1).  Find the directories ("Albums") starting at
 * 'path', and append them to 'v'.
 *
 * Symbolic links aren't followed.
 */
void pop::find(vector<Album>& v, const char* path)
{
    deque<string> backlog;
    backlog.push_back(path);
    while(!backlog.empty()) {
	::find(v, backlog);
    }
}
