#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<strings.h>
#include<stdlib.h>
#include<sys/ipc.h>     //library for interprocess communication
#include<sys/shm.h>     //library for creating shared memory segments

#define BUF_SIZE 20


int main() 
{
    
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

    int printed = 0;
    memset(shared_mem_addr,1,BUF_SIZE);
    strcpy(addr_cpy,"$$$");                 // copying the indicator dollar must be done only if shared memory segment is being created
    addr_cpy = addr_cpy + 3;



    int waitcount =0;
    rejected:
    while(1)
    {

        if(addr_cpy[0] == '@' || addr_cpy[1] == '@' || addr_cpy[2] == '@') // means reading is being done!
        {
            
            printf("Reading is being done! Please wait! \n");
            sleep(3);

        }

        char inputs[100];
        printf("Enter some data into the buffer. Type 'STOP' to pause writing \n");
        //read(0,inputs,100);
        fgets(inputs,100,stdin);
        strtok(inputs,"\n");
                                                          // writing dollar to indicate that writing is going on
        
        if(strcmp(inputs,"STOP") == 0)
        {
            printf("Stopped the writing process!");
                                                             
            char *tempaddr = shared_mem_addr;               
            for(int i = 0;i<=3;i++){
                tempaddr[i] = ' ';                              // cleaning up the characters there to indicate that the memory space is 

            }
            printf("The total stuff in the buffer is ");
            for(int i = 0;i<(BUF_SIZE+20);i++){
                printf("%c",tempaddr[i]);
            }
            break;                                                 // now open and free for all
        }
        
        if(printed <= BUF_SIZE)
        {
            
            char *tempaddr = shared_mem_addr;
            printf("You wrote this : %s \n",inputs);
            printf("Adress before print %p \n",addr_cpy);
            addr_cpy = addr_cpy + strlen(inputs);
            printed += strlen(inputs);
            strcpy(addr_cpy,inputs);
            //*(addr_cpy + BUF_SIZE) = '\0';
            printf("Adress after print %p \n",addr_cpy);
            //printf("The total stuff in the buffer is %s",tempaddr);
            //printf("shared mem address is %p \n",shared_mem_addr);
            //printf("temp address is %p \n",tempaddr);
            printf("The total stuff in the buffer is :");
            for(int i = 0;i<=(BUF_SIZE+20);i++){
                printf("%c",tempaddr[i]);
            }
            printf("\n");
            printf("Value of printed is %d \n",printed);
        }
        else{
            printf("You wrote this : %s \n",(char*)addr_cpy);
            addr_cpy = (char *)shared_mem_addr;
            printed = 0;
            addr_cpy[0] = '^';      //writing three '^' chars to indicate that the buffer is being overwriten and any reader
            addr_cpy[1] = '^';      //if present should read the contents
            addr_cpy[2] = '^';
            sleep(1);
            //memset(shared_mem_addr,1,BUF_SIZE);
        }
        *(addr_cpy + BUF_SIZE) = '\0';
        


    }
    shmdt(shared_mem_addr);
    //shmctl(shmid,IPC_RMID,NULL);
}

//buffer seemingly had infinite length?!?! even though length specified in
// can write a signal handler for SIGKILL or SIGINT and make it do the same thing as 'STOP'