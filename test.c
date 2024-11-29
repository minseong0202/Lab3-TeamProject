#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define MAX_CMD_LEN 256
#define MAX_ARGS 50

// 명령어를 공백을 기준으로 분리하여 인자 배열로 저장하는 함수
int getargs(char *cmd, char **argv)
{
    int narg = 0;
    while (*cmd)
    {
        if (*cmd == ' ' || *cmd == '\t')
        {
            *cmd++ = '\0'; // 공백은 구분자로 사용
        }
        else
        {
            argv[narg++] = cmd;
            while (*cmd != '\0' && *cmd != ' ' && *cmd != '\t')
            {
                cmd++;
            }
        }
    }
    argv[narg] = NULL; // 마지막에 NULL을 추가하여 execvp에서 인식하게 함
    return narg;
}

int main()
{
    char buf[MAX_CMD_LEN]; // 명령어 입력을 받을 버퍼
    char *argv[MAX_ARGS];  // 인자 배열
    int narg;              // 인자 수
    pid_t pid;             // 프로세스 ID

    while (1)
    {
        printf("shell> ");
        if (fgets(buf, sizeof(buf), stdin) == NULL)
        {
            perror("입력 실패");
            exit(1);
        }

        // 입력 끝의 newline 문자를 제거
        buf[strcspn(buf, "\n")] = '\0';

        // 'exit' 명령어 입력 시 쉘 종료
        if (strcmp(buf, "exit") == 0)
        {
            break;
        }

        narg = getargs(buf, argv);

        // 자식 프로세스를 생성하여 명령어를 실행
        pid = fork();
        if (pid == 0)
        { // 자식 프로세스
            if (execvp(argv[0], argv) == -1)
            {
                perror("execvp 실패");
                exit(1);
            }
        }
        else if (pid > 0)
        {               // 부모 프로세스
            wait(NULL); // 자식 프로세스 종료를 기다림
        }
        else
        { // fork 실패
            perror("fork 실패");
        }
    }

    return 0;
}
