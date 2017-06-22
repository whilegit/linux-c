# 注释
# target: prerequisites
# \t command
# target是指目标, prerequisites表示先决条件即编译target所需的目标或文件
#     command是指任意的shell命令,一般是编译命令
# 编译的规则是: prerequisites的任意文件比target新,则执行command
#
# command前必须要有一个Tab字符,如果vim把Tab扩展成空格,则应Ctrl+v禁止此扩展
helloworld: helloworld.c
	gcc -o helloworld helloworld.c
