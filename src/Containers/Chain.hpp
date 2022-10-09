// SPDX-FileCopyrightText: 2021-2022 Admer Šuko
// SPDX-License-Identifier: MIT

#pragma once

namespace adm
{
	// A linked list template for classes that will add their own instances to the list. 
	// You typically would not use this like a standard linked list, hence the name chain.
	// T must inherit from Chain
	template<typename T>
	class Chain
	{
	public:
		Chain()
		{
			prev = head;
			head = static_cast<T*>(this);
		}

		// TODO: make this a double linked list,
		// so we can easily remove this from the chain
		// in the destructor

		static T* GetHead()
		{
			return head;
		}

		using ProcessFn = void( T* object );
		static void DoAll( ProcessFn* function )
		{
			T* current = head;
			while ( current )
			{
				function( current );
				current = current->prev;
			}
		}

	protected:
		inline static T* head = nullptr;
		T* prev = nullptr;
	};
}
