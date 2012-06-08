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
  euca_run_instance("leo", "private", "emi-A98314BC");
  return 0;
}

