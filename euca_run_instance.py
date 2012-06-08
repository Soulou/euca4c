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
from euca2ools import Euca2ool, Util, ConnectionFailed

def version():
    print Util().version()
    sys.exit()


def display_reservations(reservation):
    reservation_string = '%s\t%s' % (reservation.id,
            reservation.owner_id)
    group_delim = '\t'
    for group in reservation.groups:
        reservation_string += '%s%s' % (group_delim, group.id)
        group_delim = ', '
    print 'RESERVATION\t%s' % reservation_string
    for instance in reservation.instances:
        instance_string = '%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s' % (
            instance.id,
            instance.image_id,
            instance.public_dns_name,
            instance.private_dns_name,
            instance.state,
            instance.key_name,
            instance.launch_time,
            instance.kernel,
            instance.ramdisk,
            )
        print 'INSTANCE\t%s' % instance_string


def read_user_data(user_data_filename):
    USER_DATA_CHUNK_SIZE = 512
    user_data = ''
    user_data_file = open(user_data_filename, 'r')
    while 1:
        data = user_data_file.read(USER_DATA_CHUNK_SIZE)
        if not data:
            break
        user_data += data
    user_data_file.close()
    return user_data


def euca_run_instance(argv=None):
    euca = None
    try:
        euca = Euca2ool()
    except Exception, e:
        print e
        sys.exit(1)

    image_id = argv[2]
    keyname = argv[0]
    kernel_id = None
    ramdisk_id = None
    min_count = 1
    max_count = 1
    instance_type = 'm1.small'
    group_names = []
    user_data = None
    user_data_file = None
    addressing_type = argv[1]
    zone = None
    block_device_map_args = []
    block_device_map = None
    monitor = False
    subnet_id = None

    if image_id:
        try:
            euca_conn = euca.make_connection()
        except ConnectionFailed, e:
            print e.message
            sys.exit(1)
        if block_device_map_args:
            block_device_map = \
                euca.parse_block_device_args(block_device_map_args)
        try:
            reservation = euca_conn.run_instances(
                image_id=image_id,
                min_count=min_count,
                max_count=max_count,
                key_name=keyname,
                security_groups=group_names,
                user_data=user_data,
                addressing_type=addressing_type,
                instance_type=instance_type,
                placement=zone,
                kernel_id=kernel_id,
                ramdisk_id=ramdisk_id,
                block_device_map=block_device_map,
                monitoring_enabled=monitor,
                subnet_id=subnet_id
                )
        except Exception, ex:
            euca.display_error_and_exit('%s' % ex)

        #display_reservations(reservation)
        return reservation
    else:
        print 'image_id must be specified'

