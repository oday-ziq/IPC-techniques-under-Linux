#include "header.h"
#include "constants.h"
#include "functions.h"

int wheat_containers_carrying = 0, plane_height = 0, parent_id = 0;
int main(int argc, char* argv[]){

    if(argc < 2){
        perror("Expected 2 argumernts");
        exit(-1);
    }

    parent_id = atoi(argv[1]);

    readFile("args.txt");

    plane_height = random_range(MIN_PLANE_HEIGHT, MAX_PLANE_HEIGHT, getpid());

    while(1){

        wheat_containers_carrying = random_range(WHEAT_MIN_PER_PLANE, WHEAT_MAX_PER_PLANE, getpid());

        sleep(random_range(TIME_NEEDED_TO_DROP_CONTAINERS_MIN, TIME_NEEDED_TO_DROP_CONTAINERS_MIN, getpid()));
        // Plane might get targeted.
        if(plane_gets_targeted(getpid()) == 1){
            kill(parent_id, SIGQUIT);
            send_a_message("killplane");
            if(plane_height <= HEIGHT_THRESHOLD){
                char message[20] = "drop ";
                char wheat_containers_carrying_string[3];
                sprintf(wheat_containers_carrying_string, "%d", wheat_containers_carrying);

                strcat(message, wheat_containers_carrying_string);

                char* msg = message;
                send_a_message(msg);
                printf("Plane Fell but containers are saved.\n");
            }
            else{
                printf("Plane Fell and containers lost.\n");
            }
            break;
        }
        

        char message[20] = "drop ";
        char wheat_containers_carrying_string[3];
        sprintf(wheat_containers_carrying_string, "%d", wheat_containers_carrying);

        strcat(message, wheat_containers_carrying_string);

        char* msg = message;
        send_a_message(msg);

        int *containers_count_ptr = get_shared_memory_variable_ptr(SHARED_MEMORY_CONTAINERS_COUNT);

        while(wheat_containers_carrying--){
            (*containers_count_ptr)++;
        }
        

        printf("Current Containers Reached the ground = %d\n", (*containers_count_ptr));
        fflush(stdout);

        sleep(random_range(REFILL_PERIOD_MIN, REFILL_PERIOD_MAX, getpid()));
    }

    return 0;
}