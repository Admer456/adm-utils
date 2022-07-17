
#pragma once

namespace adm
{
	// Wrapper for singleton classes
	// Requires a default constructor
	template<typename Class>
	class Singleton
	{
	public:
		static Class& GetInstance()
		{
			static Class instance;
			return instance;
		}

		static Class* GetInstancePtr()
		{
			return &GetInstance();
		}
	};
}
