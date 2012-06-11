/*
 * @File : euca_binding.h
 *
 * @Author : Unbekandt Leo <leo@soulou.fr>
 *
 * @Summary : Eucalyptus binding 
 *
 * @Version : 2012/06/08
 *
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <leo@soulou.fr> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return 
 * ----------------------------------------------------------------------------
 */

#ifndef _EUCA_BINDING_H
#define _EUCA_BINDING_H

#include <python2.7/Python.h>

typedef PyObject PyList;
typedef PyObject PyString;
typedef PyObject PyModule;
typedef PyObject PyFunc;
typedef PyObject PyTuple;

// Structure which must represent an vm instance 
typedef struct euca_instance_t
{
    char * id;
    char * ip;
} euca_instance_t;

// Create a new VM
euca_instance_t * euca_run_instance(
    char * keyname,
    char * addressing_type,
    char * image_id);

// Terminate multiple of VM 
int euca_terminate_instances(
    euca_instance_t ** instances, size_t nb);


#endif
