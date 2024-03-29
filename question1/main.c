// C program to demonstrate use of fork() and pipe() 
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h>


int calculateEvenDigits(char  strNum []){
    int num, rem,odd=0,even=0,digit,input;
    sscanf(strNum, "%d",&num);
    input = num;
    num = abs(num);

    while(num>0){
        digit = num % 10;
        num = num / 10;
        rem = digit % 2;
        if(rem != 0)
        odd=odd+digit;
        else
        even=even+digit;
    }

    return even; 
}
  

pid_t concatenate(){    
    
    // We use two pipes 
    // First pipe to send input string from parent 
    // Second pipe to send concatenated string from child 
  
    int fd1[2];  // Used to store two ends of first pipe 
    int fd2[2];  // Used to store two ends of second pipe   
    pid_t p; 
  
    if (pipe(fd1)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 
    if (pipe(fd2)==-1) 
    { 
        fprintf(stderr, "Pipe Failed" ); 
        return 1; 
    } 


  
    p = fork(); 

    // printf("%d", p);
    if (p < 0) 
    { 
        fprintf(stderr, "fork Failed" ); 
        return 1; 
    } 
  
    // Parent process 
    else if (p > 0) 
    { 
        close(fd1[0]);  // Close reading end of first pipe
        char concat_str[100];

  
        // Write input string and close writing end of first 
        // pipe.
        char firstStr[100];
        scanf("%s", firstStr);
        write(fd1[1], firstStr, strlen(firstStr)+1); 
        close(fd1[1]);

        // Wait for child to send a string 
        wait(NULL); 
  
        close(fd2[1]); // Close writing end of second pipe 
  
        // Read string from child, print it and close 
        // reading end. 
        read(fd2[0], concat_str, 100); 
        printf("%s", concat_str); 
        close(fd2[0]); 
    } 
  
    // child process 
    else
    { 
        close(fd1[1]);  // Close writing end of first pipe
  
        // Read a string using first pipe 
        char concat_str[100];
        read(fd1[0], concat_str, 100);

        // Calculate sum of even digits
        int sum = calculateEvenDigits(concat_str);
        char strSum[100];
        sprintf(strSum, "%d\n", sum);
        char secondStr[100] = " The sum of even digits in the input number: ";
  
        // Concatenate a fixed string with it 
        int k = strlen(concat_str); 
        int i; 
        for (i=0; i<strlen(secondStr); i++) 
            concat_str[k++] = secondStr[i];

        k = strlen(concat_str);
        for (i=0; i<strlen(strSum); i++) 
            concat_str[k++] = strSum[i];
  
        concat_str[k] = '\0';   // string ends with '\0' 
  
        // Close both reading ends 
        close(fd1[0]); 
        close(fd2[0]); 
  
        // Write concatenated string and close writing end 
        write(fd2[1], concat_str, strlen(concat_str)+1); 
        close(fd2[1]);
  
        exit(0); 
    }
    return p;
} 


int main() {

        concatenate();
        return 1;
}
