#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#define chunk_size 125

//els ints ocupen 4 bytes, per tant per tenir-ne 500 

int main(void)
{
  FILE *file;
  int sizeToWrite = 500000000;
  int numbers[chunk_size];
  char file_name[] = "numbers.bin";
  size_t result;
  struct timespec ts0, ts1;
  int res;
  double secs, secs2;


  /* Opening the file for writing*/
  file = fopen(file_name, "w");
  if (file == NULL)
  {
     printf("Error when opening file for writing.\n");
     return -1;
  }

   /* Fake call to warm-up */
   res = clock_gettime(CLOCK_REALTIME, &ts0);
   if (res < 0) { 
      perror ("clock_gettime");
   }


   printf ("Starting to write\n");

   res = clock_gettime(CLOCK_REALTIME, &ts0);
   if (res < 0) { 
      perror ("clock_gettime");
   }
  // Writing a block of data
  while(sizeToWrite > 0){
   result = fwrite(numbers, sizeof(int), chunk_size, file);
   if (result!=chunk_size)
      {
        printf("The %d bytes have not been written.\n", chunk_size);
      }
   sizeToWrite -= chunk_size;
  }

  res = clock_gettime(CLOCK_REALTIME, &ts1);
   if (res < 0) {
      perror ("clock_gettime");
   }
  
  // correct formula!!
   secs = (((double)ts1.tv_sec*1000000000.0 + (double)ts1.tv_nsec) - 
          ((double)ts0.tv_sec*1000000000.0 + (double)ts0.tv_nsec))/1000000000.0;

   printf("Writing time: %f \n", secs);
   printf("Bandwith: %fMbytes/sec\n",500/secs);

  if (fclose(file)!=0)
  {
    printf("Error when closing file.\n");
   return -1;
  }

}
