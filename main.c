#include "header.h"
#include "constants.h"
#include "functions.h"

int dead_families = 0, targeted_planes = 0, collect_martyers = 0, distribute_martyers = 0;
time_t start_time, current_time;

pid_t cargo_planes[100];
pid_t collecting_workers_committees[100];
pid_t splitting_workers_committees[100];
pid_t distributing_workers_committees[100];
pid_t families[100];


void create_collecting_worker_committees(){

    for(int i = 0; i < COLLECTING_WORKERS_COMMITTEES; i++){
        collecting_workers_committees[i] = fork();
        if(collecting_workers_committees[i] == -1){
            perror('Committee Creation Failed');
            exit(-1);
        }
        else if(collecting_workers_committees[i] == 0){
            pid_t parent_id = getpid();
            char parent_id_string[20];
            sprintf(parent_id_string, "%d", parent_id);

            execlp("./collecting_worker_committee", "collecting_worker_committee", parent_id_string, NULL);
            perror("Exec Error\n");
            exit(-5);
        }
    }
}

void create_splitting_worker_committees(){

    for(int i = 0; i < COLLECTING_WORKERS_COMMITTEES; i++){
        splitting_workers_committees[i] = fork();
        if(splitting_workers_committees[i] == -1){
            perror('Committee Creation Failed');
            exit(-1);
        }
        else if(splitting_workers_committees[i] == 0){
            pid_t parent_id = getpid();
            char parent_id_string[20];
            sprintf(parent_id_string, "%d", parent_id);

            execlp("./splitting_worker_committee", "splitting_worker_committee", parent_id_string, NULL);
            perror("Exec Error\n");
            exit(-5);
        }
    }
}

void create_distributing_worker_committees(){

    for(int i = 0; i < COLLECTING_WORKERS_COMMITTEES; i++){
        distributing_workers_committees[i] = fork();
        if(distributing_workers_committees[i] == -1){
            perror('Committee Creation Failed');
            exit(-1);
        }
        else if(distributing_workers_committees[i] == 0){
            pid_t parent_id = getpid();
            char parent_id_string[20];
            sprintf(parent_id_string, "%d", parent_id);

            execlp("./distributing_worker_committee", "distributing_worker_committee", parent_id_string, NULL);
            perror("Exec Error\n");
            exit(-5);
        }
    }
}

void create_cargo_planes(){

    for(int i = 0; i < CARGO_PLANES; i++){
        if(i != 0){
            sleep(TIME_BETWEEN_TWO_PLANES);
        }
        cargo_planes[i] = fork();
        if(cargo_planes[i] == -1){
            perror('Plane Creation Failed');
            exit(-1);
        }
        else if(cargo_planes[i] == 0){
            pid_t parent_id = getpid();
            char parent_id_string[20];
            sprintf(parent_id_string, "%d", parent_id);

            execlp("./cargo_plane", "cargo_plane", parent_id_string, NULL);
            perror("Exec Error\n");
            exit(-5);
        }
    }
}

void create_families(){

    for(int i = 0; i < NUM_OF_FAMILIES; i++){
        families[i] = fork();
        if(families[i] == -1){
            perror('Family Creation Failed');
            exit(-1);
        }
        else if(families[i] == 0){
            pid_t parent_id = getpid();
            char parent_id_string[20];
            sprintf(parent_id_string, "%d", parent_id);

            execlp("./family", "family", parent_id_string, NULL);
            perror("Exec Error\n");
            exit(-5);
        }
    }
}

void create_shared_memory_objects(){
    initialize_shared_memory_variable(SHARED_MEMORY_CONTAINERS_COUNT);
    initialize_shared_memory_variable(SHARED_MEMORY_CONTAINERS_SAFE_AREA_COUNT);
    initialize_shared_memory_variable(SHARED_MEMORY_WHEAT_BAGS_COUNT);
    initialize_shared_memory_variable(SHARED_MEMORY_FAMILY_MAX_STARVATION);
    initialize_shared_memory_variable(SHARED_MEMORY_FAMILY_MAX_STARVATION_PID);
}

int check_if_game_ends(){
    sleep(2);
    time(&current_time);
    double seconds = difftime(current_time, start_time);
    if(seconds >= APP_LENGTH_SECONDS || dead_families >= FAMILIES_PASS_AWAY_THRESHOLD || targeted_planes >= CARGO_PLANES_CRASHED_THRESHOLD || collect_martyers >= COLLECTING_WORKERS_MARTYRS_THRESHOLD || distribute_martyers >= DISTRIBUTION_WORKERS_MARTYRS_THRESHOLD){
        return 1;
    }
    return 0;
}

void kill_all_children(){
    for(int i = 0; i < NUM_OF_FAMILIES; i++){
        kill_process(families[i]);
    }
    for(int i = 0; i < COLLECTING_WORKERS_COMMITTEES; i++){
        kill_process(collecting_workers_committees[i]);
    }
    for(int i = 0; i < SPLITTING_WORKERS_COMMITTEES; i++){
        kill_process(distributing_workers_committees[i]);
    }
    for(int i = 0; i < DISTRIBUTING_WORKERS_COMMITTEES; i++){
        kill_process(distributing_workers_committees[i]);
    }
    for(int i = 0; i < CARGO_PLANES; i++){
        kill_process(cargo_planes[i]);
    }
}

void sigint_handler(int signum) {
    dead_families++;
}

void sigquit_handler(int signum) {
    targeted_planes++;
}

void sigusr1_handler(int signum) {
    collect_martyers++;
}

void sigusr2_handler(int signum) {
    distribute_martyers++;
}

int main(){
    pid_t main_parent_application_id = getpid();
    printf("Parent ID: %d\n", main_parent_application_id);

    send_a_message("");

    while(receive_a_message() != NULL){}

    time(&start_time);

    // Register signal handlers
    if (signal(SIGINT, sigint_handler) == SIG_ERR) {
        perror("signal(SIGINT) failed");
        return 1;
    }
    if (signal(SIGQUIT, sigquit_handler) == SIG_ERR) {
        perror("signal(SIGQUIT) failed");
        return 1;
    }
    if (signal(SIGUSR1, sigusr1_handler) == SIG_ERR) {
        perror("signal(SIGUSR1) failed");
        return 1;
    }
    if (signal(SIGUSR2, sigusr2_handler) == SIG_ERR) {
        perror("signal(SIGUSR2) failed");
        return 1;
    }

    readFile("args.txt");
    //send_a_message("");

    /* Run Opengl */
    int  gui_pid = fork();
    if (gui_pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (gui_pid == 0)
    {
        char ogl_exe[6];
        sprintf(ogl_exe, "./opengl");
        execl(ogl_exe, NULL);
        perror("Execution failed "); // execl will only return if it fails
        exit(EXIT_FAILURE);
    }


    // Create shared memory objects
    create_shared_memory_objects();
    
    /* Creating Needed Processes for the program to run */
    create_collecting_worker_committees();
    create_splitting_worker_committees();
    create_distributing_worker_committees();
    create_families();
    create_cargo_planes();

    while(check_if_game_ends() == 0){}


    // Simulation DONE.
    kill_all_children();
    kill_process(gui_pid);
    
    printf("Program Terminated.\n");

    return 0;
}