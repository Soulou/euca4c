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
        vm->ip = PyString_AsString(
                   PyObject_GetAttrString(
                     PyList_GetItem(instances, 0),
                     "public_dns_name")
                 );
        vm->state = PyString_AsString(
                   PyObject_GetAttrString(
                     PyList_GetItem(instances, 0),
                     "state")
                 );


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

euca_instance_t ** euca_describe_instances()
{
  Py_Initialize();
  PyList * instances = PyList_New(0);
  PyList * r_instances;
  PyList * reservations;
  PyObject * reservation;
  PyObject * instance;
  int i,j,nb_instances;
  
  char * argv[1] = {"noname"};
  PySys_SetArgv(1, argv);

  PyModule * m = py_load_module("euca_describe_instances");
  if(!m) return NULL;

  PyFunc * f = PyObject_GetAttrString(m, "euca_describe_instances");
  if(f && PyCallable_Check(f))
  {
      reservations = PyObject_CallObject(f, NULL);
      if(!reservations) return NULL;
  }
  else
      return NULL;

  for(i = 0; i < PyList_Size(reservations); i++)
  {
      reservation = PyList_GetItem(reservations, i);
      r_instances = PyObject_GetAttrString(reservation, "instances");
      nb_instances = PyList_Size(r_instances);
      if(nb_instances < 0)
          PyErr_Print();

      for(j = 0; j < nb_instances; j++)
      {
          instance = PyList_GetItem(r_instances, j);
          PyList_Append(instances, instance);
      }
  }

  nb_instances = PyList_Size(instances);
  euca_instance_t ** c_instances = (euca_instance_t **)malloc(sizeof(euca_instance_t*)*(nb_instances+1));
  c_instances[nb_instances] = NULL;
  for(i = 0; i < nb_instances; i++)
  {
      c_instances[i] = (euca_instance_t*)malloc(sizeof(euca_instance_t));
      instance = PyList_GetItem(instances, i);
      c_instances[i]->id = PyString_AsString(
                             PyObject_GetAttrString(instance, "id"));
      c_instances[i]->ip = PyString_AsString(
                             PyObject_GetAttrString(instance, "public_dns_name"));
      c_instances[i]->state = PyString_AsString(
                             PyObject_GetAttrString(instance, "state"));
  }

  Py_Finalize();
  return c_instances;
}

size_t euca_nb_instances()
{
  Py_Initialize();
  PyList * instances = PyList_New(0);
  PyList * r_instances;
  PyList * reservations;
  PyObject * reservation;
  PyObject * instance;
  int i,j,nb_instances;
  
  char * argv[1] = {"noname"};
  PySys_SetArgv(1, argv);

  PyModule * m = py_load_module("euca_describe_instances");
  if(!m) return -1;

  PyFunc * f = PyObject_GetAttrString(m, "euca_describe_instances");
  if(f && PyCallable_Check(f))
  {
      reservations = PyObject_CallObject(f, NULL);
      if(!reservations) return -1;
  }
  else
      return -1;

  for(i = 0; i < PyList_Size(reservations); i++)
  {
      reservation = PyList_GetItem(reservations, i);
      r_instances = PyObject_GetAttrString(reservation, "instances");
      nb_instances = PyList_Size(r_instances);
      if(nb_instances < 0)
          PyErr_Print();

      for(j = 0; j < nb_instances; j++)
      {
          instance = PyList_GetItem(r_instances, j);
          PyList_Append(instances, instance);
      }
  }

  size_t res = PyList_Size(instances);
  Py_Finalize();
  return res;
}
