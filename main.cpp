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


static int count = 0;//Number of total node entries               
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
void * alloMem(myObj*head, size_t mSize, int &count)
{                           
    myObj* cur = head;
    uintptr_t cur_addr = (uintptr_t)cur + sizeof(void*), aligned_addr;
    void** aligned_ptr = nullptr;

    while(cur != nullptr)
    {
        if(cur->free && cur->size >= mSize)// first fit: first available block that is big enough
        {
            aligned_addr = (cur_addr + (mSize-1)) & ~(mSize-1);// aligns address to our desired boundary(8 & 16 bits)
            aligned_ptr = (void**)aligned_addr;
            aligned_ptr[-1] = cur;
            cur->free = 0;
            count++;
            return (void*)(cur + 1);
        }

        cur = cur->next;
    }
    return (void*)aligned_ptr;//returns void pointer to the block of memory
}

void * sortBlocks(myObj*head)
{
    if(!head) return nullptr;
    myObj* cur = head;
    myObj* dummy =(myObj*)malloc(sizeof(myObj)); // temporary dummy node
    dummy->next = head;
    myObj* prev = dummy;
    myObj*nxt = cur->next;

    while(nxt)
    {
        if(cur->size > nxt->size)
        {
            cur->next = nxt->next;
            nxt->next = cur;
            cur = nxt;
            prev->next = cur;
        }
        prev = cur;
        cur = cur->next;
        nxt = cur->next;
    }
    head = dummy->next;
    delete dummy;
   
    
    return (void*) head;
}

/* freeMem()
Free Memory 
by going back to the previously allocated block and setting it to free

*implementing Segmented free list using best-fit, first-fit, or next-fit strategy
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
void ** addToMem(myObj**allMemory, myObj*addedNode)
{

}

int main()
{
    // memory allocation
    int * count = (int*)calloc(0,sizeof(int));
    myObj * newObj  = (myObj*) alloMem(newObj, sizeof(myObj), *count);

    //myObj ** allMemory = (myObj**) malloc(sizeof(myObj* )* count);// a double pointer like this would allow you to index into any spot in the given linked list whenever needed
    newObj = init_allocator(newObj);
    //newObj =(myObj*)sortBlocks;
    cout << "success" <<" - "<< *count << endl;
    freeMem(newObj);
    free(count);
    cout << "successfully freed" << endl;

    return 0;
}