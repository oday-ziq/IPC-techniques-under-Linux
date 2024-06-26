#include "header.h"
#include "constants.h"
#include "functions.h"

int current_workers_count = 0, sum_energy_level = 0, parent_id = 0;

int workers[100];

void update_workers(){
    int new_energy_level = 0;
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

    current_workers_count = SPLITTING_WORKERS_PER_COMMITTEE;

    for(int i = 0; i < COLLECTING_WORKERS_PER_COMMITTEE; i++){
        int energy_level = random_range(COLLECTING_WORKERS_ENERGY_LEVEL_MIN, COLLECTING_WORKERS_ENERGY_LEVEL_MAX, getpid());
        workers[i] = energy_level;
        sum_energy_level += energy_level;
    }

    while(1){
        sleep(2);

        int *containers_safe_area_count_ptr = get_shared_memory_variable_ptr(SHARED_MEMORY_CONTAINERS_SAFE_AREA_COUNT);
        printf("Containers in Safe area = %d\n", *(containers_safe_area_count_ptr));

        if((*containers_safe_area_count_ptr) != 0){
            (*containers_safe_area_count_ptr)--;

            sleep(get_random_sleep_according_to_energy_level(sum_energy_level, getpid()));
            update_workers();

            send_a_message("split");

            int *wheat_bags_count_ptr = get_shared_memory_variable_ptr(SHARED_MEMORY_WHEAT_BAGS_COUNT);
            (*wheat_bags_count_ptr) += WHEAT_CONTAINER_MASS;
            printf("Wheat bags count = %d\n", *wheat_bags_count_ptr);
        }
    }
    return 0;
}