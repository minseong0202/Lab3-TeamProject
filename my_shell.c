#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <libgen.h>

#define MAX_CMD_LEN 1024
#define MAX_ARGS 100

void cp_file(char *source, char *destination) {
    int src_fd = open(source, O_RDONLY);
    if (src_fd == -1) {
        perror("source 파일 열기 실패");
        return;
    }

    int dest_fd = open(destination, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1) {
        perror("destination 파일 열기 실패");
        close(src_fd);
        return;
    }

    char buffer[1024];
    ssize_t bytes_read;
    while ((bytes_read = read(src_fd, buffer, sizeof(buffer))) > 0) {
        if (write(dest_fd, buffer, bytes_read) != bytes_read) {
            perror("파일 복사 실패");
            break;
        }
    }

    close(src_fd);
    close(dest_fd);
    printf("파일 복사 완료: %s -> %s\n", source, destination);
}

void rm_file(char *file) {
    if (remove(file) == 0) {
        printf("파일 삭제 완료: %s\n", file);
    } else {
        perror("파일 삭제 실패");
    }
}

void mv_file(char *source, char *destination) {
    struct stat dest_stat;
    if (stat(destination, &dest_stat) == 0 && S_ISDIR(dest_stat.st_mode)) {
        char new_dest[1024];
        snprintf(new_dest, sizeof(new_dest), "%s/%s", destination, basename(source));
        destination = new_dest;
    }

    if (rename(source, destination) == 0) {
        printf("파일 이동 완료: %s -> %s\n", source, destination);
    } else {
        perror("파일 이동 실패");
    }
}

void cat_file(char *file) {
    int fd = open(file, O_RDONLY);
    if (fd == -1) {
        perror("파일 열기 실패");
        return;
    }

    char buffer[1024];
    ssize_t bytes_read;
    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        write(STDOUT_FILENO, buffer, bytes_read);
    }

    close(fd);
}

void process_command(char *cmd) {
    char *command = strtok(cmd, " ");
    char *arg1 = strtok(NULL, " ");
    char *arg2 = strtok(NULL, " ");
    
    if (command == NULL) return;

    if (strcmp(command, "cp") == 0) {
        if (arg1 && arg2) {
            cp_file(arg1, arg2);
        }
    } else if (strcmp(command, "rm") == 0) {
        if (arg1) {
            rm_file(arg1);
        }
    } else if (strcmp(command, "mv") == 0) {
        if (arg1 && arg2) {
            mv_file(arg1, arg2);
        }
    } else if (strcmp(command, "cat") == 0) {
        if (arg1) {
            cat_file(arg1);
        }
    } else {
        printf("알 수 없는 명령어: %s\n", command);
    }
}

int main() {
    char cmd[MAX_CMD_LEN];

    while (1) {
        printf("myshell> ");
        if (fgets(cmd, sizeof(cmd), stdin) == NULL) {
            perror("입력 실패");
            exit(1);
        }

        cmd[strcspn(cmd, "\n")] = 0;

        if (strcmp(cmd, "exit") == 0) {
            break;
        }

        process_command(cmd);
    }

    return 0;
}

