#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
#
# $Id: ui.py,v 1.1 2006/02/05 14:00:45 grahn Exp $
#
# Copyright (c) 2005 Jörgen Grahn <grahn-src@snipabacken.dyndns.org>
# All rights reserved.
#
import curses.wrapper
import album
import time

class UI:
    def __init__(self, window, albums):
        self.window = window
        curses.curs_set(0)
        self.aa = albums
        self.point = 0, 0
        self.display()
    def display(self):
        write = self.window.addstr
        i = 0
        for n, a in zip(range(len(albums)), albums):
            if n==self.point[0]:
                write(i, 0,
                      ' - '.join(a.albuminfo()),
                      curses.A_BOLD)
                i+=1
                tt = a.tracks
                for n, t in zip(range(len(tt)), tt):
                    if n==self.point[1]:
                        attr = curses.A_REVERSE
                    else:
                        attr = curses.A_NORMAL
                    write(i, 0, ' - '.join(a.trackinfo(t)),
                          attr)
                    i+=1
            else:
                write(i, 0,
                      ' - '.join(a.albuminfo()),
                      curses.A_BOLD)
                i+=1
        self.window.refresh()
    def run(self):
        while 1:
            n = self.window.getch()
            a, t = self.point
            if n==curses.KEY_UP:
                if t:
                    self.point = a, t-1
                    self.display()
                elif a:
                    self.point = a-1, 0
                    self.display()                    
            elif n==curses.KEY_DOWN:
                if t<=len(self.aa[a].tracks)-1:
                    self.point = a, t+1
                    self.display()
                elif a<=len(self.aa)-1:
                    self.point = a+1, 0
                    self.display()
            else:
                return

def fun(window, albums):
    ui = UI(window, albums)
    return ui.run()

if __name__ == "__main__":
    albums = map(album.album,
                 ['Kill_Uncle',
                  'Vauxhall_and_I',
                  'Head_Your_Mind',
                  'You_are_the_Quarry'])
    curses.wrapper(fun, albums)
