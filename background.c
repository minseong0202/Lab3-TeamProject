#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_CMD_LEN 1024
#define MAX_ARGS 100

void execute_command(char *cmd) {
    char *args[MAX_ARGS];
    char *token;
    int i = 0;
    int background = 0;

    if (cmd[strlen(cmd) - 1] == '&') {
        background = 1;
        cmd[strlen(cmd) - 1] = '\0';
    }

    token = strtok(cmd, " ");
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    pid_t pid = fork();
    if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("execvp 실패");
            exit(1);
        }
    } else if (pid > 0) {
        if (background) {
            printf("백그라운드에서 실행 중: PID %d\n", pid);
        } else {
            waitpid(pid, NULL, 0);
        }
    } else {
        perror("fork 실패");
    }
}

int main() {
    char cmd[MAX_CMD_LEN];

    while (1) {
        printf("myshell> ");
        if (fgets(cmd, sizeof(cmd), stdin) == NULL) {
            perror("fgets 실패");
            exit(1);
        }

        cmd[strcspn(cmd, "\n")] = 0;

        if (strcmp(cmd, "exit") == 0) {
            break;
        }

        execute_command(cmd);
    }

    return 0;
}

