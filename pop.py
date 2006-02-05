#!/usr/bin/env python
# -*- coding: iso-8859-1 -*-
#
# $Id: pop.py,v 1.1 2006/02/05 09:33:14 grahn Exp $
#
# Copyright (c) 2005 Jörgen Grahn <grahn-src@snipabacken.dyndns.org>
# All rights reserved.
#

import os

f = os.popen('ogg123 foo.ogg', 'r')
while 1:
    s = f.readline()
    print '#', s, '%'
print f.close()
