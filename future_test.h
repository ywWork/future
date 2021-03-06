#ifndef _FUTURE_H
#define _FUTURE_H


#include<abt.h>
#include<cstdlib>
#include<iostream>
#include<chrono>
#include<iterator>

#include "thread.h"


namespace stdx 
{
	/* "launch" related */
	enum class launch  
	{
		async = 1,	
		deferred = 2	
	};
	constexpr launch operator&(launch __x, launch __y)
	{
		return static_cast<launch>(
				static_cast<int>(__x) & static_cast<int>(__y));
	}
	constexpr launch operator|(launch __x, launch __y)
	{
		return static_cast<launch>(
				static_cast<int>(__x) | static_cast<int>(__y));
	}
	constexpr launch operator^(launch __x, launch __y)
	{
		return static_cast<launch>(
				static_cast<int>(__x) ^ static_cast<int>(__y));
	}
	constexpr launch operator~(launch __x)
	{ return static_cast<launch>(~static_cast<int>(__x)); }
	inline launch& operator&=(launch& __x, launch __y)
	{ return __x = __x & __y; }
	inline launch& operator|=(launch& __x, launch __y)
	{ return __x = __x | __y; }
	inline launch& operator^=(launch& __x, launch __y)
	{ return __x = __x ^ __y; }


	enum class future_status 
	{
		ready,
		timeout,
		deferred
	};

	template<class ss_T>
	struct shared_state
	{
		ss_T value_;
	};

	// template<class promise_T>
	// class promise;

	template<class future_T>
	class future
	{
		typedef struct 
		{
			future_T future_T_dup_;
			void(*func_)(void*);
			ABT_eventual eventual_;
		}wrapper_args;

		stdx::thread t1_;
		int eventual_flag_;
		int ready_flag_;
		int deferred_flag_;

		template<class arg_T>
		friend 
		future<arg_T>
		async(void(*func)(void*), arg_T args);

		template<class ret_T>
		friend void wrapper (void*); 


		public:
			future (); 
			~future (); 

			void * ptr;
			shared_state<future_T> * ss_ptr_;
			shared_state<future_T> ss_;
			future(const future& other) = delete;
			future(future&& other);
			future_T get();

			void wait ();
			bool valid ();

		    wrapper_args wa_;

			template <class Rep, class Period>
			stdx::future_status 
			wait_for (const std::chrono::duration<Rep, Period>& time);

			template <class Clock, class Duration>
			stdx::future_status
			wait_until (const chrono::time_point<Clock,Duration>& abs_time);

			// template<class Fn, class ...Args>
			// future_T
			// wrapper(wrapper_args* wa);

			void operator=(future<future_T>&& other);
	};
	
	template<class promise_T>
	class promise 
	{

		shared_state<promise_T>  ss_;
		ABT_eventual promise_eventual_;


		template<class arg_T>
		friend 
		future<arg_T>
		async(void(*func)(void*), arg_T args);


		friend class stdx::thread;

		public:
			promise(){}
			~promise(){}

			future<promise_T>
			get_future();

			template<class set_value_T>
			void 
			set_value(set_value_T && arg_in);	

			template<class set_value_T>
			void
			set_value_at_thread_exit (set_value_T && arg_in);
	};


	template<class arg_T>
	future<arg_T>
	// async (stdx::launch &policy, void (*func) (void*), arg_T args)
	async (launch policy, void (*func) (void*), arg_T args);

	template<class ret_T>
	// ret_T	
	void
	wrapper(void* wa);
}

#include"class_future.cpp"
#include"FuncInFuture.cpp"
#include"class_promise.cpp"
#endif



