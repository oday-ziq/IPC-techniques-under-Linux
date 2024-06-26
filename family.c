#include "header.h"
#include "constants.h"
#include "functions.h"

int starvation_level = 0, parent_id = 0;

void update_shared_memory_max_starvation(){
    int *family_max_starvation_ptr = get_shared_memory_variable_ptr(SHARED_MEMORY_FAMILY_MAX_STARVATION);
    if((*family_max_starvation_ptr) < starvation_level){
        (*family_max_starvation_ptr) = starvation_level;

        int *family_max_starvation_pid_ptr = get_shared_memory_variable_ptr(SHARED_MEMORY_FAMILY_MAX_STARVATION_PID);
        (*family_max_starvation_pid_ptr) = getpid();
    }
}

void alarm_handler(int signum) {
    starvation_level += STARVATION_LEVEL_INCREASE_PER_MIN;
    update_shared_memory_max_starvation();
    /* Check If Family dies */
    if(starvation_level >= STARVATION_LEVEL_PASS_AWAY_THRESHOLD){
        printf("Family died.\n");
        kill(parent_id, SIGINT);
        exit(-1);

    }
    
    alarm(30);
}

// Receive a wheat bag.
void sigusr_handler(int signum){
    printf("Family with pid = %d, received a wheat bag, Current Starvation level = %d\n", getpid(), starvation_level);
    starvation_level = getMax(0, starvation_level - STARVATION_LEVEL_DROP_PER_BAG);

    int *family_max_starvation_pid_ptr = get_shared_memory_variable_ptr(SHARED_MEMORY_FAMILY_MAX_STARVATION_PID);
    if((*family_max_starvation_pid_ptr) == getpid()){
        int *family_max_starvation_ptr = get_shared_memory_variable_ptr(SHARED_MEMORY_FAMILY_MAX_STARVATION);

        (*family_max_starvation_ptr) = starvation_level;
    }
    update_shared_memory_max_starvation();
    signal(SIGUSR1, sigusr_handler);
}

int main(int argc, char* argv[]){
    signal(SIGUSR1, sigusr_handler);

    if(argc < 2){
        perror("Expected 2 argumernts");
        exit(-1);
    }

    parent_id = atoi(argv[1]);

    readFile("args.txt");

    starvation_level = random_range(STARVATION_RATE_MIN, STARVATION_RATE_MAX, getpid());
    update_shared_memory_max_starvation();

    signal(SIGALRM, alarm_handler);
    alarm(30);

    while(1){
        sleep(2);
        update_shared_memory_max_starvation();
    }

    return 0;
}