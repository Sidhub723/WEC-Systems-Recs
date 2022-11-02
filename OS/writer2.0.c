#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/ipc.h>     //library for interprocess communication
#include<sys/shm.h>     //library for creating shared memory segments

#define BUF_SIZE 20


int main() 
{
    //void *shared_mem_addr;
    //char inputs[100];
    //int shmid;
    //key_t keyy = ftok("/home/pingpong1/Desktop/VSC/fff.txt",'b');
    //shmid = shmget((key_t)1234,BUF_SIZE,0666|IPC_CREAT);     //creating a segment and getting its id
    //shared_mem_addr = shmat(shmid,NULL,0);      //returns the address space/adress of the segment where the segment gets attached to the prcess
    //printf("ID if shared memory is %d \n",shmid);
    //printf("Address of shared memory is %p \n",shared_mem_addr);
    //int failedid = shmget((key_t)1234,BUF_SIZE,0666|IPC_EXCL|IPC_CREAT);
    //printf("Failed id if %d",failedid);
    //char *addr_cpy = (char *)shared_mem_addr;
    //int printed = 0;
    //memset(shared_mem_addr,1,BUF_SIZE);
    //strcpy(addr_cpy,"$$$");                 //adds 3 dollar signs at the beginning of the memory segment to show that it is in use
    //addr_cpy = addr_cpy + 3;                // and no other process can access it
                                            // it is all under the assumption that this is the very writer which created the shared memory
                                            // segment, there can be other writers which simply use this segment, NOT create it
                                            // write a writer process which simply writer to an already created shared memeory segment
                                            // or maybe, as a writer, check if a shared memeory segment exists, if not create it, if it does 
                                            // them simply write to it.
    //printf("The dollar signs are visible or not??? %s",(char *)shared_mem_addr);
    void *shared_mem_addr;
    char inputs[100];
    int shmid;
    char *addr_cpy;
    //key_t keyy = 1234;
    shmid = shmget((key_t)1234,BUF_SIZE,0666|IPC_CREAT|IPC_EXCL);
    shared_mem_addr = shmat(shmid,NULL,0);
    addr_cpy = (char *)shared_mem_addr;
    if(shmid == -1){
        printf("The memory segment already exists, shmid is %d \n",shmid);
        shmid = shmget((key_t)1234,BUF_SIZE,0666|IPC_CREAT);
        shared_mem_addr = shmat(shmid,NULL,0);
        addr_cpy = (char *)shared_mem_addr;
        printf("Now proper shmid is %d \n",shmid);
    }
    else {
        strcpy(addr_cpy,"$$$");      // if the segment is created succesfully, them immediatrly add our semaphore $$$ to it            
        addr_cpy = addr_cpy + 3;     // to indicate that it is under use
    }
    
    printf("ID if shared memory is %d \n",shmid);
    printf("Address of shared memory is %p \n",shared_mem_addr);

    //char *addr_cpy = (char *)shared_mem_addr;
    int printed = 0;
    memset(shared_mem_addr,1,BUF_SIZE);
    //strcpy(addr_cpy,"$$$");                 // copying the indicator dollar must be done only if shared memory segment is being created
    //addr_cpy = addr_cpy + 3;



    int waitcount =0;
    rejected:
    while(1)
    {
    
        if((char *)shared_mem_addr == '@' && (char *)(shared_mem_addr+1) == '@' && (char *)(shared_mem_addr+2) == '@'  ) // means reading is being done!
        {
            if(waitcount == 0 || waitcount == 1 || waitcount == 2){
                printf("Reading is being done, please wait!");
            }
            waitcount++;
            
            
            goto rejected;

        }

        char inputs[100];
        printf("Enter some data into the buffer. Type 'STOP' to pause writing \n");
        //read(0,inputs,100);
        fgets(inputs,100,stdin);
        strtok(inputs,"\n");
        strcpy(shared_mem_addr,"$$$");                         // writing dollar to indicate that writing is going on
        addr_cpy = addr_cpy + 3;
        if(strcmp(inputs,"STOP") == 0)
        {
            printf("Stopped the writing process!");
            strcpy((char *)shared_mem_addr,"   ");                         // cleaning up the characters there to indicate that the memory space is 
            char *tempaddr = shared_mem_addr;               // MAKE ONLY THE FIRST THREE CHARACTERS INTO NON '$' AND REST SHALL BE FINE
            
            printf("The total stuff in the buffer is ");
            for(int i = 0;i<BUF_SIZE;i++){
                printf("%c",tempaddr[i]);
            }
            break;                                                 // now open and free for all
        }
        else if(printed <= BUF_SIZE)
        {
            strcpy(addr_cpy,inputs);
            char *tempaddr = shared_mem_addr;
            printf("You wrote this : %s \n",(char*)addr_cpy);
            printf("Adress before print %p \n",addr_cpy);
            addr_cpy = addr_cpy + strlen(inputs);
            printed += strlen(inputs);
            *(addr_cpy + BUF_SIZE) = '\0';
            printf("Adress after print %p \n",addr_cpy);
            printf("The total stuff in the buffer is %s",tempaddr);
            printf("\n");
            printf("Value of printed is %d \n",printed);
        }
        else{
            printf("You wrote this : %s \n",(char*)addr_cpy);
            addr_cpy = (char *)shared_mem_addr;
            printed = 0;
            addr_cpy[0] = '^';
            addr_cpy[1] = '^';
            addr_cpy[2] = '^';
            sleep(1);
            memset(shared_mem_addr,1,BUF_SIZE);
        }
        *(addr_cpy + BUF_SIZE) = '\0';
        


    }
    shmdt(shared_mem_addr);
    //shmctl(shmid,IPC_RMID,NULL);
}

//buffer seemingly had infinite length?!?! even though length specified in
// can write a signal handler for SIGKILL or SIGINT and make it do the same thing as 'STOP'