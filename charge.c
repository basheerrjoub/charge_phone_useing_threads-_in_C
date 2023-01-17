#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/mman.h>

// Thread shareds function
void *fill_tank(void *ptr);
void *progress_bar(void *ptr);
void red();
void yellow();
void reset();
int randNum(int, int);
// Flag to terminate all threads
int kill_flag = 0;

// Shared struct between threads
struct box
{
    int tank;
    char text[100];
};
struct box *b;

// Initialize Mutex
pthread_mutex_t key = PTHREAD_MUTEX_INITIALIZER;

int main()
{

    int shmID;
    shmID = shmget(1000, sizeof(struct box), 0666 | IPC_CREAT);
    if (shmID < 0)
    {
        printf("error\n");
        return 1;
    }

    b = (struct box *)shmat(shmID, NULL, 0);

    // Initialize count = 0
    b->tank = 0;
    system("clear");
    fprintf(stderr, "       .oO(No Charge?!!)\n");
    fprintf(stderr, "    😭    ｜\n");
    fprintf(stderr, "    ⁣/|\\📱 ｜\n");
    fprintf(stderr, "|   / \\   |\n");
    fprintf(stderr, "￣￣￣￣￣￣\n");

    // Create 2 threads
    pthread_t threads[3];
    char *th0 = "Thread 0";
    char *th1 = "Thread 1";
    char *th2 = "Thread 2";

    // Save threads return values
    int thread_ret[3];
    thread_ret[0] = pthread_create(&threads[0], NULL, fill_tank, th0);
    thread_ret[1] = pthread_create(&threads[1], NULL, fill_tank, th1);
    thread_ret[2] = pthread_create(&threads[2], NULL, progress_bar, th2);

    
    
    // Join the threads after completion
    for (int i = 0; i < 3; i++)
        pthread_join(threads[i], NULL);
    exit(0);
}

void *fill_tank(void *ptr)
{
    char *message;
    message = (char *)ptr;
  
    while (!kill_flag)
    {
        
        usleep(randNum(1,10)*60000);
        pthread_mutex_lock(&key);

        // Critical section
        (b->tank)+= 5;
        // printf("%s\n", message);

        pthread_mutex_unlock(&key);
        usleep(100000);
        
    }

    fprintf(stderr, "TankFill Exit\n");
    pthread_exit(0);
}

int flag = 0;
// This thread's function will build the progress bar
void *progress_bar(void *ptr)
{
    yellow();
    fprintf(stderr, "Charging:🔌[   ");
    reset();
    while (1)
    {
        pthread_mutex_lock(&key);
        // Critical section
        if (flag < (b->tank))
        {
            flag+=5;
            red();
            fprintf(stderr, "\033[3D");
            fprintf(stderr, "\a🟩%d%%", flag);
            reset();

            if (flag == 100)
            {
                kill_flag = 1;
                yellow();
                fprintf(stderr, "⚡]\n");
                reset();
                fprintf(stderr, "PB Exit\n");
                fprintf(stderr, "       .oO(Full Charge)\n");
                sleep(1);
                for(;;){
                    system("clear");
                    fprintf(stderr, "    😊    \n");
                    fprintf(stderr, "    ⁣/|\\📱 \n");
                    fprintf(stderr, "|   / \\   \n");
                    fprintf(stderr, "￣￣￣￣￣￣\n");
                    usleep(500000);
                    system("clear");
                    fprintf(stderr, "    😃  📱  \n");
                    fprintf(stderr, "    \\|/⚡\n");
                    fprintf(stderr, "|   / \\  \n");
                    fprintf(stderr, "￣￣￣￣￣￣\n");
                    usleep(500000);

                }
                pthread_exit(0);
            }
        }

        pthread_mutex_unlock(&key);
        //Adding 10us delay for stability
        usleep(10);
    }
}




void red () {
  fprintf(stderr, "\033[1;31m");

}

void yellow() {
    fprintf(stderr, "\033[1;33m");
}

void reset () {
    fprintf(stderr, "\033[0m");
}


//Generating a random number
int randNum(int lower, int upper)
{
    int s;
    FILE *f;
    f = fopen("/dev/urandom", "rb");
    fread(&s, sizeof(int), 1, f);
    fclose(f);
    srand(s);
    int num = (rand() % (upper - lower + 1)) + lower;
    return num;

}
