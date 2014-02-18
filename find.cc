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
