#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/ipc.h>     //library for interprocess communication
#include<sys/shm.h>     //library for creating shared memory segments

#define BUF_SIZE 20

int main()
{
    /*void *shared_mem_addr;
    char inputs[100];
    int shmid;
    //key_t keyy = ftok("/home/pingpong1/Desktop/VSC/fff.txt",'b');
    shmid = shmget((key_t)1234,20,0666);       //we simply get the id of the segment which was initially created by writer  
    shared_mem_addr = shmat(shmid,NULL,0);     
    printf("ID if shared memory is %d \n",shmid);
    printf("Address of shared memory is %p \n",shared_mem_addr);    
    printf("hello?");
    printf("The data in the shared memory segment is : %s",(char *)shared_mem_addr);*/

    void *shared_mem_addr;
    char inputs[100];
    int shmid;
    //key_t keyy = ftok("/home/pingpong1/Desktop/VSC/fff.txt",'b');
    shmid = shmget((key_t)1234,20,0666);       //we simply get the id of the segment which was initially created by writer  
    shared_mem_addr = shmat(shmid,NULL,0);     
    printf("ID if shared memory is %d \n",shmid);
    printf("The data in the shared memory segment is :%s",(char *)shared_mem_addr );

    int waitcount = 0;
    char *addr_cpy = (char *)shared_mem_addr;

    while(1)
    {
        if(*addr_cpy == '$' && *(addr_cpy+1) == '$' && *(addr_cpy+2) == '$'  )
        {
            if(waitcount == 0 || waitcount == 1 || waitcount == 2)
            {
                printf("The memory segment is being written, please wait!");
                waitcount++;
            }

        }
        else
        {
            //*shared_mem_addr = '@';
            //*(shared_mem_addr+1) = '@';
            //*(shared_mem_addr+2) = '@';     //added @ characters in the beginning to tell other processes that reading is being done
            strcpy(shared_mem_addr,"@@@");
            char reply[10];
            printf("The data in the shared memory segment is : %s",(char *)shared_mem_addr);
            printf("\n");
            printf("Type STOP to stop reading");
            fgets(reply,10,stdin);
            
            strtok(reply,"\n");

            if(strcmp("STOP",reply) == 0)
            {
                printf("The reading process has stopped \n");
                strcpy(shared_mem_addr,"   ");
                goto end;
                
            }

            //break;


        }
    }

    end:


    shmdt(shared_mem_addr);
}
