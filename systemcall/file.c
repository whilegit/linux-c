/**
 * unistd.h是POSIX操作系统API功能的头文件
 */
#include <stdlib.h>
#include <unistd.h>

/**
 * 原型函数
 * 1.write 写数据到文件: 
 *
 *    size_z write(int fildes, const void *buf, size_t nbytes)
 *
 *    其中fildes为文件描述符编号,已自动打开的三个描述符为:　0标准输入,1标准输出,2标准错误　
 *    返回写入的字节数:
 *       0      未写入任何数据返回.
 *       -1     表示写入错误(查看全局错误码errno),
 *       nbytes 表示正常
 *       小于nbytes 因硬件原因有可能小于nbytes,这可能并不是一个错误
 --------------------------------------------------------------------
 * 2.read 从文件读数据:  
 *
 *    size_t read(int fildes, void *buf, size_t nbytes)
 *
 *    @param buf 输出输冲区 
 *    @param nbytes 试图读取的字节数
 *　　@return size_t 读取到的字节数
 */

int main(int argc, char** argv){
    if( write(1, "Hello world!\n", 13) != 13){
        write(2, "A write error has occurred on file descriptor 1\n", 48);
    } 

    char buffer[12];
    int  nread = read(0, buffer, 12);
    if(nread == -1){
        write(2, "A read error has occurred\n", 26);
        return 1;
    }
    if(write(1, buffer, nread) != nread){
        write(2, "A write error has occurred\n", 27);
        return 2;
    }

    return 0;
}
