
#ifndef LIBRARIES
#define LIBRARIES

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <limits.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <sys/sem.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/shm.h>
#include <errno.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <GL/glut.h>
#include <math.h>
#include <GLFW/glfw3.h>
#include <sys/msg.h>
#include <GL/gl.h>
#include <time.h>

#define MAX_LINE_LENGTH 256
#define MESSAGE_QUEUE_PATH "/tmp/message_queue"
#define B_SIZ (PIPE_BUF / 2)

const char* SHARED_MEMORY_CONTAINERS_COUNT = "/containers_count_shared_memory";
const char* SHARED_MEMORY_CONTAINERS_SAFE_AREA_COUNT = "/containers_safe_area_count_shared_memory";
const char* SHARED_MEMORY_WHEAT_BAGS_COUNT = "/wheat_bags_count_shared_memory";
const char* SHARED_MEMORY_FAMILY_MAX_STARVATION = "/family_max_starvation_shared_memory";
const char* SHARED_MEMORY_FAMILY_MAX_STARVATION_PID = "/family_max_starvation_pid_shared_memory";

struct message {
    long message_type;
    char message[100];
};


#endif