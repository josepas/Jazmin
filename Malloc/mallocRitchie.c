#include <unistd.h>  // sbrk
#include <stdio.h>

#define NALLOC 1024  // Number of block sizes to allocate on call to sbrk
#ifdef NULL
#undef NULL
#endif
#define NULL 0


// long is chosen as an instance of the most restrictive alignment type
typedef long Align;

union header {
  struct {
    union header *next;
    unsigned size;
  } s;

  Align x;
};
typedef union header Header;

static Header base;           // Used to get an initial member for free list
static Header *freep = NULL;  // Free list starting point


static Header *morecore(unsigned nblocks);
void kandr_free(void *ptr);



void *kandr_malloc(unsigned nbytes) {

  Header *currp;
  Header *prevp;
  unsigned nunits;

  nunits = ((nbytes + sizeof(Header) - 1) / sizeof(Header)) + 1;
  printf("nunits = %u\n", nunits);

  // case: no free list yet exists; we have to initialize.
  if (freep == NULL) {

    // Create degenerate free list; base points to itself and has size 0
    base.s.next = &base;
    base.s.size = 0;

    // Set free list starting point to base address
    freep = &base;
  }

  prevp = freep;
  currp = prevp->s.next;

  for (; ; prevp = currp, currp = currp->s.next) {
    printf("Current Block size: %u\n", currp->s.size);

    if (currp->s.size >= nunits) {

      if (currp->s.size == nunits) {
        prevp->s.next = currp->s.next;
      }
      else {
        // Changes the memory stored at currp to reflect the reduced block size
        currp->s.size -= nunits;
        // Find location at which to create the block header for the new block
        currp += currp->s.size;
        // Store the block size in the new header
        currp->s.size = nunits;
      }

      freep = prevp;
      return (void *) (currp + 1);

    } // end found a block of memory in free list case

    if (currp == freep) {
      if ((currp = morecore(nunits)) == NULL) {
        return NULL;
      }
  
    } // end wrapped around free list case
  
  } // end step through free list looking for memory loop
}

static Header *morecore(unsigned nunits) {

  void *freemem;    // The address of the newly created memory
  Header *insertp;  // Header ptr for integer arithmatic and constructing header

  if (nunits < NALLOC) {
    nunits = NALLOC;
  }

  freemem = sbrk(nunits * sizeof(Header));
  // case: unable to allocate more memory; sbrk returns (void *) -1 on error
  if (freemem == (void *) -1) {
    return NULL;
  }

  // Construct new block
  insertp = (Header *) freemem;
  insertp->s.size = nunits;

  kandr_free((void *) (insertp + 1));
  return freep;
}




void kandr_free(void *ptr) {

  Header *insertp, *currp;

  // Find address of block header for the data to be inserted
  insertp = ((Header *) ptr) - 1;

  for (currp = freep; !((currp < insertp) && (insertp < currp->s.next)); currp = currp->s.next) {

    if ((currp >= currp->s.next) && ((currp < insertp) || (insertp < currp->s.next))) {
      break;
    }
  }

  if ((insertp + insertp->s.size) == currp->s.next) {
    insertp->s.size += currp->s.next->s.size;
    insertp->s.next = currp->s.next->s.next;
  
  } else {
    insertp->s.next = currp->s.next;
  }

  if ((currp + currp->s.size) == insertp) {
    currp->s.size += insertp->s.size;
    currp->s.next = insertp->s.next;
  
  } else {
    currp->s.next = insertp;
  }

  freep = currp;
}

int main(int argc, char const *argv[]) {
  
  int *x, *y, *z;

  printf("Header Size: %lu\n", sizeof(Header));

  x = kandr_malloc(4);
  y = kandr_malloc(4);
  z = kandr_malloc(4);

  printf("x = %p\n", x);
  printf("y = %p\n", y);
  printf("z = %p\n", z);

  kandr_free(z);


  return 0;
}