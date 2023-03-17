#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>

int main() {
	char str[1024];
	pid_t pid=0;
	int fd[2];
	if(pipe(fd)==-1){
		perror("pipe error");
		exit(1);
	}
	pid = fork();
	if(pid<0){
		perror("fork error");
		exit(1);
	}
	else if(pid==0){
        //child
		close(fd[0]);
		close(STDOUT_FILENO);
		int fp = open("a.txt", O_RDWR | O_CREAT);
        lseek(fp, 0, SEEK_SET);
		system("ls -al");
        lseek(fp, 0, SEEK_SET);
		int len = read(fp, str, sizeof(str));
        write(fd[1], str, len);
		close(fp);
        close(fd[1]);
	}
	else {
        //father
		close(fd[1]);
        int len = read(fd[0], str, sizeof(str));
        str[len]='\0';
        write(STDOUT_FILENO, str, len);
        close(fd[0]);
	}
    return 0;
}