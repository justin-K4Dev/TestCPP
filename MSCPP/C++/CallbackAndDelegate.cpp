#include "stdafx.h"

namespace CallbackAndDelegate
{

	namespace CallbackStaticFunctionStyle
	{
		// static function approach

		class CDPlayer
		{
		public:
			void play() { std::cout << "play cd !!!" << std::endl; }
			void stop() { std::cout << "stop cd !!!" << std::endl; }

			static void play_static(void* obj) { static_cast<CDPlayer*>(obj)->play(); }
			static void stop_static(void* obj) { static_cast<CDPlayer*>(obj)->stop(); }
		};

		class Button
		{
		public:
			typedef void(*Callback)(void *);

			Button(Callback callback) : m_callback(callback) {}

			void click(void * obj)
			{
				(*m_callback)(obj);
			}

		private:
			Callback m_callback;
		};

		void Test()
		{
			CDPlayer cd;

			Button playButton(&CDPlayer::play_static);
			Button stopButton(&CDPlayer::stop_static);

			playButton.click(&cd);
			stopButton.click(&cd);

			system("pause");

			/*
			output:
				play cd !!!
				stop cd !!!
			*/
		}
	}

	namespace CallbackInterfaceStyle
	{
		// interface approach

		class AbstractObject
		{
		};

		class CDPlayer : public AbstractObject
		{
		public:
			void play() { std::cout << "play cd !!!" << std::endl; }
			void stop() { std::cout << "stop cd !!!" << std::endl; }
		};

		class Button
		{
		public:
			typedef void (AbstractObject::*Callback)();

			Button(Callback callback, AbstractObject* obj)
				: m_callback(callback), m_obj(obj)
			{
			}

			void click()
			{
				(m_obj->*m_callback)();
			}

		private:
			Callback m_callback;
			AbstractObject* m_obj;
		};

		void Test()
		{
			CDPlayer cd;

			Button cd_play_button(static_cast<Button::Callback>(&CDPlayer::play), &cd);
			Button cd_stop_button(static_cast<Button::Callback>(&CDPlayer::stop), &cd);

			cd_play_button.click(); // invoke cd.play();
			cd_stop_button.click(); // invoke cd.stop();

			system("pause");

			/*
			output:
				play cd !!!
				stop cd !!!
			*/
		}
	}

	namespace CallbackTemplateStyle
	{
		// Template approach

		class AbstractInterface
		{
		public:
			virtual void click() = 0;
		};

		class CDPlayer
		{
		public:
			void play() { std::cout << "play cd !!!" << std::endl; }
			void stop() { std::cout << "stop cd !!!" << std::endl; }
		};

		template<class T>
		class Button : public AbstractInterface
		{
		public:
			typedef void (T::*Callback)();

			Button(Callback callback, T* obj)
			: m_callback(callback), m_obj(obj)
			{ }

			void click()
			{
				(m_obj->*m_callback)();
			}

		private:
			Callback m_callback;
			T* m_obj;
		};

		void Test()
		{
			CDPlayer cd;

			Button<CDPlayer> cd_play_button(&CDPlayer::play, &cd);
			Button<CDPlayer> cd_stop_button(&CDPlayer::stop, &cd);

			cd_play_button.click();	// invoke cd.play();
			cd_stop_button.click();	// invoke cd.stop();

			system("pause");

			/*
			output:
				play cd !!!
				stop cd !!!
			*/
		}
	}

	namespace CallbackCalleeMixIn
	{
		class Button
		{
		public:
			class Notifiable
			{
			public:
				virtual void notify() = 0;
			};

			Button(Notifiable* callee) : m_callee(callee)
			{
			}

			void click()
			{
				m_callee->notify();
			}

		private:
			Notifiable* m_callee;
		};

		class CDPlayer
		{
		public:
			void play() { std::cout << "play cd !!!" << std::endl; }
			void stop() { std::cout << "stop cd !!!" << std::endl; }
		};

		class MyCDPlayer : public CDPlayer, public Button::Notifiable
		{
		public:
			void notify()
			{
				this->play();
			}
		};

		void Test()
		{
			MyCDPlayer cd;

			Button cd_play_button(&cd);

			cd_play_button.click();	// invoke cd.play();

			system("pause");

			/*
			output:
				play cd !!!
			*/
		}
	}


	namespace DelegateAbstractInterfaceStyle
	{
		template <typename Return, typename Param0>
		class Callback
		{
		public:
			virtual Return invoke(Param0 param0) = 0;
		};

		template <typename Return, typename Param0>
		class StaticFunctionCallback : public Callback<Return, Param0>
		{
		private:
			Return(*m_func)(Param0);

		public:
			StaticFunctionCallback(Return(*func)(Param0))
				: m_func(func)
			{}

			virtual Return invoke(Param0 param0)
			{
				return (*m_func)(param0);
			}
		};

		template <typename Return, typename Param0, typename T, typename Method>
		class MethodCallback : public Callback<Return, Param0>
		{
		private:
			void* m_object;
			Method m_method;

		public:
			MethodCallback(void *object, Method method)
				: m_object(object)
				, m_method(method)
			{}

			virtual Return invoke(Param0 param0)
			{
				T* obj = static_cast<T*>(m_object);
				return (obj->*m_method)(param0);
			}
		};

		template <typename Return, typename Param0>
		class Delegate
		{
		private:
			Callback<Return, Param0>* m_callback;

		public:
			Delegate(Return(*func)(Param0))
				: m_callback(new StaticFunctionCallback<Return, Param0>(func))
			{}

			template <typename T, typename Method>
			Delegate(T* object, Method method)
				: m_callback(new MethodCallback<Return, Param0, T, Method>(object, method))
			{}

			~Delegate(void) { delete m_callback; }

			Return operator()(Param0 param0)
			{
				return m_callback->invoke(param0);
			}
		};

		class A
		{
		public:
			virtual int foo(int p)
			{
				std::cout << "A::foo(" << p << ")" << std::endl;
				return p;
			}
		};

		class B : public A
		{
		public:
			virtual int foo(int p)
			{
				std::cout << "B::foo(" << p << ")" << std::endl;
				return p;
			}
		};

		class C
		{
		};

		class D : public C, public A
		{
		public:
			virtual int foo(int p)
			{
				std::cout << "D::foo(" << p << ")" << std::endl;
				return p;
			}
		};

		int foo(int x)
		{
			std::cout << "foo(" << x << ")" << std::endl;
			return x;
		}

		void Test()
		{
			A *a = new A();
			A *b = new B();
			A *d = new D();

			//static function
			Delegate<int, int> d1(foo);

			//member function
			Delegate<int, int> d2(a, &A::foo);

			//member function + subclass instance
			Delegate<int, int> d3(b, &A::foo);

			//member function + subclass instance + multiple inheritance
			Delegate<int, int> d4(d, &A::foo);

			d1(100); //"foo(100)"
			d2(200); //"A::foo(200)"
			d3(300); //"B::foo(300)"
			d4(400); //"D::foo(400)"

			system("pause");

			/*
			output:
				foo(100)
				A::foo(200)
				B::foo(300)
				D::foo(400)
			*/
		}
	}

	namespace DelegateCSharpStyle
	{
		class Delegate 
		{
		protected:
			typedef void(*SIGNAL)(int);

			SIGNAL signal;
			std::vector<Delegate*> delegate_list;

		public:
			Delegate(SIGNAL s) : signal(s) {}
			~Delegate() 
			{
				for( auto itPos = delegate_list.begin();
					 itPos != delegate_list.end();
					 ++itPos ) {
					delete *itPos;
				}
			}

			void Add(Delegate* p)
			{ 
				delegate_list.push_back(p);
			}
			
			void Remove(Delegate* p)
			{
				auto itPos = std::find_if( delegate_list.begin(),
					                       delegate_list.end(),
										   [&p](Delegate* pDelegate ) -> bool {
					return pDelegate == p;
				});
				if (delegate_list.end() != itPos) {
					delete *itPos;
					delegate_list.erase(itPos);
				}
			}

			void Invoke(int param)
			{
				if (signal != 0)
					signal(param);

				for (int i = 0; i < delegate_list.size(); ++i) {
					delegate_list[i]->Invoke(param);
				}
			}

			Delegate* operator += ( Delegate* p )
			{
				this->Add(p);
				return this;
			}

			Delegate* operator -= (Delegate* p)
			{
				this->Remove(p);
				return this;
			}

			void operator()(int param)
			{
				this->Invoke(param);
			}
		};

		class Car
		{
		public:
			Delegate hanlder;

		public:
			Car() : hanlder(0) { }

			void SpeedUp(int speed)
			{
				std::cout << "Speed : " << speed << std::endl;

				if ( speed > 100 ) {
					hanlder(speed);
				}
			}
		};

		void Police(int speed)
		{
			std::cout << "Police Start !!! - Speed:" << speed << std::endl;
		}

		void Test()
		{
			Car c;

			Delegate *pDelegate = new Delegate(Police);

			c.hanlder += pDelegate;
			c.hanlder += new Delegate(Police);

			c.SpeedUp(100);
			c.SpeedUp(200);

			c.hanlder -= pDelegate;

			c.SpeedUp(500);

			system("pause");

			/*
			output:
				Speed : 100
				Speed : 200
				Police Start !!! - Speed:200
				Police Start !!! - Speed:200
				Speed : 500
				Police Start !!! - Speed:500
			*/
		}
	}


	void Test()
	{
		//CallbackStaticFunctionStyle::Test();

		//CallbackInterfaceStyle::Test();

		//CallbackTemplateStyle::Test();

		//CallbackCalleeMixIn::Test();

		//DelegateAbstractInterfaceStyle::Test();
		
		//DelegateCSharpStyle::Test();
	}
}