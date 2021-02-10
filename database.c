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

// Parses the space in given string and returns all the parsed string in a ** list so each command can be taken with it's parameter
void parseSpace(char* str, char** parsed) 
{ 
    int i; 
  
    for (i = 0; i < MAXA; i++) { 
        parsed[i] = strsep(&str, " "); 
  
        if (parsed[i] == NULL) 
            break; 
        if (strlen(parsed[i]) == 0) 
            i--; 
    } 
} 

// Parses the = sign shown as -> "=" and splits strings before and after = and puts them in a ** list.
int parseEq(char* str, char** strpiped) 
{ 
    int i; 
    for (i = 0; i < MAXA; i++) { 
        strpiped[i] = strsep(&str, "="); 
        if (strpiped[i] == NULL) 
            break; 
    } 
  
    if (strpiped[1] == NULL) 
        return 0; 
    else { 
        return 1; 
    } 
} 

//main program that consists only one while loop
int main(){

//defining named pipe    
int fd1; 
  
char * myfifo = "/tmp/myfifo"; 

mkfifo(myfifo, 0666); 
char allwords[MAXA][MAXA];  
char str1[MAXA], str2[MAXA];

while (1) 
{ 
    //reading from the pipe
    fd1 = open(myfifo,O_RDONLY); 
    read(fd1, str1, MAXA); 
    close(fd1); 
  
    char ** parsedText[MAXA];
    parseSpace(str1,parsedText);
    
    //choosing which results to fetch from the result array based on the command.
    int selectIndex=-1;
    int i1= strcmp(parsedText[1],"ad");
    int j1= strcmp(parsedText[1],"number");
    int k1= strcmp(parsedText[1],"*");
    
    if (i1==0)
    {
       selectIndex=0;
    }
    else if(j1==0){
        selectIndex=1;
    }
    else{
        selectIndex=2;
    }
    
    //keyword and search are the parameters crucial to know what are we searching for.
    //keyword is "number" or "ad" which defines what we are looking for in the result array
    //search is the word we are searching in the .txt file
    char * keyword[MAXA];
    char * search[MAXA];
    char * tempKeyword[MAXA];
    strcpy(tempKeyword,parsedText[5]);
    char ** parsedKey[MAXA];
    //parses the string for the equal sign
    parseEq(tempKeyword,parsedKey);
    //assings keyword and search to created char arrays.
    strcpy(keyword,parsedKey[0]);
    strcpy(search,parsedKey[1]);
    
    // choosing which parametres to compare with the given string in the query array in order to find the matching result from the result array.
    int i2= strcmp(keyword,"ad");
    int j2= strcmp(keyword,"number");
    int searchIndex=-1;
    if (i2==0)
    {
        searchIndex=0;
    }
    if (j2==0)
    {
        searchIndex=1;
    }
    
    //picks file name from the query array.
    char file[MAXA];  
    strcpy(file,parsedText[3]);
    

//creates a file pointer to read from the file name fetched above.
//Assigns everything from file to a 2d char array.
FILE* filePointer;
int bufferLength = 255;
char buffer[bufferLength];
char fileContent[MAXA][MAXA];
filePointer = fopen(file, "r");

int counter=0;
while(fgets(buffer, bufferLength, filePointer)) {
    strcpy(fileContent[counter],buffer);
    counter++;
}
fclose(filePointer);

//Iterates over 2d array and picks values based on the search string, select index and the search index.
int wordCount=0;
for (int i = 0; i < counter; i++)
{
    char ** parsedContent[MAXA];
    char * tempcontent[MAXA];
    strcpy(tempcontent,fileContent[i]);
    parseSpace(tempcontent,parsedContent);
   

     int result=-1;
    if (searchIndex==0)
    {
        result= strncmp(parsedContent[searchIndex],search,strlen(parsedContent[searchIndex]));
       
    }
    else if(searchIndex==1)
    {
       int num1 = atoi(parsedContent[searchIndex]);
       int num2= atoi(search);
       if (num1==num2)
       {
           result=0;
       }
    }
    
    if (result==0)
    {
       if (selectIndex==0)
       {
           strcpy(allwords[wordCount],parsedContent[0]);
           wordCount++;
           strcpy(allwords[wordCount],"  ---   ");
           wordCount++;
       }
       if (selectIndex==1)
       {
           strcpy(allwords[wordCount],parsedContent[1]);
           wordCount++;
           strcpy(allwords[wordCount],"  ---   ");
           wordCount++;
       }
       if (selectIndex==2)
       {
          strcpy(allwords[wordCount],fileContent[i]);
           wordCount++;
           strcpy(allwords[wordCount],"  ---   ");
           wordCount++;
       }
       
       
    }
    

}

//Concatenates all the result from 2d array to a normal char array in order to properly send it via named pipe.
char string[MAXA];
for (int i = 0; i < wordCount; i++)
{
    strcat(string,allwords[i]);
} 


//Checks whether the string is empty or not
if (strlen(string)==0)
{
 strcpy(string,"null");
}
    fd1 = open(myfifo,O_WRONLY); 
    write(fd1, string, strlen(string)+1); 
    close(fd1);   
    string[0]=0; 

} 
    return 0; 
}