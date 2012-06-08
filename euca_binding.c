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

#include "euca_binding.h"

int euca_run_instance(
    char * keyname,
    char * addressing_type,
    char * image_id)
{
    int i;

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
      return 1;
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
        char * c_argv[4] = {"./testmain", keyname, addressing_type, image_id};
        PySys_SetArgv(argc, c_argv);

        PyObject * ret;
        PyTuple_SetItem(argv, 0, argv_list);
        ret = PyObject_CallObject(f_run_inst, argv);
        if(!ret)
        {
          PyErr_Print();
          return 1;
        }

        Py_DECREF(pykeyname);
        Py_DECREF(pyaddtype);
        Py_DECREF(pyimageid);
        Py_DECREF(argv_list);
        Py_DECREF(argv);
    }
    else
    {
      PyErr_Print();
      return 1;
    }

    Py_Finalize();
}

int main(int argc, char * argv)
{
  euca_run_instance("leo", "private", "emi-A98314BC");
  return 0;
}
