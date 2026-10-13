#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(void) {
  // Allocate 32 pages of memory. Because memset is disabled in the kernel,
  // this memory will contain the remnants of the 'secret' process.
  char *mem = (char *)sbrk(32 * 4096);
  if (mem == (char *)-1) {
    exit(1);
  }

  // Scan the raw memory for the abandoned secret string
  for (int i = 0; i < 32 * 4096; i++) {
    
    // Check if the current byte is an alphanumeric character
    if ((mem[i] >= 'A' && mem[i] <= 'Z') || 
        (mem[i] >= 'a' && mem[i] <= 'z') || 
        (mem[i] >= '0' && mem[i] <= '9')) {
      
      int len = 0;
      // Measure the length of the consecutive alphanumeric sequence
      while ((mem[i+len] >= 'A' && mem[i+len] <= 'Z') || 
             (mem[i+len] >= 'a' && mem[i+len] <= 'z') || 
             (mem[i+len] >= '0' && mem[i+len] <= '9')) {
        len++;
      }
      
      // A valid secret is a string of alphanumeric characters terminated by a null byte.
      // We check for len >= 2 to filter out random single-character garbage.
      if (len >= 2 && mem[i + len] == '\0') {
        printf("%s\n", &mem[i]);
        exit(0);
      }
      
      // Skip past this block to continue searching
      i += len; 
    }
  }
  
  exit(0);
}