#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
#
# $Id: progress.py,v 1.1 2005/12/22 23:49:57 grahn Exp $
# $Name:  $
#
# Copyright (c) 2005 Jörgen Grahn <grahn+src@snipabacken.dyndns.org>
# All rights reserved.
import os
import re

class Progress:
    def __init__(self, file, width=80):
        self._file = file
        self._width = width
        self._painted = 0
    def close(self):
        if self._painted:
            self._file.write('\n')
    def set(self, total, progress):
        newpaint = progress*self._width/total
        if newpaint > self._painted:
            self._file.write('#' * (newpaint - self._painted))
            self._file.flush()
        self._painted = newpaint;

def progress(file, feedback):
    pre = re.compile(r'Time: (\d\d):(\d\d).+?of (\d\d):(\d\d)')
    progress = Progress(feedback)
    fin, foe = os.popen4('ogg123 %s' % file, mode='b')
    s = 'x'
    while s:
        s = foe.read(100)
        m = pre.search(s)
        if m:
            tt = map(int, m.groups())
            progress.set(tt[2]*60+tt[3],
                         tt[0]*60+tt[1])
    progress.close()
    print foe.close()

if __name__ == "__main__":
    import os.path
    import getopt
    import sys

    prog = os.path.split(sys.argv[0])[1]
    usage = 'usage: %s file' % prog
    try:
        opts, files = getopt.getopt(sys.argv[1:], '')
        if opts or not len(files)==1:
            raise ValueError, 'bad args'
    except (ValueError, getopt.GetoptError), s:
        print >>sys.stderr, 'error:', s
        print >>sys.stderr, usage
        sys.exit(1)

    progress(files[0], sys.stdout)
