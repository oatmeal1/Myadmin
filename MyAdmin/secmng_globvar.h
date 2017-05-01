
//  hpp		.h .cpp
// secmng_globvar.h

//1 保证全局变量 被定义且只定义一次 分配内存一次
//2 头文件在同一个cpp中被包含两次  和  头文件被多个地方包含 是两个不同的事情

#ifndef SECMNG_GLOB_VAR_H_
#define SECMNG_GLOB_VAR_H_

#include"stdafx.h"

//数据源全局变量
CString	g_dbSource, g_dbUse, g_dbpw;

// 建立10条连接 你应该如何实现
//  
CDatabase	myDB;
CDatabase	*g_pDB = NULL;

CString		g_SecMngIniName = "secmngadmin.ini";


#endif