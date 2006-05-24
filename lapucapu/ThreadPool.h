#ifndef THREADPOOL_H_
#define THREADPOOL_H_ 1
#include <glib.h>

class ThreadPool
{
	public:
		ThreadPool(GFunc func);
		ThreadPool(GFunc func, gpointer ud);
		~ThreadPool();
		void push(gpointer data);
	
	protected:
		// ThreadPool variables
	
		// TODO: add member variables...
	private:
		GFunc tf_; // wskaznik do funkcji dzialajacej jako watek
		gpointer user_data;
		GThreadPool *pula_;
		
	
};

#endif /*THREADPOOL_H_*/
