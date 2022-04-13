#include "param.h"
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"
#include "memlayout.h"


// Test munprotect returns -1 for non-aligned address
void testPageAllignment(){
  printf(1, "Testing page alignment: ");
  if (munprotect((int *) 0x1001, 1) != -1) {
    printf(1, "Test Failed\n");
    exit();
    }
  else {
    printf(1, "Test Passed\n");
  }
  printf(1, "\n");
}

// Test munprotect returns -1 for len 0
void testZeroLength(){
  printf(1, "Testing length of 0: ");
  if (munprotect((int *) 0x1000, 0) != -1) {
    printf(1, "Test Failed\n");
    exit();
  }
  else {
    printf(1, "Test Passed\n");
  }
  printf(1, "\n");
}

// Test munprotect returns -1 for addresses above and below the process limit
void testInvalidAddress(){
  // Test mprotect returns -1 for an address below the base of process.
	printf(1, "Testing address lower-bound: ");
  if (munprotect((int *) 0xFFF, 1) != -1) {
  	printf(1, "Test Failed\n");
    exit();
  }
  else {
  	printf(1, "Test Passed\n");
  }
  printf(1, "\n");

  // Test munprotect returns -1 for an address above the process limit
  printf(1, "Testing address upper-bound: ");
  if (mprotect((int *) KERNBASE, 1) != -1) {
  	printf(1, "Test Failed\n");
    exit();
  }
  else {
  	printf(1, "Test Passed\n");
  }
  printf(1, "\n");
}

//Test munprotect works for a valid address, and a valid length
void testValidAddress(){
  int num = 2;
  int * addr = (int *) 0x1000;

  printf(1, "Temporarily setting %d pages from address: 0x%p to read-only in order to test munprotect\n", num, addr);
  if (mprotect(addr, num) != 0) {
 		printf(1, "Failed to run mprotect on desired address");
  	exit();
  }
  else {
		printf(1, "Successfully protected desired addresses\n");
  }
  printf(1, "\n");

	printf(1, "Attempting to unprotect 1 page from address: 0x%p\n", addr);
	if (munprotect(addr, 1) != 0) {
  	printf(1, "Running munprotect failed");
    exit();
  }
  else {
  	printf(1, "Running munprotect was successful\n");
  }
  printf(1, "\n");
	
  // 0x1000 should not protected (due to munprotect)
  printf(1, "Attempting to write to an address that is writeable (trap should not occur)\n");
  int * mem = (int *) 0x1000;
  *mem = 5;
  printf(1, "Writing was successful\n");
  printf(1, "\n");

  printf(1, "Attempting to write to an address that is READ-ONLY (trap should occur)\n");
  // 0x2000 should be protected
  int * rdonly = (int *) 0x2000;
  *rdonly = 5;
  // If a trap doesn't occur, munprotect didn't work.
  printf(1, "This message shouldnt appear. Test Failed- mprotect did not work.");
}


int main(int argc, char *argv[])
{
  printf(1, "Testing mprotect..\n");	
  testPageAllignment();
  testZeroLength();
  testInvalidAddress();
  testValidAddress();
  exit();
}
