/* -*- c++ -*-
 *
 * Copyright (c) 2010, 2014 Jörgen Grahn
 * All rights reserved.
 */
#ifndef GEESE_CHILD_H
#define GEESE_CHILD_H

#include <string>
#include <vector>


/**
 * fork+exec as an object.  Create it, try to start it, check if it
 * started, try to kill it or wait for it to die, see how it died.
 *
 * No way of communicating with the child, except by killing or
 * waiting.
 *
 * Destroying the object means killing the child.
 */
class Child {
public:
    template<class It> Child(It a, It b);
    ~Child();

    bool fork();
    bool running() const { return child > 0; }
    void kill();
    void wait();

    bool crashed() const;
    unsigned exit_status() const;

private:
    Child();
    Child(const Child&);
    Child& operator= (const Child&);

    const std::vector<std::string> v;
    int child;
    int status;
};


template<class It> Child::Child(It a, It b)
    : v(a, b),
      child(0)
{}

#endif
