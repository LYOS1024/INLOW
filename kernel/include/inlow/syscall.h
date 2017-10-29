#ifndef _INLOW_SYSCALL_H
#define _INLOW_SYSCALL_H

#define SYSCALL_EXIT 0
#define SYSCALL_WRITE 1
#define SYSCALL_READ 2
#define SYSCALL_MMAP 3
#define SYSCALL_MUNMAP 4
#define SYSCALL_OPENAT 5
#define SYSCALL_CLOSE 6
#define SYSCALL_REGFORK 7
#define SYSCALL_EXECVE 8
#define SYSCALL_WAITPID 9
#define SYSCALL_FSTATAT 10
#define SYSCALL_READDIR 11
#define SYSCALL_NANOSLEEP 12
#define SYSCALL_TCGETATTR 13
#define SYSCALL_TCSETATTR 14

#define NUM_SYSCALLS 15

#endif
