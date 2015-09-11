# pragma once

namespace View
{
	class Keyboard abstract : private boost::noncopyable
	{
		static const char factory_ =
# if defined _CONSOLE || _LIB
			'C'
# elif defined _WINDOWS
			'W'
# else
			"WRONG PROJECT";
# endif
		;
		static const Keyboard& GetInstance();
	public:
		static void Init()
		{
			GetInstance().Init_impl();
		}
		static void Update()
		{
			GetInstance().Update_impl();
		}
		virtual ~Keyboard() = 0 {}
	protected:
		Keyboard() = default;
	private:
		virtual void Init_impl() const = 0
		{
			assert(!"PURE VIRTUAL FUNCTION IKeyboard::Init_impl() SHOULD NEVER BE CALLED");
		}
		virtual void Update_impl() const = 0
		{
			assert(!"PURE VIRTUAL FUNCTION IKeyboard::Update_impl() SHOULD NEVER BE CALLED");
		}
	};
}

# if defined _CONSOLE || _LIB
#	include "KeyboardConsole.h"
# elif defined _WINDOWS
#	include "KeyboardWin32.h"
# else
#	"WRONG PROJECT";
# endif