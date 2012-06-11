/*
 * @File : euca_run_instance.c
 *
 * @Author : leo@soulou.fr
 *
 * @Summary : Test file for euca4c binding
 *
 * @Version : 2012/06/08
 *
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * leo@soulou.fr wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return 
 * ----------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>

#include <euca4c.h>

int main(int argc, char ** argv)
{
  euca_instance_t ** instances = euca_describe_instances();
  if(instances == NULL)
    fprintf(stderr, "Erreur, instances NULL\n");
  while(*instances != NULL)
  {
    printf("Instance : %s\n", (*instances)->id);
    printf("\tÉtat : %s\n", (*instances)->state);
    printf("\tIP : %s\n", (*instances)->ip);
    instances++;
  }

  printf("Il y a %u instances.\n", (unsigned int)euca_nb_instances());

  return 0;
}

