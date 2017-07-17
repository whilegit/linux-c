/**
 * unistd.h是POSIX操作系统API功能的头文件,应当最先出现
 */
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <stdio.h>

/**
 * 原型函数
 * 1.write 写数据到文件: 
 *
 *    size_z write(int fildes, const void *buf, size_t nbytes)
 * 
 *    写一个字节后,文件指针向后移一位
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
 *-----------------------------------------------------------------------------------------------------------
 * 6. lseek系统调用
 *   
 *    off_t lseek(int fildes, off_t offset, int whence)
 * 
 *    @param filedes 文件描述符
 *　　@param offset  偏移值　可以为负值. off_t类型是与具体实现相关,类型在sys/types.h中定义
 *    @param whence  该参数决定前一个参数offset的含义,可以是如下值之一:
 *                   SEEK_SET: offset是一个绝对位置(从0开始)
 *                   SEEK_CUR: offset是相对于当前位置的一个偏移值
 *                   SEEK_END: offset是相对于文件尾的一个偏移值(length + offset)
 *    @return 返回相对于文件头的绝对偏移值.
 *-----------------------------------------------------------------------------------------------
 * 7. 描述符相关的文件状态信息的系统调用
 *
 *   int fstat(int fildes, struct stat *buf);
 *   int stat(const char *path, struct stat *buf);
 *   int lstat(const char *path, struct stat *buf);
 *   
 *   @param fildes  文件描述符
 *   @param path    文件路径
 *   @param buf     struct stat结构体指针
 *   @return 返回0成功,返回-1失败,错误代码存放于全局变量errno中
 *   @注意 stat(..)调用在遇到符号连接时,继续追查目标文件.
 *         lstat则停止,返回符号文件本身
 * 
 *   struct stat{
 *      mode_t     st_mode;       //文件权限和文件类型信息
 *      ino_t      st_ino;        //与该文件关联的inode
 *      dev_t      st_dev;        //保存文件的设备
 *      dev_t      st_rdev;       
 *      nlink_t    st_nlink;      //该文件硬连接的数量
 *      uid_t      st_uid;        //文件属主的UID号
 *      gid_t      st_gid;        //文件属主的GID号
 *      off_t      st_size;
 *      time_t     st_atime;      //文件上一次被访问的时间
 *      time_t     st_mtime;      //文件内容上一次被修改的时间
 *      time_t     st_ctime;      //文件的权限\属主\组或者内容上一次被修改的时间
 *      blksize_t  st_blksize;    //文件单位块的大小(本机显示是4096,这个可能是位单位)
 *      blkcnt_t   st_blocks;     //文件所占用的块数.文件实际占用磁盘的大小为st_blksize * st_blocks,　该值通常大于st_size.
 *   };
 *   
 *   mode_t st_mode成员的解释
 *   1. mode_t的类型为__U32_TYPE, 也就是unsigned int类型
 *   2. 文件类型相关的掩码S_IFMT是0170000, 位15:12,16进制掩码为0xf000
 *      公共宏原型: #define __S_ISTYPE(mode,mask) (((mode) & __S_IFMT) == (mask))
 *      a). S_IFBLK: 0x6000特殊的块设备标志. 宏S_ISBLK(st_mode)用于测试是否是特殊的块设备.
 *          S_ISBLK(st_mode)的原型为: #define S_ISBLK(mode) __S_ISTYPE((mode), __S_IFBLK)
 *      b). S_IFCHR: 0x2000字符设备标志. 宏S_ISCHR(st_mode)用于测试是否是字符设备(宏定义参照S_ISBLK(mode))
 *      c). S_IFDIR: 0x4000是否为目录. 宏S_ISDIR(st_mode)用于本测试
 *      d). S_IFIFO: 0x1000是否是FIFO设备. 宏S_ISFIFO(st_mode)用于本测试
 *      e). S_IFREG: 0x8000是否为普通文件. 宏S_ISREG(st_mode)用于本测试
 *      f). S_IFLNK: 0xA000是否为符号连接. 宏S_ISLNK(st_mode)用于本测试
 *   3. 文件权限相关的掩码有三个,分别是
 *      a). S_IRWXU(0x01C0,位8:6) 文件属主的读写执行权限
 *          读标志位  S_IRUSR(0x0100,第8位)
 *          写标志位  S_IWUSR(0x0080,第7位)
 *          执行标志位S_IXUSR(0x0040,第6位)
 *      b). S_IRWXG(0x38,位5:3) 文件属组的读写执行权限
 *          读标志位  S_IRGRP(0x0020,第5位)
 *          写标志位  S_IWGRP(0x0010,第4位)
 *          执行标志位S_IXGRP(0x0008,第3位)
 *      c). S_IRWXO(0x07,位2:0) 其他用户的读写执行权限
 *          读标志位  S_IROTH(0x0004,第2位)
 *          写标志位  S_IWOTH(0x0002,第1位) 
 *          执行标志位S_IXOTH(0x0001,第0位)
 *   4. SUID和SGID相关(文件运行时以文件属主或属组的身份运行)
 *      a). S_ISUID(bit 12) 是否设置了SUID
 *      b). S_ISGID(bit 11) 是否设置了SGID
 *--------------------------------------------------------------------------------------------
 *
 * 8. 复制文件描述符 
 *
 *    int dup(int fildes);
 *    int dup2(int fildes, int filedes2);
 *
 *    @return  具体功能未知
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
    /*
    char block[1024];
    int in, out;
    int nread;
    in = open("file.tmp", O_RDONLY);
    out = open("file.dest.tmp", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    while(nread = read(in, block, sizeof(block)) > 0){
        write(out, block, nread);
    }
    close(in);
    close(out);

    out = open("file_seek.tmp", O_WRONLY | O_TRUNC);
    write(out, "0123456789", 10);
    int offset = lseek(out, 4, SEEK_SET);
    write(out, "a", 1);
    offset = lseek(out, 2, SEEK_CUR);
    write(out, "b", 1);
    offset = lseek(out, -2, SEEK_CUR);
    write(out, "c", 1);
    offset = lseek(out, -8, SEEK_END);
    write(out, "d", 1);
    close(out);
    */
    struct stat st;
    int fildes = open("file.tmp", O_RDONLY);
    fstat(fildes, &st);
    printf("%o\n",st.st_mode);
    printf("stat.st_size=%d, stat.st_blksize(%d)*stat.st_blocks(%d)=%d\n", st.st_size, st.st_blksize, st.st_blocks, st.st_blksize*st.st_blocks);
    return 0;
}
