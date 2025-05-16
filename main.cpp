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

void * alloMem(myObj*head, size_t mSize)// allocates space in memory of specified size
{                           // can manage memory using pointer math
    myObj* cur = head;

    while(cur != nullptr)
    {
        if(cur->free && cur->size >= mSize)// if the current cell is free and the cuurrent size of our memory is greater then the space we want to allocate
        {
            cur->free = 0;// cell is no longer free
            return (void*)(cur + 1);// Pointer just after the header
        }
        cur = cur->next;//out of memory
    }
    return nullptr;
}

void * freeMem(void * p)
{
    if(!p) return nullptr;//base case
    myObj* obj = ((myObj*) p) - 1;//pointer to location in memory just before the heade, basically removing one cell in our linked list
    obj->free = 1;// set the cell back to free/Available Status
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
    myObj * newObj = init_allocator(newObj);//initialize memory node
    alloMem(newObj,sizeof(newObj));// allocate corresponting w/ conditional checks
    cout << "success" << endl;
    freeMem(newObj);// free memory(opposite of alloMem() w/ less checks)
    cout << "successfully freed" << endl;

    return 0;
}