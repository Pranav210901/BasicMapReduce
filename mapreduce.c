#include "mapreduce.h"

int main(int argc, char *argv[]) {
    
    if(argc < 4) {
        printf("Less number of arguments.\n");
        printf("./mapreduce #mappers #reducers inputFile\n");
        exit(0);
    }

    // ###### DO NOT REMOVE ######
    int nMappers     = strtol(argv[1], NULL, 10);
    int nReducers     = strtol(argv[2], NULL, 10);
    char *inputFile = argv[3];
    int pids [nMappers];
    char num [10];

    if (nReducers > nMappers)
    {
        printf ("Error: Mappers spawned must be greater than or equal to Reducers spawned. \n");
        return 0;
    }

    // ###### DO NOT REMOVE ######
    bookeepingCode();

    // ###### DO NOT REMOVE ######
    pid_t pid = fork();
    if(pid == 0){
        //send chunks of data to the mappers in RR fashion
        sendChunkData(inputFile, nMappers);
        exit(0);
    }
    sleep(1);


    // To do
    // spawn mappers processes and run 'mapper' executable using exec

    for (int i = 1; i <= nMappers; i++){
        pids[i] = fork();
        sprintf (num, "%d", i);

        if (pids[i] == -1){
            printf ("Error creating process\n");
        }

        else if (pids[i] == 0){                                // Child process, exec
            execl ("mapper", "mapper", num, NULL);
        }
    }

    // To do
    // wait for all children to complete execution

    for (int i = 1; i <= nMappers; i++){
        wait (NULL);
    }

    // ###### DO NOT REMOVE ######
    // shuffle sends the word.txt files generated by mapper 
    // to reducer based on a hash function
    pid = fork();
    if(pid == 0){
        shuffle(nMappers, nReducers);
        exit(0);
    }
    sleep(1);


    // To do
    // spawn reducer processes and run 'reducer' executable using exec

    for (int i = 1; i <= nReducers; i++){
        pids[i] = fork();
        sprintf (num, "%d", i);

        if (pids[i] == -1){
            printf ("Error creating process\n");
        }

        else if (pids[i] == 0){                                        // Child process, spawn reducer
            execl ("reducer", "reducer", num, NULL);
        }
    }

    // To do
    // wait for all children to complete execution

    for (int i = 0; i < nReducers; i++){
        wait (NULL);
    }

    return 0;
}
