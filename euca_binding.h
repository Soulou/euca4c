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

#include <Python.h>

typedef PyObject PyList;
typedef PyObject PyString;
typedef PyObject PyModule;
typedef PyObject PyFunc;
typedef PyObject PyTuple;

int euca_run_instances(
    char * keyname,
    char * addressing_type,
    char * image_id);

#endif

