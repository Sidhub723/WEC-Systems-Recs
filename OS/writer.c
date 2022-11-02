#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/ipc.h>     //library for interprocess communication
#include<sys/shm.h>     //library for creating shared memory segments

#define BUF_SIZE 20


int main() 
{
    void *shared_mem_addr;
    //char inputs[100];
    int shmid;
    //key_t keyy = ftok("/home/pingpong1/Desktop/VSC/fff.txt",'b');
    shmid = shmget((key_t)1234,BUF_SIZE,0666|IPC_CREAT);     //creating a segment and getting its id
    shared_mem_addr = shmat(shmid,NULL,0);      //returns the address space/adress of the segment where the segment gets attached to the prcess
    printf("ID if shared memory is %d \n",shmid);
    printf("Address of shared memory is %p \n",shared_mem_addr);

    char *addr_cpy = (char *)shared_mem_addr;
    int printed = 0;
    memset(shared_mem_addr,1,BUF_SIZE);
    strcpy(addr_cpy,"$$$");                 //adds 3 dollar signs at the beginning of the memory segment to show that it is in use
    addr_cpy = addr_cpy + 3;                // and no other process can access it
                                            // it is all under the assumption that this is the very writer which created the shared memory
                                            // segment, there can be other writers which simply use this segment, NOT create it
                                            // write a writer process which simply writer to an already created shared memeory segment
                                            // or maybe, as a writer, check if a shared memeory segment exists, if not create it, if it does 
                                            // them simply write to it.
    //printf("The dollar signs are visible or not??? %s",(char *)shared_mem_addr);

    while(1)
    {
    
        char inputs[100];
        printf("Enter some data into the buffer. Type 'PAUSE' to pause writing \n");
        //read(0,inputs,100);
        fgets(inputs,100,stdin);
        strtok(inputs,"\n");
        if(strcmp(inputs,"PAUSE") == 0)
        {
            printf("Stopped the writing process!");
            break;
        }
        if(printed <= BUF_SIZE)
        {
            strcpy(addr_cpy,inputs);
            printf("You wrote this : %s \n",(char*)addr_cpy);
            printf("Adress before print %p \n",addr_cpy);
            addr_cpy = addr_cpy + strlen(inputs);
            printed += strlen(inputs);
            printf("Adress after print %p \n",addr_cpy);
            printf("The total stuff in the buffer is %s",(char *)shared_mem_addr);
            printf("\n");
            printf("Value of printed is %d \n",printed);
        }
        else{
            printf("You wrote this : %s \n",(char*)addr_cpy);
            addr_cpy = (char *)shared_mem_addr;
            printed = 0;
            memset(shared_mem_addr,1,BUF_SIZE);
        }
        


    }
    shmdt(shared_mem_addr);
    //shmctl(shmid,IPC_RMID,NULL);
}

//buffer seemingly had infinite length?!?! even though length specified in