#include <stdio.h>
#include <string.h>
/**********************************************
 *
 * FILE *fopen(const char *filename, const char *mode);
 * @comment fopen(...)对应open系统调用,如果对要对设备进行明确控制,
 *　　　　　并不受stdio的输入/输出缓冲区的限制,请使用open系统调用.
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
 * @return 成功: 关联一个文件流,并返回一个FILE结构体指针
 *         失败: 返回NULL
 *
 *********************************************/
void fopen_test(){
   FILE* f = fopen("fopen.tmp","r"); 
   char buf[1024];
   memset(buf, '*', 32);
   int len = fread(buf, 1, 10, f);
   //buf[10] = 0x00;
   printf("%d bytes read\n", len);
   printf("%s\n", buf);
    
}
int main(){
    fopen_test();
    return 0;
}
