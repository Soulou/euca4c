#!/usr/bin/python
# -*- coding: utf-8 -*-

# Software License Agreement (BSD License)
#
# Copyright (c) 2009, Eucalyptus Systems, Inc.
# All rights reserved.
#
# Redistribution and use of this software in source and binary forms, with or
# without modification, are permitted provided that the following conditions
# are met:
#
#   Redistributions of source code must retain the above
#   copyright notice, this list of conditions and the
#   following disclaimer.
#
#   Redistributions in binary form must reproduce the above
#   copyright notice, this list of conditions and the
#   following disclaimer in the documentation and/or other
#   materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# Author: Neil Soman neil@eucalyptus.com

import getopt
import sys
import os
from euca2ools import Euca2ool, InstanceValidationError, Util, \
    ConnectionFailed

usage_string = \
    """
Shows information about instances.

euca-describe-instances [-h, --help] [--version] [--debug]
[instance1... instanceN] 
"""

def euca_describe_instances():
    euca = None
    try:
        euca = Euca2ool()
    except Exception, e:
        print e
        sys.exit(1)

    for (name, value) in euca.opts:
        if name in ('-h', '--help'):
            usage(0)
        elif name == '--version':
            version()

    instance_ids = euca.process_args()
    try:
        for id in instance_ids:
            euca.validate_instance_id(id)
    except InstanceValidationError:
        print 'Invalid instance id'
        sys.exit(1)

    try:
        euca_conn = euca.make_connection()
    except ConnectionFailed, e:
        print e.message
        sys.exit(1)
    try:
        reservations = euca_conn.get_all_instances(instance_ids)
    except Exception, ex:
        euca.display_error_and_exit('%s' % ex)

    return reservations    


