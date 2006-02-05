#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
#
# $Id: album.py,v 1.1 2006/02/05 09:33:14 grahn Exp $
#
# Copyright (c) 2006 Jörgen Grahn
# All rights reserved.
#
"""directory as a music album

Treating a directory's content (Ogg Vorbis files) as an album,
with track numbers and track names.

Requires vorbiscomment(1), which comes with the vorbis-tools package.

Also, os.popen2 cannot be used like I do below (to avoid the shell and
its mangling of file names) on anything non-Unix.
"""
import os

def _tags(file):
    """Return the comment fields of a Vorbis file,
    as a list of tuples.
    Field names will be in lowercase. Duplicate fields
    and field order is preserved.
    """
    acc = []
    # this execv-style use of os.popen2 will
    # probably not work in some non-Unix OSen,
    # and it wasn't documented until Python 2.4.
    fd0, fd1 = os.popen2(['vorbiscomment',
                          '-l',
                          '--',
                          file])
    for s in fd1.xreadlines():
        name, value = s.strip().split('=', 1)
        acc.append((name.lower(), value))
    fd0.close(); fd1.close()
    return acc

class Track:
    def __init__(self, dir, filename, tags):
        self.path = os.path.join(dir, filename)
        self.artist = None
        self.album = None
        self.date = None
        self.title = None
        self.track = None
        for key, val in tags:
            if key=='artist':
                self.artist = val
            elif key=='album':
                self.album = val
            elif key=='date':
                self.date = val
            elif key=='title':
                self.title = val
            elif key=='tracknumber':
                self.track = int(val)

def _uniques(f, seq):
    """Apply 'f' across 'seq' and return a list of unique values.
    (Why is there no standard uniq() Python algorithm?)
    """
    ss = map(f, seq)
    ss.sort()
    if not ss:
        return ss
    last = ss[0]
    acc = [last]
    for s in ss[1:]:
        if s!=last:
            acc.append(s)
            last = s
    return acc

class Album:
    """An album and its sorted list of tracks.

    An album always has a title. It might also have a single artist
    and a single recording date (year, more likely) -- but different
    tracks might also be created by different artists, and/or in
    different years.

    Tracks may have their ordering defined by track numbers. If they
    don't, they are ordered by file name. Hopefully, they also have an
    artist and title.
    """
    def __init__(self, tracks):
        """Group a sequence of Track instances into an album.
        """
        aa = _uniques(lambda t: t.album, tracks)
        assert aa
        if len(aa)==1:
            self.album = aa[0]
        else:
            raise ValueError('not one single album')
        self.artist = None
        aa = _uniques(lambda t: t.artist, tracks)
        assert aa
        if len(aa)==1:
            self.artist = aa[0]
        self.date = None
        aa = _uniques(lambda t: t.date, tracks)
        assert aa
        if len(aa)==1:
            self.date = aa[0]
        if None in map(lambda t: t.track, tracks):
            # ok, we cannot trust track numbers,
            # make new ones based on file names
            tracks.sort(lambda a, b: cmp(a.path, b.path))
            for n, t in zip(len(tracks), tracks):
                t.track = n+1
        else:
            tracks.sort(lambda a, b: cmp(a.track, b.track))
        self.tracks = tracks
    def albuminfo(self):
        """Info common for the whole album, as a sequence for
        printing.  The album name is always present; artist and date
        are sometimes absent.
        """
        return filter(None, [self.artist,
                             self.album,
                             self.date])
    def trackinfo(self, track):
        """Info for a specific Track instance, as a sequence for printing.
        Fields common to all tracks are omitted, 
        """
        acc = []
        acc.append('%2d' % track.track)
        if not self.artist:
            acc.append(track.artist)
        if track.title:
            acc.append(track.title)
        else:
            acc.append('track %d' % track.track)
        if not self.date:
            acc.append(track.date)
        return acc
    def __str__(self):
        acc = []
        acc.append(' - '.join(self.albuminfo()))
        for t in self.tracks:
            acc.append(' - '.join(self.trackinfo(t)))
        return '\n'.join(acc)

def album(path):
    """Pull an Album instance from 'path', which needs
    to be a directory.
    """
    tracks = []
    names = os.listdir(path)
    for f in names:
        tags = _tags(os.path.join(path, f))
        tracks.append(Track(path, f, tags))
    return Album(tracks)
