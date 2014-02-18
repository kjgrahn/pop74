/*
 * Copyright (c) 2010, 2014 Jörgen Grahn
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
#include "child.h"

#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <cstdlib>
#include <cassert>

#include <iostream>


bool Child::fork()
{
    child = ::fork();
    if(child) return child!=-1;

    std::vector<char*> argv;
    for(std::vector<std::string>::const_iterator i = v.begin();
	i != v.end(); i++) {
	argv.push_back(strdup(i->c_str()));
    }
    argv.push_back(0);

    execvp(argv[0], argv.data());
    std::cerr << "failed to exec '" << argv[0]
	      << "': " << strerror(errno) << '\n';
    exit(1);
}

Child::~Child()
{
    if(running()) {
	kill();
	wait();
    }
}

void Child::kill()
{
    if(running()) {
	::kill(child, SIGINT);
    }
}

void Child::wait()
{
    assert(running());
    waitpid(child, &status, 0);
    child = 0;
}

bool Child::crashed() const
{
    assert(!running());
    return !WIFSIGNALED(status);
}

unsigned Child::exit_status() const
{
    assert(!running());
    return WEXITSTATUS(status);
}
