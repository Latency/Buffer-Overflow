#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>

float atoff(const char *s);

typedef struct MY_TYPE {
  char *word;
  int *inumb;
  float *fnumb;
  //  struct MY_TYPE *next;
} MY_TYPE;

//struct MY_TYPE *node {
//  word = "the quick brown fox jumps over the lazy dog"
int main(int argc, const char *argv[])
{
  int j, debug = 0, len = 0, lenMT = 0, lenT = 0, k = 0, len1 = 0, len2 = 0;
  int inte;
  float flt;

  for (j = 0; j < argc; j++)
    printf("argv[%d] = \"%s\"\n", j, argv[j]);

  //   for (i=0; i++; i< 99)
  //    inGPS[i] = '\0';

  struct MY_TYPE *new2MY_TYPE = (MY_TYPE *)malloc(sizeof(struct MY_TYPE));
  //  struct MY_TYPE *head = (MY_TYPE *) malloc(sizeof(struct MY_TYPE));
  // struct MY_TYPE *test1 = (MY_TYPE *) malloc(sizeof(struct MY_TYPE));
  struct MY_TYPE *test1, *head;
  len = sizeof(new2MY_TYPE);
  if (debug) printf("%p\n", (void *)new2MY_TYPE, len);
  int i = 1, x = 0;
  //char *words[9] = {"the1", "quick", "brown", "fox", "jumps", "over", "the2", "lazy", "dog"};
  printf("argc is %i\n", argc--);
  len1 = (void *)new2MY_TYPE;
  while (i < argc) {
    new2MY_TYPE->word = argv[i];
    printf("words are %s %p %i\n", new2MY_TYPE->word, (void *)new2MY_TYPE, i);
    ++i;
    inte = atoi(argv[i]);
    new2MY_TYPE->inumb = &inte;
    printf("int numb %i  %p %i\n", *new2MY_TYPE->inumb, (void *)new2MY_TYPE, i);
    ++i;
    flt = atof(argv[i]);
    new2MY_TYPE->fnumb = &flt;
    printf("float numb %.4f  %p %i\n", *new2MY_TYPE->fnumb, (void *)new2MY_TYPE, i);
    ++i;
    if (debug)  printf("sizeof is %i\n", sizeof(new2MY_TYPE));

    if (i >= argc)
      break;
    else {
      ++new2MY_TYPE;
    }
    new2MY_TYPE = (MY_TYPE *)malloc(sizeof(struct MY_TYPE));
    //      new2MY_TYPE->word = words[x++];
    len = sizeof(new2MY_TYPE);
    if (i == 4)
      len2 = (void *)new2MY_TYPE;
  }
  lenT = len2 - len1;
  // reverse words
  printf("\nReversing aggregate Structure Nodes Now \n\n");
  //printf("%s %p\n", new2MY_TYPE->word, (void *) new2MY_TYPE);
  if (debug) printf("word %i %s %p\n", i, new2MY_TYPE->word, (void *)new2MY_TYPE);
  test1 = new2MY_TYPE;
  k = i / 3;
  if (debug) printf("outside loop \n%s %p %i\n", test1->word, (void *)test1, k);

  while (k-- > 0) {
    lenMT = sizeof(struct MY_TYPE);
    //lenT = 0x84f1038-0x84f1028;
    if (debug) printf("inside loop i is %i %s lenMT is %i, lenT is %x %p\n", i, test1->word, lenMT, lenT, (void *)test1);

    printf("fnumb is %.4f  %p\n", *test1->fnumb, (void *)test1);
    printf("inumb is %i  %p\n", *test1->inumb, (void *)test1);
    printf("words are %s  %p\n", test1->word, (void *)test1);

    test1 = ((void *)test1) - lenT;

    //test1=head;
    //--test1;
    // %i %f (int) test1->inumb, *test1->fnumb,
    //printf(" loop %s %p\n", test1->word,  (void *) test1);
    if (debug)  printf("  loop %p\n", (void *)test1);
  }

}
//
//run with this command, on Linux :
//
//. / ew5 "the quick brown fox jumps over the lazy dog" 5 77.3 "this broken messy absolutely not working piss poor code" 8 34.2 "this is a test and only a test" 22 3.456
//
//It still has a wicked issue where the numbers are not being changed as the pointer address is;
//
/// Programming / C$ . / ew5 "the quick brown fox jumps over the lazy dog" 5 77.3 "this broken messy absolutely not working piss poor code" 8 34.2 "this is a test and only a test" 22 3.456
//argv[0] = "./ew5"
//argv[1] = "the quick brown fox jumps over the lazy dog"
//argv[2] = "5"
//argv[3] = "77.3"
//argv[4] = "this broken messy absolutely not working piss poor code"
//argv[5] = "8"
//argv[6] = "34.2"
//argv[7] = "this is a test and only a test"
//argv[8] = "22"
//argv[9] = "3.456"
//argc is 10
//words are the quick brown fox jumps over the lazy dog 0x8131008 1
//int numb 5  0x8131008 2
//float numb 77.3000  0x8131008 3
//words are this broken messy absolutely not working piss poor code 0x8131018 4
//int numb 8  0x8131018 5
//float numb 34.2000  0x8131018 6
//words are this is a test and only a test 0x8131028 7
//int numb 22  0x8131028 8
//float numb 3.4560  0x8131028 9
//
//Reversing aggregate Structure Nodes Now
//
//fnumb is 3.4560  0x8131028
//inumb is 22  0x8131028
//words are this is a test and only a test  0x8131028
//fnumb is 3.4560  0x8131018
//inumb is 22  0x8131018
//words are this broken messy absolutely not working piss poor code  0x8131018
//fnumb is 3.4560  0x8131008
//inumb is 22  0x8131008
//words are the quick brown fox jumps over the lazy dog  0x8131008
