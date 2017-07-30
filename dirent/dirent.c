#include <unistd.h>
#include <stdio.h>
#include <dirent.h>

#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
/**目录流库 
 * 1. opendir函数获取目录流
 * 
 *  #include <sys/types.h>
 *  #include <dirent.h>
 *  DIR *opendir(const char *name);
 *
 *  @comment 失败时返回null，目录流以底层文件描述符访问目录本身.
 *  @param name 接受相对路径和绝对路径，相对路径的参考值为当前工作路径(参考getcwd函数和chdir函数)
 *---------------------------------------------------------------------
 * 2. readdir读取目录流DIR的下一个目录项
 *
 *   #include <sys/types.h>
 *   #include <dirent.h>
 *   struct dirent *readdir(DIR *dirp);
 *
 *  @comment 从目录流中读取下一个目录项,若发生错误或到达目录尾，则返回null
 *  @comment dirent结构体的定义：
 *  　　　　 struct dirent {
 *              ino_t d_ino;       //文件的inode节点号
 *              char d_name[256];  //文件名
 *              ...
 *           };
 *  @comment 如果进一步区分目录项是文件还是目录，需要使用stat系统调用.
 *------------------------------------------------------------------------------
 * 3. telldir 获取目录流里的当前目录位置
 *     seekdir 设置当前的目录流位置
 *  
 *   #include <sys/types.h>
 *   #include <dirent.h>
 *   long int telldir(DIR *dirp);
 *   void seekdir(DIR *dirp, long int loc);
 *
 *   @comment  telldir获取当前的目录项位置，seekdir设置当前的目录项位置
 *-------------------------------------------------------------------------------
 * 4. closedir 关闭目录统
 * 
 *   #include <sys/types.h>
 *   #include <dirent.h>
 *   int closedir(DIR *dirp);
 *   
 *   @comment 成功返回0, 失败返回-1
 */

void printdir(const char *dir, int depth){
    //目录流
    DIR *dp;

    //若无法打开目录流则返回
    if((dp = opendir(dir)) == NULL){
        fprintf(stderr, "cannot open directory: %s\n", dir);
        return;
    }

    //改变当前的工作目录，进入需要遍历的目录。这样递归调用时，就不需要拼接目录路径
    chdir(dir);

    //目录项指针
    struct dirent *entry; 
    while((entry = readdir(dp)) != NULL){
        //获取目录项的文件状态结构体，以判断是文件还是目录
        struct stat statbuf;
        lstat(entry->d_name, &statbuf);
        if(S_ISDIR(statbuf.st_mode)){
           //忽略.和..和.git目录
	   if(strcmp(".", entry->d_name) == 0 || strcmp("..", entry->d_name) == 0 || strcmp(".git", entry->d_name) == 0){
	       continue;
           }
           //printf的格式%*s对应两个参数　＊表示域宽对应一个整型参数  s对应一个字符串, 若字符串的长度不足*参数给定的长度, 以空格补充
	   printf("%*s%s/\n", depth, "", entry->d_name);
	   printdir(entry->d_name, depth+4);
        } else {
	   printf("%*s%s\n", depth, "", entry->d_name);
        }
    }
    //重新回到上一级目录，这是与chdir(dir)合用的递归技巧
    chdir("..");
    //关闭当前目录流
    closedir(dp);
}

/**
 * 遍历一个目录
 */
int main(int argc, char **argv){
    if(argc != 2) {
        fprintf(stderr, "need a dir to display");
 	return 1;
    }
    
    printf("Directory scan of %s:\n", argv[1]);
    printdir(argv[1], 0);
    printf("done. \n");

    return 0;
}
