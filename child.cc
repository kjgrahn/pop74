/*
 * Copyright (c) 2010, 2014 Jörgen Grahn
 * All rights reserved.
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
