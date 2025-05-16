#include <iostream>
#include <cstdlib>   // for malloc, free
using namespace std;
/* Overview :
    created a fixed-size memory pool.
    Used a struct (myObj) as a header for blocks.
    Managed free/used blocks using a linked list of free blocks.
*/

#define POOL_SIZE 1024 * 1024  // 1 MB
char memory_pool[POOL_SIZE];//pre allocated block of memory(Array of Bytes)
                            // speeds up allocation(you don't call malloc() or free() every time) 
                            // specific use of char because a character is one byte

                    
typedef struct myObj//Block(of memory)
{
    // data 
    size_t size;
    int free;
    struct myObj * next;
}myObj;


/* alloMem()
Allocate Memory 
IF
the current cell is free and the cuurrent size of our memory is greater then the space we want to allocate
we then set the block of memory to 'In-use' and return a pointer to the address after this block
*/
void * alloMem(myObj*head, size_t mSize)
{                           
    myObj* cur = head;
    uintptr_t cur_addr = (uintptr_t)cur + sizeof(void*), aligned_addr;
    void** aligned_ptr = nullptr;

    while(cur != nullptr)
    {
        if(cur->free && cur->size >= mSize)
        {
            aligned_addr = (cur_addr + (mSize-1)) & ~(mSize-1);// aligns address to our desired boundary
            aligned_ptr = (void**)aligned_addr;
            aligned_ptr[-1] = cur;
            cur->free = 0;
            return (void*)(cur + 1);
        }
        cur = cur->next;//out of memory
    }
    return (void*)aligned_ptr;
}

/* freeMem()
Free Memory 
by going back to the previously allocated block and setting it to free
*/
void * freeMem(void * p)
{
    if(!p) return nullptr;//base case
    myObj* obj = ((myObj*) p) - 1;
    obj->free = 1;
}


myObj * init_allocator(myObj * newObj) // initializes everything in the new obj node
{
    newObj = reinterpret_cast<myObj*>(memory_pool);
    newObj->size = POOL_SIZE - sizeof(myObj);
    newObj->free = 1;
    newObj->next = nullptr;

    return newObj;
}

int main()
{
    myObj * newObj = init_allocator(newObj);
    alloMem(newObj,sizeof(newObj));
    cout << "success" << endl;
    freeMem(newObj);
    cout << "successfully freed" << endl;

    return 0;
}