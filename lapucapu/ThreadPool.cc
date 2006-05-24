//
// File: ThreadPool.cc
// Created by: User <Email>
// Created on: Sat Mar 25 13:45:52 2006
//
#include <iostream>
#include <glib.h>

#include "ThreadPool.h"

#define MAXWATKOW 100

ThreadPool::ThreadPool(GFunc func):tf_(func),user_data(NULL)
{
	std::cerr << "ThreadPool::ThreadPool(GFunc func):start" << std::endl;
	if( !g_thread_supported() )
		g_thread_init (NULL);
	
	pula_ = g_thread_pool_new(tf_,user_data, MAXWATKOW, FALSE,NULL);
	std::cerr << "ThreadPool::ThreadPool(GFunc func):koniec" << std::endl;
}


ThreadPool::ThreadPool(GFunc func, gpointer ud):tf_(func), user_data(ud)
{
	std::cerr << "ThreadPool::ThreadPool(GFunc func,gpointer ud):start" << std::endl;
	if( !g_thread_supported() )
		g_thread_init (NULL);
	
	pula_ = g_thread_pool_new(tf_,user_data, MAXWATKOW, FALSE,NULL);
	std::cerr << "ThreadPool::ThreadPool(GFunc func,gpointer ud):koniec" << std::endl;
}


ThreadPool::~ThreadPool()
{
	if( pula_ )
		g_thread_pool_free(pula_, TRUE, FALSE);
}


void ThreadPool::push(gpointer data)
{
	g_thread_pool_push(pula_,data, NULL);
}
