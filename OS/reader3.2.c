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
    printf("The memory is being written, please wait! \n");
   
    char *addr_cpy = shared_mem_addr;

    printf("Crossed it!");

    int overflow = 0;
    while(1){
        
        if(addr_cpy[0] == '$' || addr_cpy[1] == '$' || addr_cpy[2] == '$'){
           
            printf("Please wait! \n");
            
            sleep(3);
            
        } 
        else if(addr_cpy[0] == '^' || addr_cpy[1] == '^' || addr_cpy[2] == '^'){
            
            if(overflow == 0){
                printf("Reading before the segment is overwritten: \n");
                for(int i = 0;i<BUF_SIZE;i++){
                    printf("%c",addr_cpy[i]);
                }
                printf("\n");
                strcpy(addr_cpy,"$$$");
                //overflow++;
            }


        }
        else{
            
            char *tempaddr = (char *)shared_mem_addr;
            //strcpy(addr_cpy,"@@@");
            for(int i =  0;i<=3;i++){
                tempaddr[i] = '@';          //to indicate that reading is being done
            }
            
            printf("The data in the shared emmory segment is given below : \n");
            
            for(int i = 0;i<(BUF_SIZE+20);i++){
                printf("%c",tempaddr[i]);
            }
            printf("\n");
            char reply[10];
            printf("Type STOP to stop reading \n");
            fgets(reply,10,stdin);
            strtok(reply,"\n");

            if(strcmp(reply,"STOP") == 0){
                printf("The reading process has stopped \n ");
                //strcpy(shared_mem_addr,"   ");
                for(int i =0;i<=3;i++ ){
                    tempaddr[i] = ' ';
                }
                printf("The '@' at the beginning shouldve gone now \n");
                for(int i = 0;i<(BUF_SIZE+20);i++){
                    printf("%c",tempaddr[i]);
                }
                //printf("Now the data at the shared segment is :%s",(char *)shared_mem_addr);
                goto end;
            }
        }

    } 
    
    end:
    shmdt(shared_mem_addr);

    
}
