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
#include <string>
#include <iostream>
#include <stdio.h>
#include <getopt.h>

#include "find.h"


namespace {

    const char* version()
    {
	return "0.1";
    }

    void pop74(const char* directory)
    {
        std::vector<Album> v;

	pop::find(v, directory);
	for(std::vector<Album>::const_iterator i = v.begin();
	    i != v.end();
	    i++) {

	    i->play();
	}
    }
}


int main(int argc, char ** argv)
{
    using std::string;

    const string prog = argv[0] ? argv[0] : "pop74";
    const string usage = string("usage: ")
	+ prog + " directory\n"
	"       "
	+ prog + " --version\n"
	"       "
	+ prog + " --help";
    const char optstring[] = "";
    const struct option long_options[] = {
	{"version", 0, 0, 'V'},
	{"help", 0, 0, 'H'},
	{0, 0, 0, 0}
    };

    std::cin.sync_with_stdio(false);
    std::cout.sync_with_stdio(false);

    int ch;
    while((ch = getopt_long(argc, argv,
			    optstring,
			    &long_options[0], 0)) != -1) {
	switch(ch) {
	case 'V':
	    std::cout << prog << ' ' << version() << '\n'
		      << "Copyright (c) 2014 Jörgen Grahn\n";
	    return 0;
	    break;
	case 'H':
	    std::cout << usage << '\n';
	    return 0;
	    break;
	case ':':
	case '?':
	    std::cerr << usage << '\n';
	    return 1;
	    break;
	default:
	    break;
	}
    }

    if(argv+optind+1 != argv+argc) {
	std::cerr << usage << '\n';
	return 1;
    }

    const char* const directory = argv[optind];

    pop74(directory);

    return 0;
}
