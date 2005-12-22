#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
#
# $Id: foo.py,v 1.1 2005/12/22 23:49:57 grahn Exp $
# $Name:  $
#
# Copyright (c) 2005 Jörgen Grahn <grahn+src@snipabacken.dyndns.org>
# All rights reserved.
import curses
import time

def main(w):
    w.addstr(0,0,'foo')
    w.addstr(5,5,'foo')
    w.addstr(15,15,'foo')
    w.addstr(25,25,'foo')
    w.refresh()
    w.getch()

curses.wrapper(main)
