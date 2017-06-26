/**
 * unistd.h是POSIX操作系统API功能的头文件,应当最先出现
 */
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 

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
 *       nbytes 表示正常 *       小于nbytes 因硬件原因有可能小于nbytes,这可能并不是一个错误
 *
 *--------------------------------------------------------------------
 * 2.read 从文件读数据:  
 *
 *    size_t read(int fildes, void *buf, size_t nbytes)
 *
 *    @param buf 输出输冲区 
 *    @param nbytes 试图读取的字节数
 *　　@return size_t 读取到的字节数,如发生错误则返回-1
 *
 *-------------------------------------------------------------------
 * 3. open系统调用 
 *   
 *    int open(const char *path, int oflags);
 *    int open(const char *path, int oflags, mode_t mode);
 *
 *    @param path: 路径
 *    @param oflags: 文件访问模式
 *                   O_RDONLY,O_WRONLY,O_RDWR(此三者必须有一)
 *                   O_APPEND(追加), O_TRUNC(截取), O_CREAT(创建), O_EXCL(排他)
 *    @param mode: 文件的访问权限请求(还需与系统变量umask取and操作,最终的文件访问权限可能小于请求的文件访问权限)
 *                 如果文件访问模式中存在O_CREAT创建文件,则需加此此参数. 
 *                      S_IRUSR(S_IWUSR, S_IXUSR): 读(写,执行)权限,文件属主
 *                      S_IRGRP(S_IWGRP, S_IXGRP): 读(写,执行)权限,文件所属组
 *                      S_IROTH(S_IWOTH, S_IXOTH): 读(写,执行)权限,其他用户
 *    @return: 返回文件描述符,如错误则返回-1
 *    @description: 如果多个进程同时打一个文件,那么这些进程彼此不知道对方的存在,各写各的,相互覆盖。
 *--------------------------------------------------------------------------------
 * 4. close系统调用
 *   
 *    int close(int fildes)
 *
 *    @param fildes 文件描述符
 *    @return 返回0成功,返回-1表示发生错误
 *    @description 关闭后文件描述符释放并能够被重新使用. 有些网络文件系统,可能不会在关闭前报告之前写操作的错误.
 *-------------------------------------------------------------------------------
 * 5. ioctl系统调用
 *   
 *    int ioctl(int fildes, int cmd, ...)
 *    
 *    @param cmd 操作码 
 *    @description: 硬件的特殊接口.
 *    
 */


int main(int argc, char** argv){
    /*
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

    int fildes = open("myfile", O_CREAT | O_WRONLY, S_IRUSR | S_IXOTH);
    if(fildes == -1) write(1, "create myfile failed", 21);
    else write(fildes, "myfile opened", 14);

    close(fildes);

    write(1, "文件Copy测试", 17);

    */
    char block[1024];
    int in, out;
    int nread;
    in = open("file.tmp", O_RDONLY);
    out = open("file.dest.tmp", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    while(nread = read(in, block, sizeof(block)) > 0){
        write(out, block, nread);
    }

    return 0;
}
