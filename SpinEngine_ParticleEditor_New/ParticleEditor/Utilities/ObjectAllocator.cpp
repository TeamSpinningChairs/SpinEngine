/******************************************************************************/
/*!
\file ObjectAllocator.cpp
\author Esteban Maldonado Cabán
\par esteban.maldonado
\par cs280
\par Assignment#1
\date 09/18/2014
\brief
  Memory Manager - An object allocator that gives the client faster access to memory
  instead on depending on just using new and elete as always

  An additional note: I swear that I'm not bluffing, when I checked for mem leaks with
  Dr. Memory for the first time when done, 0 leaks were found. That was nice.
\par Functions Include:
  public:
  - typedef void (*DUMPCALLBACK)(const void *, size_t);
	- typedef void (*VALIDATECALLBACK)(const void *, size_t);
  - ObjectAllocator(size_t ObjectSize, const OAConfig& config) NOEXCEPT(false);
  - ~ObjectAllocator() NOEXCEPT(true);
  - void *Allocate(const char *label = 0) NOEXCEPT(false);
  - void Free(void *Object) NOEXCEPT(false);
  - unsigned ValidatePages(VALIDATECALLBACK fn) const;
  - unsigned FreeEmptyPages(void);
  - static bool ImplementedExtraCredit(void);
  - void SetDebugState(bool State);       
  - const void *GetFreeList(void) const;
  - const void *GetPageList(void) const;
  - OAConfig GetConfig(void) const;
  - OAStats GetStats(void) const;

  private:
  - void allocate_new_page();       
  - void put_on_freelist(void *Object);
  - unsigned int get_page_size();
  - void release_memory();
  - bool is_on_freelist(void *Object);
  - bool is_out_of_bounds(void *Object);
  - void* awesome_cool_MyOAalloc(const char *label = 0);
  - void* lame_old_newdelAlloc();
  - ObjectAllocator(const ObjectAllocator &oa);
   - ObjectAllocator &operator=(const ObjectAllocator &oa);
*/
/******************************************************************************/
#include"ObjectAllocator.h"

/******************************************************************************/
/*!
\brief
  ObjectAllocator(size_t ObjectSize, const OAConfig& config) - 
  Creates the ObjectManager per the specified values
  Throws an exception if the construction fails. (Memory allocation problem)
\param ObjectSize
  The size of the object type
\param config
  The configuration for the Object Allocator
*/
/******************************************************************************/

ObjectAllocator::ObjectAllocator(size_t ObjectSize, const OAConfig& config):
                                 OAConfig_(config), PageList_(NULL), FreeList_(NULL),
                                 DebugState_(config.DebugOn_),UseOA(!config.UseCPPMemManager_)
{
  // Calculate the entire byte count for each element on a 
  // page, including size, headers,etc.
  OAStats_.ObjectSize_ = ObjectSize;
  OAStats_.FreeObjects_ = OAConfig_.ObjectsPerPage_;
  OAStats_.PageSize_ = get_page_size();

  if(!config.UseCPPMemManager_)
  {
    allocate_new_page();
  }
}

/******************************************************************************/
/*!
\brief
  ~ObjectAllocator() - Class destructor
*/
/******************************************************************************/
// Destroys the ObjectManager (never throws)
ObjectAllocator::~ObjectAllocator() NOEXCEPT(true)
{
  if ( UseOA )
  {
    release_memory();
  }
}

/******************************************************************************/
/*!
\brief
 Returns a pointer to newly allocated memory(CPPMemManager or OA)
 Throws an exception if the object can't be allocated. (Memory allocation problem)
\param label
  A string of characters to label an external header if used
\exception std::bad_alloc is out of memory
\return 
  returns a pointer to the next free address from the free list
*/
/******************************************************************************/
void* ObjectAllocator::Allocate(const char *label) 
{
  if ( UseOA)
  {
    return awesome_cool_MyOAalloc(label);
  }

  return lame_old_newdelAlloc();
}

/******************************************************************************/
/*!
\brief
 Returns an object to the free list for the client (simulates delete)
 Throws an exception if the the object can't be freed. (Invalid object)
\param Object
  The pointer that was used by the client
\exception OA_EXCEPTON::E_MULTIPLE_FREE if pointer is freed twice
*/
/******************************************************************************/
void ObjectAllocator::Free(void *Object)
{
  if ( UseOA)
  {
    put_on_freelist(Object);
    return;
  }
  OAStats_.Deallocations_++;
  OAStats_.FreeObjects_++;
  OAStats_.ObjectsInUse_--;
  if(Object)
  {
    delete [] reinterpret_cast<char*>(Object);
  }
}

/******************************************************************************/
/*!
\brief
 Calls the callback fn for each block still in use
\param fn
  The function pointer to the callback funcion
\return 
  returns how many blocks are still in use
*/
/******************************************************************************/
unsigned ObjectAllocator::DumpMemoryInUse(DUMPCALLBACK fn) const
{
  GenericObject* tempPageList = PageList_;
  unsigned used_count = 0;
  while (tempPageList)
  {
    char* tempCharPtr = reinterpret_cast<char*>(tempPageList) + sizeof(void*);

    for(unsigned i = 0; i < OAConfig_.ObjectsPerPage_; i++)
    {
      if(OAConfig_.HBlockInfo_.type_ == OAConfig::hbNone)
      {
        //If no header then compare Object with each object on the free list
        if ( !is_on_freelist(tempCharPtr) )
        {
          fn(tempCharPtr, OAStats_.ObjectSize_);
          used_count++;
        }
      }
      else if(OAConfig_.HBlockInfo_.type_ == OAConfig::hbBasic ||
        OAConfig_.HBlockInfo_.type_ == OAConfig::hbExtended)
      {
        if ( *(tempCharPtr + sizeof(int) )  & 1 )
        {
          fn(tempCharPtr + OAConfig_.HBlockInfo_.size_ + OAConfig_.PadBytes_,
            OAStats_.ObjectSize_);

          used_count++;
        }
      }
      
      else
      {
        MemBlockInfo* tempMemBlkPtr = reinterpret_cast<MemBlockInfo*>(
          reinterpret_cast<GenericObject*>(tempCharPtr)->Next);
        if(tempMemBlkPtr)
        {
            fn(tempCharPtr + OAConfig_.HBlockInfo_.size_ + OAConfig_.PadBytes_,
            OAStats_.ObjectSize_);
            used_count++;
        }
      }

      tempCharPtr = tempCharPtr  + (OAConfig_.HBlockInfo_.size_ + OAConfig_.PadBytes_ 
        + OAStats_.ObjectSize_ +  OAConfig_.PadBytes_);
    }
    

    tempPageList = tempPageList->Next;
  }

  return used_count;
}

/******************************************************************************/
/*!
\brief
 Calls the callback fn for each block that is potentially corrupted
\param fn
  The function pointer to the callback funcion
\return 
  returns how many blocks are corrupted
*/
/******************************************************************************/
unsigned ObjectAllocator::ValidatePages(VALIDATECALLBACK fn) const
{
  GenericObject* tempPageList = PageList_;
  unsigned corrupt_count = 0;
  while (tempPageList)
  {
    char* tempCharPtr = reinterpret_cast<char*>(tempPageList) + sizeof(void*);
    
    for(unsigned i = 0; i < OAConfig_.ObjectsPerPage_; i++)
    {
      tempCharPtr = tempCharPtr + OAConfig_.HBlockInfo_.size_;

      char* tempCheck = new char[OAConfig_.PadBytes_ + 1];
      std::memset(tempCheck, PAD_PATTERN, OAConfig_.PadBytes_);

      if( std::memcmp(tempCheck, tempCharPtr, OAConfig_.PadBytes_ ) ||
        std::memcmp(tempCheck, tempCharPtr + OAConfig_.PadBytes_ + OAStats_.ObjectSize_,
        OAConfig_.PadBytes_ ) )
      {
        fn(tempCharPtr + OAConfig_.PadBytes_, OAStats_.ObjectSize_);
        corrupt_count++;
      }

      delete [] tempCheck;
      tempCharPtr = tempCharPtr + OAConfig_.PadBytes_ + OAStats_.ObjectSize_
        +  OAConfig_.PadBytes_;
    }

    tempPageList = tempPageList->Next;
  }
  return corrupt_count;
}

// Frees all empty pages (extra credit)
unsigned ObjectAllocator::FreeEmptyPages(void)
{
  return 0;
}

// Returns true if FreeEmptyPages and alignments are implemented
bool ObjectAllocator::ImplementedExtraCredit(void)
{
  return false;
}

/******************************************************************************/
/*!
\brief
 Testing/Debugging/Statistic methods
\param State
  true=enable, false=disable 
*/
/******************************************************************************/
void ObjectAllocator::SetDebugState(bool State) 
{
  DebugState_ = State;
}

/******************************************************************************/
/*!
\brief
  returns a pointer to the internal free list
\return 
  returns a pointer to the head of the free list
*/
/******************************************************************************/
const void* ObjectAllocator::GetFreeList(void) const  
{
  return FreeList_;
}

/******************************************************************************/
/*!
\brief
  returns a pointer to the internal page list
\return 
  returns a pointer to the head of the page list
*/
/******************************************************************************/
const void* ObjectAllocator::GetPageList(void) const 
{
  return PageList_;
}

/******************************************************************************/
/*!
\brief
  gives a copy of the current configuration
\return 
  returns the configuration parameters
*/
/******************************************************************************/
OAConfig ObjectAllocator::GetConfig(void) const
{
  return OAConfig_;
}

/******************************************************************************/
/*!
\brief
  gives a copy of the current stats
\return 
  returns the statistics for the allocator
*/
/******************************************************************************/
OAStats ObjectAllocator::GetStats(void) const
{
  return OAStats_;
}

/******************************************************************************/
/*!
\brief
  allocates another page of objects
\return 
  returns the statistics for the allocator
*/
/******************************************************************************/
void ObjectAllocator::allocate_new_page()
{
  try
  {
    //Allocate a new array of characters(or page)
    char* tempNewPage_ = new char[OAStats_.PageSize_];

    //Check if memory successfully allocated
    if (DebugState_)
    {
      memset(tempNewPage_, UNALLOCATED_PATTERN, OAStats_.PageSize_);
    }

    GenericObject* TempPageList_ = reinterpret_cast<GenericObject*>(tempNewPage_);
    TempPageList_->Next = PageList_;
    PageList_ = TempPageList_;

    tempNewPage_ =  tempNewPage_ + sizeof(void*);
    //TempPageList_ = reinterpret_cast<GenericObject*>(tempNewPage_);

    //PREPARE RIGHT SIDE OF PAGE
    for(unsigned i = 0; i < OAConfig_.ObjectsPerPage_; i++)
    {
      std::memset(tempNewPage_, 0, OAConfig_.HBlockInfo_.size_);

      //Go to the first pad bytes and sign them
      tempNewPage_ = tempNewPage_ + OAConfig_.HBlockInfo_.size_;
      if (DebugState_)
      {
        std::memset(tempNewPage_, PAD_PATTERN, OAConfig_.PadBytes_);
      }

      //go to the next object
      tempNewPage_ = tempNewPage_ + OAConfig_.PadBytes_;
      TempPageList_ = reinterpret_cast<GenericObject*>(tempNewPage_);

      //Add address to free list
      TempPageList_->Next = FreeList_;
      FreeList_ = TempPageList_;

      //Go to the pad bytes and sign them
      tempNewPage_ = tempNewPage_ + OAStats_.ObjectSize_;
      if (DebugState_)
      {
        std::memset(tempNewPage_, PAD_PATTERN, OAConfig_.PadBytes_);
      }
      //Sign the last pad bytes
      tempNewPage_ = tempNewPage_ + OAConfig_.PadBytes_;
    }

    OAStats_.FreeObjects_ = OAConfig_.ObjectsPerPage_;
    OAStats_.PagesInUse_++;
  }
  catch (std::bad_alloc&)
  {
    throw OAException(OAException::E_NO_MEMORY, 
      "allocate_new_page: No system memory available.");
  }
}

/******************************************************************************/
/*!
\brief
 calculates the size of the entire page 
\return 
  returns the exact byte count
*/
/******************************************************************************/
unsigned int ObjectAllocator::get_page_size()
{
  unsigned calc_page_size = static_cast<unsigned>( sizeof(void*) );
  unsigned each_object =  static_cast<unsigned>(OAConfig_.HBlockInfo_.size_) + 
    (OAConfig_.PadBytes_ * 2) + static_cast<unsigned>(OAStats_.ObjectSize_) ;

  calc_page_size += (OAConfig_.ObjectsPerPage_ * each_object);

  return calc_page_size;
}

/******************************************************************************/
/*!
\brief
 Checks if an object is on the free list
\param Object
  A pointer to allocated memory
\return 
  returns true or false
*/
/******************************************************************************/
bool ObjectAllocator::is_on_freelist(void *Object) const
{
  GenericObject* tempFreeList = FreeList_;
  while(tempFreeList)
  {
    if(Object == tempFreeList->Next)
    {
      return true;
    }
    tempFreeList = tempFreeList->Next;
  }

  return false;
}

/******************************************************************************/
/*!
\brief
 puts Object onto the free list
\param Object
  A pointer to allocated memory
\exception OAException::E_CORRUPTED_BLOCK if Object is out of bounds
\exception OA_EXCEPTION::E_NO_PAGES is out of pages
\return 
  returns a pointer to the next free address from the free list
*/
/******************************************************************************/
void ObjectAllocator::put_on_freelist(void *Object)
{
  if(Object)
  {
    char* temp = reinterpret_cast<char*>(Object);
    //Check if Object is already freed
    if(DebugState_)
    {
      if(OAConfig_.HBlockInfo_.type_ == OAConfig::hbNone)
      {
        //If no header then compare Object with each object on the free list
        if (is_on_freelist(Object))
        {
          throw( OAException(OAException::E_MULTIPLE_FREE,"") );
        }
      }
      else if (OAConfig_.HBlockInfo_.type_ == OAConfig::hbBasic ||
        OAConfig_.HBlockInfo_.type_ == OAConfig::hbExtended)
      {
        temp = temp + sizeof(int) - (OAConfig_.HBlockInfo_.size_ + OAConfig_.PadBytes_);
        
        //If the bit that indicates if in use or not is 0
        if (!(*temp & 1))
        {
          throw( OAException(OAException::E_MULTIPLE_FREE,"") );
        }
        *temp = *temp ^ 1;
      }

      //lastly, if it's external then check is address is 0
      else
      {
        temp = temp - (OAConfig_.HBlockInfo_.size_ + OAConfig_.PadBytes_);
        //If the address on the header block is 0, then it's already free
        if (!reinterpret_cast<GenericObject*>(temp)->Next )
        {
          throw( OAException(OAException::E_MULTIPLE_FREE,"") );
        }
      }
    }
    //end check if Object is already freed;

    if (  is_out_of_bounds(Object) )
    {
      throw OAException( OAException::E_CORRUPTED_BLOCK, "" );
    }

    GenericObject* tempGenObj = reinterpret_cast<GenericObject*>(Object);
    temp = reinterpret_cast<char*>(Object);
    //If debug is on then sign the bytes as freed
    if (DebugState_)
    {
      memset(Object, ObjectAllocator::FREED_PATTERN, OAStats_.ObjectSize_);
    }
    tempGenObj->Next = FreeList_;
    FreeList_ = tempGenObj;

    //Do Header adjustments according to the proper type
    if (OAConfig_.HBlockInfo_.type_ == OAConfig::hbBasic||
      OAConfig_.HBlockInfo_.type_ == OAConfig::hbExtended)
    {
      temp = temp - (OAConfig_.HBlockInfo_.size_ - sizeof(int) +  OAConfig_.PadBytes_);
      if( *temp & 1)
      {
        throw(OAException(OAException::E_MULTIPLE_FREE, "Free List: Block already free."));
      }
      *temp = *temp & 1;
      //if(OAConfig_.HBlockInfo_.type_ == OAConfig::hbExtended)
      //{
      //  temp = temp + sizeof(short);
      //}
    }

    else if (OAConfig_.HBlockInfo_.type_ == OAConfig::hbExternal)
    {
      temp = temp - (OAConfig_.HBlockInfo_.size_ +  OAConfig_.PadBytes_);
      //reinterpret the temp pointer to access the mem block info data
      MemBlockInfo* tempMemBlkPtr = reinterpret_cast<MemBlockInfo*>(
        reinterpret_cast<GenericObject*>(temp)->Next);
      tempMemBlkPtr->in_use = false;
      if(tempMemBlkPtr->label)
      {
        delete [] tempMemBlkPtr->label;
      }
      tempMemBlkPtr->label = NULL;
      delete tempMemBlkPtr;
      std::memset(temp, 0, OAConfig_.HBlockInfo_.size_);
    }

    OAStats_.Deallocations_++;
    OAStats_.FreeObjects_++;
    OAStats_.ObjectsInUse_--;
  }
}

/******************************************************************************/
/*!
\brief
  Releases all pages and if external headers used, all of the MemblockInfo
  objects and their respective labels.
*/
/******************************************************************************/
void ObjectAllocator::release_memory()
{
  GenericObject *tempPageList = PageList_, *tempPageList2 = PageList_;
  while (tempPageList)
  {
    //If header block is external then delete the memory
    if( OAConfig_.HBlockInfo_.type_ == OAConfig::hbExternal)
    {
      char*  tempCharPtr = reinterpret_cast<char*>(tempPageList) + sizeof(void*); 
      for(unsigned i = 0; i < OAConfig_.ObjectsPerPage_; i++)
      {
        MemBlockInfo* tempMemBlkPtr = reinterpret_cast<MemBlockInfo*>(
          reinterpret_cast<GenericObject*>(tempCharPtr)->Next );
        if(tempMemBlkPtr)
        {
          if(tempMemBlkPtr->label)
          {
            delete [] tempMemBlkPtr->label;
          }
          delete tempMemBlkPtr;
        }
        tempCharPtr = tempCharPtr + OAConfig_.HBlockInfo_.size_ + 
          OAConfig_.PadBytes_ + OAStats_.ObjectSize_ +  OAConfig_.PadBytes_;
      }
    }
    tempPageList = tempPageList->Next;
    delete[] reinterpret_cast<char *>(tempPageList2);
    tempPageList2 = tempPageList;
  }
}
/******************************************************************************/
/*!
\brief
  Determines if a given pointer to memory is out of bounds.
\param Object
  A pointer to an object
\return 
  returns true or false
*/
/******************************************************************************/
bool ObjectAllocator::is_out_of_bounds(void *Object) const
{
  if(OAConfig_.PadBytes_)
  {
    char* tempCharPtr = reinterpret_cast<char*>(Object) - (OAConfig_.PadBytes_);

    char* tempCheck = new char[OAConfig_.PadBytes_ + 1];
    std::memset(tempCheck, PAD_PATTERN, OAConfig_.PadBytes_);

    bool result =  std::memcmp(tempCheck, tempCharPtr, OAConfig_.PadBytes_ ) ||
      std::memcmp(tempCheck, tempCharPtr + OAConfig_.PadBytes_ + OAStats_.ObjectSize_,
      OAConfig_.PadBytes_ );

    delete [] tempCheck;
    return result;
  }

  return false;
}

/******************************************************************************/
/*!
\brief
 Take an object from the free list and give it to the client (simulates new)
 Throws an exception if the object can't be allocated. (Memory allocation problem)
\param label
  A string of characters to label an external header if used
\exception std::bad_alloc is out of memory
\exception OA_EXCEPTION::E_NO_PAGES is out of pages
\return 
  returns a pointer to the next free address from the free list
*/
/******************************************************************************/
void* ObjectAllocator::awesome_cool_MyOAalloc(const char *label)
{
  GenericObject* newBlockAddr = NULL;
  //Continue is there are open spots available
  if (OAStats_.FreeObjects_ || OAStats_.PagesInUse_ < OAConfig_.MaxPages_ 
    || OAConfig_.MaxPages_ == 0 /*|| !DebugState_ */)
  {
    //If freelist is NULL, then create a new page
    if(!FreeList_)
    {
      allocate_new_page();
    }

    newBlockAddr = FreeList_;
    FreeList_ = FreeList_->Next;

    if ( OAConfig_.HBlockInfo_.type_ == OAConfig::hbBasic ||
      OAConfig_.HBlockInfo_.type_ == OAConfig::hbExtended )
    {
      int* tempIntPtr = NULL;
      //Get an int pointer to the beginning of the header
      //Increase its allocation number
      tempIntPtr = reinterpret_cast<int*>( reinterpret_cast<char*>(newBlockAddr) - 
        (OAConfig_.PadBytes_  + OAConfig_.HBlockInfo_.size_) );

      (*tempIntPtr)++;

      //Move the pointer to the extrta byte
      char* tempCharPtr = reinterpret_cast<char*>(tempIntPtr) + sizeof(int);
      //mark the last bit of the extra byte to indicate that block is allocated
      *tempCharPtr = *tempCharPtr | 1;

      if(OAConfig_.HBlockInfo_.type_ == OAConfig::hbExtended)
      {
        short* tempShortPtr = reinterpret_cast<short*>(tempCharPtr + sizeof(char));
        *tempShortPtr = static_cast<short>( (*tempShortPtr) + 1);
      }
    }
    else if ( OAConfig_.HBlockInfo_.type_ == OAConfig::hbExternal )
    {
      char* tempPtr = reinterpret_cast<char*>(newBlockAddr) - 
        (OAConfig_.PadBytes_ +  OAConfig_.HBlockInfo_.size_);
      MemBlockInfo* tempMemBlkPtr = new MemBlockInfo();
      tempMemBlkPtr->alloc_num = OAStats_.Allocations_ + 1;
      tempMemBlkPtr->in_use = true;
      tempMemBlkPtr->label = NULL;

      if(label)
      {
        tempMemBlkPtr->label = new char[strlen(label) + 1];
        std::strcpy(tempMemBlkPtr->label, label);
      }
      reinterpret_cast<GenericObject*>(tempPtr)->Next = 
        reinterpret_cast<GenericObject*>(tempMemBlkPtr);
    }

    OAStats_.FreeObjects_--;
    OAStats_.ObjectsInUse_++;

    if(OAStats_.ObjectsInUse_ > OAStats_.MostObjects_)
    {
      OAStats_.MostObjects_++;
    }

    OAStats_.Allocations_++;

    if(DebugState_)
    {
      std::memset(newBlockAddr, ALLOCATED_PATTERN, OAStats_.ObjectSize_);
    }

  }

  else
  {
    //Throw exception that no pages are available
    throw( OAException(OAException::E_NO_PAGES," Allocate: No more pages available.") );
  }
  //return NULL if there are no blocks available.
  return newBlockAddr;
}

/******************************************************************************/
/*!
\brief
 Allocates memory without the Object Allocator's functinalities
\return 
  returns a pointer to an array of X bytes
*/
/******************************************************************************/
void* ObjectAllocator::lame_old_newdelAlloc()
{
  OAStats_.FreeObjects_--;
  OAStats_.ObjectsInUse_++;

  if(OAStats_.ObjectsInUse_ > OAStats_.MostObjects_)
  {
    OAStats_.MostObjects_++;
  }

  OAStats_.Allocations_++;
  return new char[OAStats_.ObjectSize_];
}
