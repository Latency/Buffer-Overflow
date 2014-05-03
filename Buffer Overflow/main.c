/* *******************************************************************************************
* Author:       Latency McLaughlin
* Date:         04/29/2014
* Forum Topic:  http://lnkd.in/dNh2fvC
* *******************************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>

extern int ParseLine(char *str, int size, int padding, int offset);

char buf[3][32];         // 3 secret addresses.

#define PRINT(a,x,l) printf("Value of `%s' - 0x%p\n\t`%"#x"' - %s: Length[%d]\n\n", #a, &a, a, ((char *)#x=="s" ? "string" :  (char *)#x=="p" ? "address" : "digit"), l )
#define CALC(a)      printf("Size of `%s':\n\t8 bits/byte * %d bytes => %d bits\n\n", #a, sizeof(a), 8*sizeof(a))
#define ADDRESS(buf, a)   sprintf(buf, "%p", &a)
#define FUNC(x)           void x(char *str)


enum State {
  Tutorial,
  Normal,
  Advanced,
  Hacker
};


int numlen(float n)
{
  if (n < 0) n = -n;
  int i = 1;
  while ((n /= 10)>0) i++;
  return i;
}


FUNC(foo)
{
  float My_Float = 10.5; // Addr = 0x0023FF4C
  char  My_String[28];   // Addr = 0x0023FF30
  char* data = "\x10\x10\xc0\x42";

  // Will print 10.500000
  PRINT(My_Float, f, numlen(My_Float));
  CALC(My_Float);

  /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Memory map:
  @ : c allocated memory
  # : My_Float allocated memory

      *c                          *My_Float
       0x0023FF30                  0x0023FF4C
       |                           |
       @@@@@@@@@@@@@@@@@@@@@@@@@@@@####
  foo("my string is too long !!!!! XXXXX");

  memcpy will put 0x1010C042 (little endian) in My_Float value.
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
  
  memcpy(My_String, str, strlen(str));  // no bounds checking...%  Turn off Stack Frames (/RTCs)

  // Inject here.
  memcpy(My_String + strlen(str), data, strlen(data));

  // Will print 96.031372
  PRINT(My_Float, f, numlen(My_Float));
 }


FUNC(bar)
{
  puts("Password attempt failure!");
}


FUNC(baz)
{
  puts("*** Authenticated ***");
}


FUNC(qut)
{
  int len  = sizeof(buf) / sizeof(*buf);
  int len2 = sizeof(*buf);
  char *ptr = (char *) &buf;
  for (int x = 0; x < len; x++, ptr += len2) {
    printf("---------- Secret Address #%d ----------\n", x + 1);
    PRINT(buf[x], p, strlen(buf[x]));
    PRINT(ptr   , p, strlen(buf[x]));
  }
  puts("\nAll your base are belong to us!!!");
}


typedef void(*address)(char *);
address jmp[] = { foo, bar, baz, qut };


int IsEqual(int *state, char *str1, char *str2)
{
  if (!strcmp(str1, str2))   // If password == argv[1]
    *state = Advanced;

  // Enforce validation from overflow.
  switch (*state) {
    case Tutorial:
      // Up until now, everything appears to be okay.  Lets do it one more time!
      *state = Normal;
      break;
    case Normal:
    case Advanced:
      jmp[*state](str2);
      break;
    default:
      jmp[Hacker](str2);
      break;
  }

  return *state;
}


int main(int argc, char **argv)
{
  int  ctm = Tutorial;
  char ary[10],
       *str = argv[1];

  if (argc != 2) {
    printf("Usage:  %s <admin_password>\n", argv[0]);
    return 0;
  }

  PRINT(str, s, strlen(str));
  CALC(str);

  // Lets demo some injection here.
  jmp[ctm](str);

  // Do you want to try it yourself?
  strncpy(ary, argv[1], sizeof(ary) - 1);   // Copy original string.
  ary[sizeof(ary) - 1] = '\0';

  //
  // BAM!!! Notice what I did?
  //
  // If I didn't specify the count and used strcpy(), it would have overflowed the array and overwrote isEqual.
  // + another 4 bytes of the the stack frame from the injection bytecode!
  //

  // Check it to be sure our memory is not contaiminated already.
  assert(!ctm);

  // At this point we know for sure the strings are NOT the same.
  
  // Save the addresses for later just so you can see what we are working with.
  ADDRESS(buf[0], ctm);
  ADDRESS(buf[1], ary);
  ADDRESS(buf[2], str);

  // Test the strings and get the result.
  while (IsEqual(&ctm, ary, str) <= Normal) {

    // Now inject a trailing '1' > 11 bytes and overflow the array into the next address space used for 'isEqual'
    printf("Password:  ");

    // Boundary align is 4 bytes on x32 & 8 bytes each on x64.  ary[10+1] + oflw(1) is good enough.
    // But, allocated memory allocated to cache lines is off.
    //
    // Overflow into the next line of 4 bytes on x32 should yield an offset of 4 minus any padding.
    int offset = sizeof(ary);
    const int align = 4;
    int padding = /* align - (offset & (align - 1)) = */ (-offset) & (align - 1);
    // New offset
    offset = (offset + align - 1) & ~(align - 1);
    // ^------------v   What should have been used [0 < offset <= 12]
    if (fgets(ary, 16, stdin) != NULL) {
      puts("");
      PRINT(ary, s, strlen(ary));
    } else {
      puts("Read input failure!");
      break;
    }
  }

  system("pause");
  return 0;
}