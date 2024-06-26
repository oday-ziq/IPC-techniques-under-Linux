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
            printf("Distributing Worker unfortunatelly fallen.\n");
            kill(parent_id, SIGUSR2);
            send_a_message("distributedie");
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

    current_workers_count = DISTRIBUTING_WORKERS_PER_COMMITTEE;

    for(int i = 0; i < COLLECTING_WORKERS_PER_COMMITTEE; i++){
        int energy_level = random_range(COLLECTING_WORKERS_ENERGY_LEVEL_MIN, COLLECTING_WORKERS_ENERGY_LEVEL_MAX, getpid());
        workers[i] = energy_level;
        sum_energy_level += energy_level;
    }
    while(1){
        sleep(2);

        int *wheat_bags_count_ptr = get_shared_memory_variable_ptr(SHARED_MEMORY_WHEAT_BAGS_COUNT);
        if(*(wheat_bags_count_ptr) != 0){
            int new_wheat_bags_count = getMax(0, (*wheat_bags_count_ptr) - DISTRIBUTION_COMMITTIEE_CARRY_BAGS);
            int carry_bags = (*wheat_bags_count_ptr) - new_wheat_bags_count;
            (*wheat_bags_count_ptr) = new_wheat_bags_count;

            sleep(get_random_sleep_according_to_energy_level(sum_energy_level, getpid()));
            update_workers();

            send_a_message("send");

            while(carry_bags--){
                sleep(2);

                int *family_max_starvation_pid_ptr = get_shared_memory_variable_ptr(SHARED_MEMORY_FAMILY_MAX_STARVATION_PID);

                // Sending a signal to receive a wheat bag.
                kill((*family_max_starvation_pid_ptr), SIGUSR1);
            }
        }
    }
    return 0;
}