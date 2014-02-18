/*
 * Copyright (c) 2013 Jörgen Grahn
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
#include "basename.h"

#include <algorithm>
#include <cstring>


namespace {

    const char* find_last(const char* a, const char* b, char c)
    {
	const char* p = &c;
	return std::find_end(a, b, p, p+1);
    }

    /**
     * Trim 'c' from the end of [a, b) but don't make the range
     * empty.  Returns the new 'b'.
     */
    const char* trim_end(const char* a, const char* b, const char c)
    {
	while(b-a > 2) {
	    const char* e = b-1;
	    if(*e!=c) {
		break;
	    }
	    b = e;
	}
	return b;
    }
}


namespace path {

    static const std::string dot(".");

    std::string dirname(const char* a, const char* b)
    {
	if(a==b) return dot;

	b = trim_end(a, b, '/');
	const char* c = find_last(a, b, '/');
	if(c==b) return dot;
	if(c==a) c++;
	b = trim_end(a, c, '/');

	return std::string(a, b);
    }

    std::string basename(const char* a, const char* b)
    {
	if(a==b) return dot;

	b = trim_end(a, b, '/');
	const char* c = find_last(a, b, '/');
	if(c!=b) {
	    a = c+1;
	}
	return std::string(a, b);
    }

    std::string dirname(const std::string& path)
    {
	const char* p = path.c_str();
	return dirname(p, p + path.size());
    }

    std::string dirname(const char* path)
    {
	return dirname(path, path + std::strlen(path));
    }

    std::string basename(const std::string& path)
    {
	const char* p = path.c_str();
	return basename(p, p + path.size());
    }

    std::string basename(const char* path)
    {
	return basename(path, path + std::strlen(path));
    }

    std::string join(const std::string& a,
		     const std::string& b)
    {
	const char* p = a.c_str();
	const char* q = trim_end(p, p+a.size(), '/');
	std::string s(p, q);
	s.push_back('/');
	s += b;
	return s;
    }
}
