#include <stdio.h>
#include <stdlib.h>

char * tmpname = "/tmp/XXXXXX";
int fh;
int main(int argc, char * argv[])
{
  printf("\nfh: %08x %s\n", fh, tmpname);
  fh = mkstemp (tmpname);
  printf("\nfh: %08x %s\n", fh, tmpname);
}

