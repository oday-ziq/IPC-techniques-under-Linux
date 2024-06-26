#ifndef __FUNCTIONS__
#define __FUNCTIONS__

#include "header.h"
#include "constants.h"


void readFile(char* filename){
    char line[200];
    char label[50];

    FILE *file;

    file = fopen(filename, "r");

    if (file == NULL){
        perror("The file not exist\n");
        exit(-2);
    }

    char separator[] = "=";

    while(fgets(line, sizeof(line), file) != NULL){

        char *str = strtok(line, separator);
        strncpy(label, str, sizeof(label));
        str = strtok(NULL, separator);

        if (strcmp(label, "COLLECTING_WORKERS_COMMITTEES") == 0){
            COLLECTING_WORKERS_COMMITTEES = atoi(str);
        } else if (strcmp(label, "COLLECTING_WORKERS_PER_COMMITTEE") == 0){
            COLLECTING_WORKERS_PER_COMMITTEE = atoi(str);
        } else if(strcmp(label, "SPLITTING_WORKERS_COMMITTEES") == 0){
            SPLITTING_WORKERS_COMMITTEES = atoi(str);
        } else if(strcmp(label, "SPLITTING_WORKERS_PER_COMMITTEE") == 0){
            SPLITTING_WORKERS_PER_COMMITTEE = atoi(str);
        } else if(strcmp(label, "DISTRIBUTING_WORKERS_COMMITTEES") == 0){
            DISTRIBUTING_WORKERS_COMMITTEES = atoi(str);
        } else if(strcmp(label, "DISTRIBUTING_WORKERS_PER_COMMITTEE") == 0){
            DISTRIBUTING_WORKERS_PER_COMMITTEE = atoi(str);
        } else if(strcmp(label, "CARGO_PLANES") == 0){
            CARGO_PLANES = atoi(str);
        } else if(strcmp(label, "WHEAT_MIN_PER_PLANE") == 0){
            WHEAT_MIN_PER_PLANE = atoi(str);
        } else if(strcmp(label, "WHEAT_MAX_PER_PLANE") == 0){
            WHEAT_MAX_PER_PLANE = atoi(str);
        } else if(strcmp(label, "WHEAT_CONTAINER_MASS") == 0){
            WHEAT_CONTAINER_MASS = atoi(str);
        } else if(strcmp(label, "MIN_PLANE_HEIGHT") == 0){
            MIN_PLANE_HEIGHT = atoi(str);
        } else if(strcmp(label, "MAX_PLANE_HEIGHT") == 0){
            MAX_PLANE_HEIGHT = atoi(str);
        } else if(strcmp(label, "HEIGHT_THRESHOLD") == 0){
            HEIGHT_THRESHOLD = atoi(str);
        } else if(strcmp(label, "REFILL_PERIOD_MIN") == 0){
            REFILL_PERIOD_MIN = atoi(str);
        } else if(strcmp(label, "REFILL_PERIOD_MAX") == 0){
            REFILL_PERIOD_MAX = atoi(str);
        } else if(strcmp(label, "COLLECTING_WORKERS_ENERGY_LEVEL_MIN") == 0){
            COLLECTING_WORKERS_ENERGY_LEVEL_MIN = atoi(str);
        } else if(strcmp(label, "COLLECTING_WORKERS_ENERGY_LEVEL_MAX") == 0){
            COLLECTING_WORKERS_ENERGY_LEVEL_MAX = atoi(str);
        } else if(strcmp(label, "COLLECTING_WORKER_TIRING_LEVEL") == 0){
            COLLECTING_WORKER_TIRING_LEVEL = atoi(str);
        } else if(strcmp(label, "REPLACE_COLLECTING_WORKER_THRESHOLD") == 0){
            REPLACE_COLLECTING_WORKER_THRESHOLD = atoi(str);
        } else if(strcmp(label, "NUM_OF_FAMILIES") == 0){
            NUM_OF_FAMILIES = atoi(str);
        } else if(strcmp(label, "DISTRIBUTION_COMMITTIEE_CARRY_BAGS") == 0){
            DISTRIBUTION_COMMITTIEE_CARRY_BAGS = atoi(str);
        } else if(strcmp(label, "STARVATION_RATE_MIN") == 0){
            STARVATION_RATE_MIN = atoi(str);
        } else if(strcmp(label, "STARVATION_RATE_MAX") == 0){
            STARVATION_RATE_MAX = atoi(str);
        } else if(strcmp(label, "REPLACE_DISTRIBUTING_WORKER_THRESHOLD") == 0){
            REPLACE_DISTRIBUTING_WORKER_THRESHOLD = atoi(str);
        } else if(strcmp(label, "STARVATION_LEVEL_INCREASE_PER_MIN") == 0){
            STARVATION_LEVEL_INCREASE_PER_MIN = atoi(str);
        } else if(strcmp(label, "STARVATION_LEVEL_DROP_PER_BAG") == 0){
            STARVATION_LEVEL_DROP_PER_BAG = atoi(str);
        } else if(strcmp(label, "STARVATION_LEVEL_PASS_AWAY_THRESHOLD") == 0){
            STARVATION_LEVEL_PASS_AWAY_THRESHOLD = atoi(str);
        } else if(strcmp(label, "APP_LENGTH_SECONDS") == 0){
            APP_LENGTH_SECONDS = atoi(str);
        } else if(strcmp(label, "CARGO_PLANES_CRASHED_THRESHOLD") == 0){
            CARGO_PLANES_CRASHED_THRESHOLD = atoi(str);
        } else if(strcmp(label, "LOST_CARGO_CONTAINERS_THRESHOLD") == 0){
            LOST_CARGO_CONTAINERS_THRESHOLD = atoi(str);
        } else if(strcmp(label, "COLLECTING_WORKERS_MARTYRS_THRESHOLD") == 0){
            COLLECTING_WORKERS_MARTYRS_THRESHOLD = atoi(str);
        } else if(strcmp(label, "DISTRIBUTION_WORKERS_MARTYRS_THRESHOLD") == 0){
            DISTRIBUTION_WORKERS_MARTYRS_THRESHOLD = atoi(str);
        } else if(strcmp(label, "FAMILIES_PASS_AWAY_THRESHOLD") == 0){
            FAMILIES_PASS_AWAY_THRESHOLD = atoi(str);
        } else if(strcmp(label, "TIME_NEEDED_TO_DROP_CONTAINERS_MIN") == 0){
            TIME_NEEDED_TO_DROP_CONTAINERS_MIN = atoi(str);
        } else if(strcmp(label, "TIME_NEEDED_TO_DROP_CONTAINERS_MAX") == 0){
            TIME_NEEDED_TO_DROP_CONTAINERS_MAX = atoi(str);
        } else if(strcmp(label, "TIME_BETWEEN_TWO_PLANES") == 0){
            TIME_BETWEEN_TWO_PLANES = atoi(str);
        } else if(strcmp(label, "TIME_NEEDED_TO_COLLECT_CONTAINERS_MIN") == 0){
            TIME_NEEDED_TO_COLLECT_CONTAINERS_MIN = atoi(str);
        } else if(strcmp(label, "TIME_NEEDED_TO_COLLECT_CONTAINERS_MAX") == 0){
            TIME_NEEDED_TO_COLLECT_CONTAINERS_MAX = atoi(str);
        }

    }

    fclose(file);
}

// Returns a random sleep dependant to energy_level, the bigger the energy level the less time the ball takes time to arrive to the next player.
int getRandomSleepForEnergyLevel(int energy_level, pid_t process_id){
    srand(process_id);
    return 1 + rand() % (300 / energy_level);
}

int random_range(int left, int right, pid_t process_id){
    srand(process_id);
    return left + rand() % (right - left + 1);
}

int getMax(int a, int b){
    if(a > b){
        return a;
    }
    return b;
}

void initialize_shared_memory_variable(char* name){
    const int SIZE = sizeof(int);

    int shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SIZE);

    int *ptr = (int *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    *ptr = 0;
}

int* get_shared_memory_variable_ptr(char *name){
    const int SIZE = sizeof(int);

    int shm_fd = shm_open(name, O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(-1);
    }

    int *ptr = (int *)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        exit(-1);
    }
    return ptr;
}

void send_a_message(char* msg){
    // Create a file if it doesn't exist
    int fd = open(MESSAGE_QUEUE_PATH, O_CREAT | O_RDWR, 0666);
    if (fd == -1) {
        perror("open");
        exit(1);
    }
    close(fd);
    key_t key = ftok(MESSAGE_QUEUE_PATH, 'A');
    
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    // Create or obtain the message queue ID
    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    // Prepare the message
    struct message message;
    message.message_type = 1; // Unique message type
    strcpy(message.message, msg);

    // Send the message
    if (msgsnd(msgid, &message, sizeof(message), 0) == -1) {
        perror("msgsnd");
        exit(1);
    }
    printf("Message Sent = %s\n", message.message);
}

char* receive_a_message(){
    static struct message received_message;
    key_t key = ftok(MESSAGE_QUEUE_PATH, 'A');
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    // Receive the message
    if (msgrcv(msgid, &received_message, sizeof(received_message), 1, IPC_NOWAIT) == -1) {
        if (errno == ENOMSG) {
            // No message available yet
            return NULL;
        } else {
            perror("msgrcv");
            exit(1);
        }
    }

    return received_message.message;
}

int get_random_sleep_according_to_energy_level(int total_energy, pid_t process_id){
    return random_range(TIME_NEEDED_TO_COLLECT_CONTAINERS_MIN, getMax(TIME_NEEDED_TO_COLLECT_CONTAINERS_MIN, 15000/total_energy), process_id);
}

int plane_gets_targeted(pid_t process_id){
    if(random_range(0, 100, process_id) <= 5){
        return 1;
    }
    return 0;
}

int worker_get_targeted(int energy_level, pid_t process_id){
    if(random_range(0, 100, process_id) <= 200/energy_level){
        return 1;
    }
    return 0;
}

void kill_process(pid_t pid) {
    if (kill(pid, SIGKILL) == -1) {
        perror("kill");
    }
}
#endif