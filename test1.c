#include <stdio.h>
#include <unistd.h>

int main()
{
	int pipefd[2];
	pid_t pid;
	char buf;

	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return 1;
	}

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return 1;
	}

	if (pid == 0)
	{
		// 子进程: 从管道读取数据
		close(pipefd[1]); // 关闭写端
		while (read(pipefd[0], &buf, 1) > 0)
		{
			write(STDOUT_FILENO, &buf, 1);
		}
		write(STDOUT_FILENO, "\n", 1);
		close(pipefd[0]);
		_exit(0);
	}
	else
	{
		// 父进程: 向管道写入数据
		close(pipefd[0]); // 关闭读端
		write(pipefd[1], "Hello, world!\n", 14);
		close(pipefd[1]); // 关闭写端，产生EOF
		wait(NULL);		  // 等待子进程退出
	}
	return 0;
}
