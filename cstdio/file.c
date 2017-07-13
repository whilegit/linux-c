#include <stdio.h>
#include <string.h>
/**********************************************
 * 0. stdio.h概述
 * 　当程序打开时，有三个流是自动打开的, stdin, stdout和stderr，对应
 * 　文件描述符的0,1,2
 * --------------------------------------------------------------------
 * 1. 打开文件
 *
 * FILE *fopen(const char *filename, const char *mode);
 *
 * @comment fopen(...)对应open系统调用,如果对要对设备进行明确控制,
 *　　　　　并不受stdio的输入/输出缓冲区的限制,请使用open系统调用.
 *          与文件描述符一样，可用的文件流数量也是有限的
 * @param filename 文件路径
 * @param mode     读写模式
 *    "r"或"rb":   以只读方式打开文件
 *    "w"或"wb":   以只写方式打开文件,并把文件长度截短为零
 *    "a"或"ab":   以只写方式打开文件,新内容追加到文件尾
 *    "r+"或"rb+"或"r+b": 以修改方式打开文件(读和写)
 *    "w+"或"wb+"或"w+b": 以修改方式打开文件,并把文件长度截短为零
 *    "a+"或"ab+"或"a+b": 以修改方式打开文件,新内容追加在文件尾
 *    字母b表示二进制文件,在Linux中不区分文件是文本文件还是二进制文件,
 *　　　　所以此处b并无特别作用.
 *        用a或者w打开文件时,若文件不存在则会新建文件.
 *        用a或者a+方式打开文件,文件原有部分可能会受到保护(fseek函数可能无效)
 * @return 成功: 关联一个文件流,并返回一个FILE结构体指针
 *         失败: 返回NULL
 *
 *---------------------------------------------------------------------------
 * 2. 读取文件
 *  
 * size_t fread(void *ptr, size_t size, size_t nitems, File *stream);
 *
 * @param ptr    读取的缓冲区,任意类型
 * @param size   每个数据记录的长度
 * @param ntimes 数据记录计数器
 * @param stream FILE流
 * @return 返回的记录个数(如果文件提前结束，该值可能小于nitems.) 
 * @comment  实际从文件读取的字节数是size * nitems，也可能比这个要小一些
 *
 * -------------------------------------------------------------------------
 * 3. 写数据到文件
 *
 * size_t fwrite(const void *ptr, size_t size, size_t nitems, FILE *stream);
 *
 * @param ptr    准备写入文件的缓冲区
 * @param size   每个数据记录的长度
 * @param nitems 数据计录的个数
 * @param stream FILE流 
 * @return 返回成功写入的记录个数
 * 
 * @备注: 如果是ptr中存放结构化数据，持久化到文件时可能不具备移植性
 * 　　　　原因是int的大小端问题和struct空隙等
 * @备注: 在Linux里，文本文件的末尾都应该添加一个\n换行符。
 * 　　　 所以每次vim编辑结束后，都自动地在文末添加一个\n换行符。
 *        发现这个现象的原因是vim后的文件字节数比预期的要大几个字节。
 *        fwrite不会自动添加\n换行符，文件的字节数符合预期。
 * ------------------------------------------------------------------------
 * 4. 关闭文件
 *
 * int fclose(FILE * stream);
 *
 * @param stream FILE流
 * @return 成功0 失败-1
 * @comment stdio库会对输入输出的数据进行缓存,如果程序需要确保数据已全部写出,就应该调用fclsose()
 *          和序结束时,会自动对所有已经打开的文件流调用fclose(...),但这样就没有机会检查fclose(..)报告的错误
 *          fclose(...)调用时隐含一次fflush()调用
 *--------------------------------------------------------------------------------------------------------
 * 5. 刷出缓存
 *
 * int fflush(FILE *stream);
 *
 * @comment 把文件流里的所有未写出数据立即写出.
 *
 *--------------------------------------------------------------------------------
 * 6. 设置文件指针
 * 
 *   int fseek(FILE *stream, long int offset, int whence);
 *   
 *  @param offset 指针偏移值
 *  @param whence 定位方式 可以使用SEEK_SET, SEEK_CUR, SEEK_END
 *  @return 成功返回0,失败返回-１并设置errno指出错误.这一点区别于lseek().
 *  @comment 参见系统调用lseek(int fildes, off_t offset, int whence). 其中参数部分两函数是等价的.
 *           如果文件的打开方式不是r+,fseek(...)函数重设文件指针可能会无效.
 *********************************************/
void fopen_test(){
   FILE* f = fopen("file.tmp","r"); 
   char buf[1024];
   memset(buf, '*', 32);
   int len = fread(buf, 1, 10, f);
   buf[10] = 0x00;
   printf("%d bytes read\n", len);
   printf("%s\n", buf);
   fclose(f);

   f = fopen("file.tmp", "r+");
   fwrite("ABCDEFG\n",1,8,f);
   fseek(f, -8, SEEK_END);
   fwrite("!00",1,3,f);
   fflush(f);
   scanf("Hello %d", &len);
   fclose(f);   
}
int main(){
    fopen_test();
    return 0;
}
