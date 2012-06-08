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

euca_instance_t * euca_run_instance(
    char * keyname,
    char * addressing_type,
    char * image_id)
{
    euca_instance_t * vm = malloc(sizeof(euca_instance_t));

    Py_Initialize();
    PyList * path = PySys_GetObject("path");
    PyString * cur_dir = PyString_FromString(".");
    PyList_Append(path, cur_dir);

    PyString * import_name = PyString_FromString("euca_run_instance");
    PyList * import_list = PyList_New(0);
    PyList_Append(import_list, import_name);

    PyModule * module = PyImport_ImportModule("euca_run_instance");
   
    if(!module)
    {
      PyErr_Print();
      return NULL;
    }

    // INCREF has been done in PyString_FromString
    // We don't need anymore this object, lets python
    // free it by removing the last reference to it.
    Py_DECREF(import_name);
    Py_DECREF(import_list);


    PyTuple * argv = PyTuple_New(1);
    PyList * argv_list = PyList_New(0);

    PyString * pykeyname = PyString_FromString(keyname);
    PyString * pyaddtype = PyString_FromString(addressing_type);
    PyString * pyimageid = PyString_FromString(image_id);

    PyList_Append(argv_list, pykeyname);
    PyList_Append(argv_list, pyaddtype);
    PyList_Append(argv_list, pyimageid);

    PyFunc * f_run_inst = PyObject_GetAttrString(module, "euca_run_instance");

    if(f_run_inst && PyCallable_Check(f_run_inst))
    {
        int argc = 4;
        char * c_argv[4] = {"noname" , keyname, addressing_type, image_id};
        PySys_SetArgv(argc, c_argv);

        PyObject * reservation;
        PyTuple_SetItem(argv, 0, argv_list);
        reservation = PyObject_CallObject(f_run_inst, argv);
        if(!reservation)
        {
          PyErr_Print();
          return NULL;
        }

        Py_DECREF(pykeyname);
        Py_DECREF(pyaddtype);
        Py_DECREF(pyimageid);
        Py_DECREF(argv_list);
        Py_DECREF(argv);


        PyList * instances = PyObject_GetAttrString(reservation, "instances");
        if(!instances)
        {
          PyErr_Print();
          return NULL;
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
      return NULL;
    }

    Py_Finalize();
    return vm;
}

