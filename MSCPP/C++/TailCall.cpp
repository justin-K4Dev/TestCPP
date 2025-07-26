#include "stdafx.h"



namespace TailCall
{
	void tail_call_simple()
	{

	}


	// Multiplying by two

	long times_two_hardware(long value)
	{
		return value * 2;
	}

	long times_two_loop(long value)
	{
		long ret = 0;
		for (long i = 0; i < value; ++i)
		{
			ret += 2;
		}
		return ret;
	}

	long times_two_naive_recursive(long value)
	{
		if (value == 0)
		{
			return 0;
		}
		else
		{
			return 2 + times_two_naive_recursive(value - 1);
		}
	}


	// Tail call optimisation

	long times_two_recursive_impl(long total, long counter)
	{
		if (counter == 0)
		{
			return total;
		}
		else
		{
			return times_two_recursive_impl(total + 2, counter - 1);
		}
	}

	long times_two_recursive(long value)
	{
		return times_two_recursive_impl(0, value);
	}


	// Tail call optimised C++

	struct Answer;
	typedef Answer(*impl_fn_type)(long, long);

	struct FnPlusArgs
	{
		impl_fn_type fn_;
		long arg1_;
		long arg2_;

		FnPlusArgs(
			impl_fn_type fn,
			long arg1,
			long arg2
		);

		Answer operator()();
	};

	impl_fn_type null_fn = NULL;
	FnPlusArgs null_fn_plus_args(null_fn, 0, 0);

	struct Answer
	{
		bool finished_;
		FnPlusArgs tail_call_;
		long value_;

		Answer(bool finished, FnPlusArgs tail_call, long value);
	};


	FnPlusArgs::FnPlusArgs(
		impl_fn_type fn,
		long arg1,
		long arg2
	)
		: fn_(fn)
		, arg1_(arg1)
		, arg2_(arg2)
	{
	}

	Answer FnPlusArgs::operator()()
	{
		return fn_(arg1_, arg2_);
	}


	Answer::Answer(bool finished, FnPlusArgs tail_call, long value)
		: finished_(finished)
		, tail_call_(tail_call)
		, value_(value)
	{
	}


	// The Trampoline
	long trampoline(Answer answer)
	{
		while (!answer.finished_)
		{
			answer = answer.tail_call_();
		}
		return answer.value_;
	}

	Answer times_two_tail_call_impl(long acc, long i)
	{
		if (i == 0)
		{
			return Answer(true, null_fn_plus_args, acc);
		}
		else
		{
			return Answer(
				false,
				FnPlusArgs(
					times_two_tail_call_impl,
					acc + 2,
					i - 1
				),
				0
			);
		}
	}

	long times_two_tail_call(long n)
	{
		return trampoline( Answer( false
								 , FnPlusArgs( times_two_tail_call_impl, 0, n )
								 , 0) );
	}


	void tail_call()
	{
		times_two_recursive(100);

		times_two_tail_call(100);

		system("pause");
	}


	typedef  unsigned long long ULONG64;

	ULONG64 fibonachi_tail_x64(ULONG64 n, ULONG64 res, ULONG64 next)
	{
		if (n == 0) {
			return res;
		}

		return fibonachi_tail_x64(n - 1, next, res + next);
	}

	void fibonachi()
	{
		{
			ULONG64 a = 40;
			ULONG64 res = fibonachi_tail_x64(a, 0, 1);
		}

		{
			volatile bool a = true;
			ULONG64 res = 0;
			ULONG64(*ptr)(ULONG64 n, ULONG64 res, ULONG64 next) = NULL;
			if (a) {
				ptr = &fibonachi_tail_x64;
			}
			res = ptr(40, 0, 1);
		}

		system("pause");
	}


	void Test()
	{
		//tail_call_simple();

		//tail_call();

		//fibonachi();
	}

}