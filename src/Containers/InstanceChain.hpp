
#pragma once

namespace adm
{
    // T must inherit from InstanceChain
    template<typename T>
    class InstanceChain
    {
    public:
        InstanceChain()
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
