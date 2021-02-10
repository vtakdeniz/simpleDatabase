#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h> 
#include <signal.h>
#include <fcntl.h> 
#include <sys/stat.h> 

#define MAXA 100



int main(){
    //Defines the named pipe
    int fd; 

    char * myfifo = "/tmp/myfifo"; 
  
    mkfifo(myfifo, 0666); 
  
    char arr1[MAXA], arr2[MAXA]; 
    
    while (1) 
    { 
        //Opens the pipe and writes the entered value
        printf("Bir sorgu giriniz : ");
        printf("\n");
        
        fd = open(myfifo, O_WRONLY); 
        fgets(arr2, MAXA, stdin); 
        write(fd, arr2, strlen(arr2)+1); 
        close(fd); 
  
        
        //Reads the result from the pipe
        printf("Sonuç : ");
        printf("\n");
        fd = open(myfifo, O_RDONLY); 
  
        read(fd, arr1, sizeof(arr1)); 

        close(fd);   
        //if the value is null program informs the user.
        if (strncmp(arr1,"null",strlen("null"))==0)
        {
            printf("the value is null\n"); 
        }
        
        else{
          printf(arr1); 
          printf("\n");
          printf("program sonucu kaydedilsin mi ? e/h \n");
          char * answer[MAXA];
          fgets(answer, MAXA, stdin); 
          //Based on the answer, calls the kaydet program via fork and writes the result string into a pipe.
           if (strncmp(answer,"evet",strlen("evet"))==0||strncmp(answer,"e",strlen("e"))==0)
           {

            int f = fork();
            int pipefd[2];
            int e=0;
            if (pipe(pipefd) < 0) {
            perror("pipe");
            exit(1);
              }

            if (f==0)
            {
            write(pipefd[1], arr1, strlen(arr1)+1);   
            e = execv("kaydet",NULL);
            printf("başarısız.\n");
             close(pipefd[0]);
             close(pipefd[1]);
            }

            if(f!=0){
            int status;
            wait(&status);
            close(pipefd[0]);
            close(pipefd[1]);
            }
            


           }
           else if(strncmp(answer,"hayir",strlen("hayir"))==0||strncmp(answer,"h",strlen("h"))==0){
            printf("Kayıt işlemi iptal edildi.");
           }
           else{
               printf("Yanlış bir komut girdiniz.");
           }
           
        }

    } 
    return 0;    


}