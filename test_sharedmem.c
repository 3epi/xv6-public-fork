#include "types.h"
#include "stat.h"
#include "user.h"
#include "ipc.h"
// Simple struct for shared factorial data
struct shared_data {
    int current_n;
    int factorial;
    int sync_flag;
};

int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf(1, "Usage: test_sharedmem <num_processes> <n>\n");
        exit();
    }

    int num_processes = atoi(argv[1]);
    int final_n = atoi(argv[2]);

    // Use IPC_CREAT so get_sharedmem doesn't fail
    int shmid = get_sharedmem(1234, sizeof(struct shared_data), 06 | IPC_CREAT);
    if(shmid < 0) {
        printf(1, "get_sharedmem returned: %d\n", shmid);
        printf(1, "get_sharedmem failed\n");
        exit();
    }

    // Parent attaches to initialize shared memory
    struct shared_data *pData = (struct shared_data*)open_sharedmem(shmid, 0, 0);
    if(pData == (void*)-1) {
        printf(1, "Parent open_sharedmem failed\n");
        exit();
    }

    // Initialize
    pData->current_n = 1;
    pData->factorial = 1;
    pData->sync_flag = 0;

    // Fork children
    int pid;
    int process_id = 0;
    for(int i=0; i<num_processes; i++){
        if((pid = fork()) == 0){
            process_id = i+1;
            break;
        }
    }

    // Child code
    if(pid == 0){
        // Each child re-attaches the shared memory
        struct shared_data *cData = (struct shared_data*)open_sharedmem(shmid, 0, 0);
        if(cData == (void*)-1) {
            printf(1, "Child %d open_sharedmem failed\n", process_id);
            exit();
        }
        printf(1, "Child %d started\n", process_id);

        while(1){
            // Wait for this child's turn
            while(cData->sync_flag != process_id)
                sleep(1);

            // Check completion
            if(cData->current_n >= final_n){
                cData->sync_flag = (process_id % num_processes)+1;
                // Child detaches before exit
                close_sharedmem(cData);
                exit();
            }

            // Calculate next factorial
            cData->factorial *= (cData->current_n + 1);
            cData->current_n++;
            printf(1, "Child %d: factorial(%d)=%d\n", 
                   process_id, cData->current_n, cData->factorial);

            // Hand off to next child
            cData->sync_flag = (process_id % num_processes)+1;
        }
    }
    // Parent code
    else {
        sleep(5);
        pData->sync_flag = 1; // Start calculation
        for(int i=0; i<num_processes; i++){
            wait();
        }
        printf(1, "Final: factorial(%d)=%d\n", final_n, pData->factorial);
        // Parent detaches
        close_sharedmem(pData);
    }

    exit();
}