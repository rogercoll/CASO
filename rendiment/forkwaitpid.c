#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>


int main(int argc, char * argv [])
{
   int res;
   void *p;
   struct timespec ts0, ts1;
   double secs, secs2;
   int n = 6000, i;
   int child_pids[n];

   if (argc > 1) {
      fprintf(stderr, "Us: %s\n", argv[0]);
      exit(1);
   }

   /* Fake call to warm-up */
   res = clock_gettime(CLOCK_REALTIME, &ts0);
   if (res < 0) { 
      perror ("clock_gettime");
   }
   /* Fake call to warm-up */
   
   printf ("Starting the test\n");

   res = clock_gettime(CLOCK_REALTIME, &ts0);
   if (res < 0) { 
      perror ("clock_gettime");
   }

   for (i=0; i < n; i++) {
      res = fork();
      if (res < 0) {
         perror ("fork");
      }
      else if(res == 0){
         child_pids[i] = getpid();
         exit(i);
         break;
      };
   }

   for(i=0;i<n;i++){
      int status;
      waitpid(child_pids[i], &status, 0);
      printf("exit status %d = %d\n", i, WEXITSTATUS(status));
   }

   res = clock_gettime(CLOCK_REALTIME, &ts1);
   if (res < 0) {
      perror ("clock_gettime");
   }

   // correct formula!!
   secs = (((double)ts1.tv_sec*1000000000.0 + (double)ts1.tv_nsec) - 
          ((double)ts0.tv_sec*1000000000.0 + (double)ts0.tv_nsec))/1000000000.0;

   //// incorrect formula!!!!
   //secs2 = (double) (ts1.tv_sec - ts0.tv_sec) + (((double)(ts1.tv_nsec - ts0.tv_nsec))/1000000000.0);
   secs2 = (((double) (ts1.tv_sec - ts0.tv_sec)*1000000000.0) + (((double)(ts1.tv_nsec - ts0.tv_nsec))))/1000000000.0;

   if (secs!=secs2) {
      printf ("  Error %.24lf\n      %.24lf!!!\n", secs, secs2);
      printf ("  ts0      %d      %d\n", ts0.tv_sec, ts0.tv_nsec);
      printf ("  ts1      %d      %d\n", ts1.tv_sec, ts1.tv_nsec);
      printf ("  diff     %d      %d\n", ts1.tv_sec - ts0.tv_sec, ts1.tv_nsec - ts0.tv_nsec);
   }
   if (ts1.tv_nsec - ts0.tv_nsec < 0) printf ("   Negative!!   %d     %d     %d\n", ts0.tv_nsec, ts1.tv_nsec, ts1.tv_nsec - ts0.tv_nsec);
   printf (" secs   %.8lf s. (%le s/sleep)\n", secs, secs / n );
   printf (" secs2  %.8lf s. (%le s/sleep)\n", secs2, secs2 / n );
   printf("\n");

  
   

}
