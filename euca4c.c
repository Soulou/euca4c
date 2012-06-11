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

#include <stdlib.h>
#include "euca4c.h"

#define INST_NULL (euca_instance_t *)NULL

static PyModule * py_load_module(char * name)
{
    PyString * import_name = PyString_FromString(name);
    PyList * import_list = PyList_New(0);
    PyList_Append(import_list, import_name);

    PyModule * module = PyImport_ImportModule(name);
   
    if(!module)
    {
      PyErr_Print();
      return (PyModule*)NULL;
    }

    Py_DECREF(import_name);
    Py_DECREF(import_list);
    return module;

}

euca_instance_t * euca_run_instance(
    char * keyname,
    char * addressing_type,
    char * image_id)
{
    Py_Initialize();

    int argc = 4;
    char * c_argv[4] = {"noname" , keyname, addressing_type, image_id};
    PySys_SetArgv(argc, c_argv);

    euca_instance_t * vm = (euca_instance_t*)malloc(sizeof(euca_instance_t));

    PyModule * module = py_load_module("euca_run_instance");
    if(!module) return INST_NULL;

    PyFunc * f = PyObject_GetAttrString(module, "euca_run_instance");
    if(f && PyCallable_Check(f))
    {
        PyObject * reservation;
        reservation = PyObject_CallObject(f, NULL);
        if(!reservation)
        {
          PyErr_Print();
          return INST_NULL;
        }

        PyList * instances = PyObject_GetAttrString(reservation, "instances");
        if(!instances)
        {
          PyErr_Print();
          return INST_NULL;
        }

        vm->id = PyString_AsString(
                   PyObject_GetAttrString(
                     PyList_GetItem(instances, 0),
                     "id")
                 );
        /* vm->ip = PyString_AsString( */
        /*            PyObject_GetAttrString( */
        /*              PyList_GetItem(instances, 0), */
        /*              "public_dns_name") */
        /*          ); */


        Py_DECREF(instances);
        Py_DECREF(reservation);
    }
    else
    {
      PyErr_Print();
      return INST_NULL;
    }

    Py_Finalize();
    return vm;
}

// Terminate multiple of VM 
int euca_terminate_instances(
    euca_instance_t ** instances, size_t nb_inst)
{
    Py_Initialize();

    int argc = nb_inst+1;
    char * argv[nb_inst+2];
    argv[0] = "noname";
    for(int i = 1; i <= nb_inst ; i++)
        argv[i] = instances[i-1]->id;
    argv[nb_inst+1] = NULL;
    PySys_SetArgv(argc, argv);

    PyModule * m = py_load_module("euca_terminate_instances");
    if(!m) return 1;

    PyFunc * f = PyObject_GetAttrString(m, "euca_terminate_instances");
    PyObject * ret;
    long iret;

    if(f && PyCallable_Check(f))
    {
        ret = PyObject_CallObject(f, NULL);
        iret = PyInt_AsLong(ret);
        if(iret) return 1;
    }
    else
      return 1;

    Py_Finalize();
    return 0;
}

