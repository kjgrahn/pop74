/* -*- c++ -*-
 *
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
