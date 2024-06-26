#include "header.h"
#include "constants.h"
#include "functions.h"

int current_workers_count = 0, sum_energy_level = 0, parent_id = 0;

int workers[100];

void update_workers(){
    int new_energy_level = 0;
    for(int i = 0; i < current_workers_count; i ++){
        if(worker_get_targeted(workers[i], getpid()) == 1){
            workers[i] = workers[current_workers_count - 1];
            i--;
            current_workers_count--;
            printf("Collecting Worker unfortunatelly has fallen.\n");
            kill(parent_id, SIGUSR1);
            send_a_message("collectdie");
        }
    }
    for(int i = 0; i < current_workers_count; i++){
        workers[i] = getMax(5, workers[i] - COLLECTING_WORKER_TIRING_LEVEL);
        new_energy_level += workers[i];
    }
    sum_energy_level = new_energy_level;
}

int main(int argc, char* argv[]){

    if(argc < 2){
        perror("Expected 2 argumernts");
        exit(-1);
    }

    parent_id = atoi(argv[1]);

    readFile("args.txt");

    current_workers_count = COLLECTING_WORKERS_PER_COMMITTEE;

    for(int i = 0; i < COLLECTING_WORKERS_PER_COMMITTEE; i++){
        int energy_level = random_range(COLLECTING_WORKERS_ENERGY_LEVEL_MIN, COLLECTING_WORKERS_ENERGY_LEVEL_MAX, getpid());
        workers[i] = energy_level;
        sum_energy_level += energy_level;
    } 

    while(1){
        sleep(2);
        int *containers_count_ptr = get_shared_memory_variable_ptr(SHARED_MEMORY_CONTAINERS_COUNT);
        
        if((*containers_count_ptr) != 0){
            (*containers_count_ptr)--;

            sleep(get_random_sleep_according_to_energy_level(sum_energy_level, getpid()));
            update_workers();

            send_a_message("collect");
            
            //printf("Workers a continer, new containers amount on the ground = %d\n", *(containers_count_ptr));

            int *containers_safe_area_count_ptr = get_shared_memory_variable_ptr(SHARED_MEMORY_CONTAINERS_SAFE_AREA_COUNT);

            (*containers_safe_area_count_ptr)++;
        }
    }

    return 0;
}