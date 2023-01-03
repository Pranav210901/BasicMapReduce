Team Member Names: Pranav Pokhrel, Ty Nguyen
X500s: pokhr013, nguy3976

The purpose of mapping and reducing for our program is to get divide the test input file into segments (<key,value> pairs), arrange the order of these pairs and sort them based on their key values.

The program is easy to compile:
    - compile using provided makefile: $ make
    - after which a simple ./mapreduce #mappers #reducers inputfile would be the streamlined commandline call. 
    -An explanation would be- the arguments passed are the number of mappers and reducers along with the file being mapreduced. 

mapper.c:
    - Reads the input file
    - map() uses emit() to insert <key,'1'> pairs into intermediateDS
    - writeintermediateDS writes them out to output/MapOut/Map_mapperID.txt
mapreduce.c:
    - Uses fork() and exec() to spawn mappers and reducers
    - Allows for execution of mapper and reducer processes
    - Uses shuffle to send <key,value> pairs to reducer using hashfunction()
    - Waits for child processes to terminate
reduce.c:
    - reduce() Reads mapper's created word.txt files
    - Counts the number of repititions of 'word' in word.txt by searching for 1's
    - Inserts them into a node of finalKeyValueDS
    - writeFinalDS() writes them out to output/ReduceOut/Reduce_reducerID.txt

Mapper - Ty
Reducer - Pranav
We helped each other wherever we encountered errors