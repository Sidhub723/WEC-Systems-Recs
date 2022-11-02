#include<stdio.h>
#include<strings.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<stdlib.h>
#include<string.h>

#define BUF_SIZE 20

int main()
{   
    void *shared_mem_addr;
    char inputs[100];
    int shmid;
    shmid = shmget((key_t)1234,BUF_SIZE,0666);
    printf("ID of shared memory segment is %d \n",shmid);
    shared_mem_addr = shmat(shmid,NULL,0);
    printf("The address of the memory segment is %p \n",shared_mem_addr);
    
   
    char *addr_cpy = shared_mem_addr;
    
    printf("Crossed it!");



    while(1){
        char *addr_cpy = shared_mem_addr;
        
        
        if(addr_cpy[0] == '$' || addr_cpy[1] == '$' || addr_cpy[2] == '$'){
            //repeat:
            printf("The memory is being written, please wait! \n");
            
            sleep(3);
            //goto repeat;
            //break;
        } 
        else if(addr_cpy[0] == '^' || addr_cpy[1] == '^' || addr_cpy[2] == '^'){
            printf("Reading before the segment is overwritten: \n");
            for(int i = 0;i<BUF_SIZE;i++){
                printf("%c",addr_cpy[i]);
            }


        }
        else{
            

            strcpy(addr_cpy,"@@@");
            char reply[10];
            printf("The data in the shared emmory segment is given below : \n");
            char *tempaddr = (char *)shared_mem_addr;
            for(int i = 0;i<BUF_SIZE;i++){
                printf("%c",tempaddr[i]);
            }
            printf("\n");
            printf("Type STOP to stop reading \n");
            fgets(reply,10,stdin);
            strtok(reply,"\n");

            if(strcmp(reply,"STOP") == 0){
                printf("The reading process has stopped");
                strcpy(shared_mem_addr,"   ");
                for(int i = 0;i<BUF_SIZE;i++){
                    printf("%c",tempaddr[i]);
                }
                
                goto end;
            }
        }

    } 
    
    end:
    shmdt(shared_mem_addr);

    
}


