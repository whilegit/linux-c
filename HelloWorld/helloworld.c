//#include <stdio.h>
#include <unistd.h>


int main(int argc, char ** argv){
    write(1, "Hello world\n",12); 
    //printf("Hello World, Linux c\r\n");
    return 0;
}
