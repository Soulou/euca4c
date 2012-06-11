Small binding in C to use eucalyptus scripts euca2ools

## Changelog :
```c
char * euca_run_instance(char * keyname, char * addressing_type, char * image_id)
```
* Boot a VM
* Return the identifier of the VM     
* NULL if the operation fails
     
```c
int euca_terminate_instances(euca_instance_t ** instances, size_t nb_instances)
```
* Send shutdown signal to the instances
* 0 is OK, 1 is error

```c
euca_instance_t ** euca_describe_instances()
```
* Returns a NULL-terminated tab
* NULL if fails
 
```c
size_t euca_nb_instances()
```
* Return the number of instances
     


