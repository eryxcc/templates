#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <SDL/SDL_mutex.h>
#include <unistd.h>

int tab[16];

int calctab(void *ipv) {
  int i = 1, j = 1;
  for(int a=0; a<1000000000; a++)
  for(int b=0; b<10; b++)
    { int k = i+j; i=j; j=k; }
  int* ip = (int*) ipv;
  tab[*ip] = j;
  return 19 + (*ip);
  }

// SDL_Thread *SDL_CreateThread(int (*fn)(void *), void *data);

SDL_mutex *m;

SDL_Thread *t;

int cthrd(void*) {
  sleep(1);
  SDL_mutexP(m);
  printf("b\n");
  sleep(4);
  SDL_mutexV(m);
  printf("c\n");
  return 0;
  }

int main() {
  t = SDL_CreateThread(cthrd, NULL);
  m = SDL_CreateMutex();
  SDL_mutexP(m);
  printf("d\n");
  sleep(5);
  SDL_mutexV(m);
  printf("a\n");
  sleep(1);
  SDL_mutexP(m);
  SDL_mutexV(m);
  return 0;
/*
  int u[16];
  for(int z=0; z<16; z++) u[z] = z;
  SDL_Thread *t[16];
  for(int z=0; z<16; z++)
    t[z] = SDL_CreateThread(calctab, &u[z]);
  for(int z=0; z<16; z++)
    SDL_WaitThread(t[z], &u[z]);
  for(int z=0; z<16; z++)
    printf("v[%d] = %d,%d\n", z, u[z], tab[z]);
  */
  return 0;
  }

/*
SDL_CreateThread -- Creates a new thread of execution that shares its parent's properties.
SDL_ThreadID -- Get the 32-bit thread identifier for the current thread.
SDL_GetThreadID -- Get the SDL thread ID of a SDL_Thread
SDL_WaitThread -- Wait for a thread to finish.
SDL_KillThread -- Gracelessly terminates the thread.
SDL_CreateMutex -- Create a mutex
SDL_DestroyMutex -- Destroy a mutex
SDL_mutexP -- Lock a mutex
SDL_mutexV -- Unlock a mutex
SDL_CreateSemaphore -- Creates a new semaphore and assigns an initial value to it.
SDL_DestroySemaphore -- Destroys a semaphore that was created by SDL_CreateSemaphore.
SDL_SemWait -- Lock a semaphore and suspend the thread if the semaphore value is zero.
SDL_SemTryWait -- Attempt to lock a semaphore but don't suspend the thread.
SDL_SemWaitTimeout -- Lock a semaphore, but only wait up to a specified maximum time.
SDL_SemPost -- Unlock a semaphore.
SDL_SemValue -- Return the current value of a semaphore.
SDL_CreateCond -- Create a condition variable
SDL_DestroyCond -- Destroy a condition variable
SDL_CondSignal -- Restart a thread wait on a condition variable
SDL_CondBroadcast -- Restart all threads waiting on a condition variable
SDL_CondWait -- Wait on a condition variable
SDL_CondWaitTimeout -- Wait on a condition variable, with timeout
*/
