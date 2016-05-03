/*************************************************************************
Copyright (c) 2013-2015, Autelan Networks. All rights reserved.

This Software is the property of Autelan Networks. 
*************************************************************************/

/*
 * File version info: 
 *
 */
#ifndef __AUTEOS_BASE_H__
#define __AUTEOS_BASE_H__
/******************************************************************************/
#define autelan_replaced(_function)     autelan_replaced_##_function
#define autelan_forbidden(_function)    autelan_forbidden_##_function

#define autelan_used_file64             0
#define autelan_used_wchar              0

/******************************************************************************/
#endif /* __AUTEOS_BASE_H__ */