#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>


// mkdir 명령어 함수 (디렉토리 생성 함수)
void mkdir_test(int arg_num, char **arg_list){
       if (arg_num<2) {
	       printf("디렉토리명을 작성하세요\n");
       }
       else {
	       if (mkdir(arg_list[1], 0755) == -1 ) {
		       if(errno==EEXIST) {
			       printf("디렉토리가 이미 존재\n");
		       }
		       else {
			       printf("오류 발생\n");
		       }
	       }

	       else {
		       printf("디렉토리 생성\n");
	       }
       }
}


// cd 명령어 함수 (디렉토리 이동 함수)
void cd_test(int arg_num, char **arg_list) {

        const char *home_path = getenv("HOME");
        char current_path[1024];

        if (arg_num==1) {
            if (chdir(home_path)<0) {
                    printf("홈 디렉토리 이동 실패\n");
                    return;
            }
        }

	else if (strcmp(arg_list[1], "..") == 0) {
		if (chdir("..") < 0) {
			printf("부모 디렉토리로 이동 성공\n");
			return;
		}
	}

        else {
            if (chdir(arg_list[1]) < 0 ) {
                    printf("디렉토리 이동 실패\n");
                    return;
            }
        }

	if (getcwd(current_path, sizeof(current_path)) == NULL) {
        	printf("현재 디렉토리 경로 가져오기 실패");
        	return;
    	}

        printf("디렉토리 변경 성공\n");
        printf("변경된 디렉토리 : %s\n", current_path);
}

// rmdir 명령어 함수 (디렉토리 삭제 함수)
void rmdir_test(int arg_num, char **arg_list) {
	if (arg_num < 2) {
		printf("디렉토리 명을 작성하세요\n");
	}
	else {
		if (rmdir(arg_list[1]) == -1) {
				printf("디렉토리 삭제 실패\n");
		}
		else {
			printf("디렉토리 삭제 완료\n");
		}
	}
}

// ln 명령어 함수 (파일 연결 관계 함수)
void ln_test(int arg_num, char **arg_list) {
	if (arg_num < 3) {
		printf("두 파일명 필요\n");
		return;
	}

	else if (symlink(arg_list[1], arg_list[2]) == -1 ) {
		printf("링크 생성 실패\n");
	}
	else {
		printf("링크 생성 성공\n");
		printf("%s -> %s\n", arg_list[2], arg_list[1]);
	}
}




int getargs(char *cmd, char **argv) {
    int narg = 0;
    while(*cmd) {
        if(*cmd == ' ' || *cmd == '\t')
            *cmd++ = '\0';
        else {
            argv[narg++] = cmd++;
            while(*cmd != '\0' && *cmd != ' ' && *cmd != '\t')
                cmd++;
        }
    }
    argv[narg] = NULL;
    return narg;
}

int main() {
	char buf[100];
        char *argv[50];
        int narg;

	while (1) {
		printf("입력> ");
                fgets(buf, sizeof(buf), stdin);
                buf[strcspn(buf, "\n")] = '\0';

                clearerr(stdin);
                narg = getargs(buf, argv);

		if (strcmp(argv[0], "mkdir") ==0) {
			mkdir_test(narg, argv);
		}
		else if (strcmp(argv[0], "cd") == 0 ) {
            cd_test(narg,argv);
                }
		else if (strcmp(argv[0], "rmdir") == 0) { 
            rmdir_test(narg, argv);
		}
		else if (strcmp(argv[0], "ln") == 0) {
			ln_test(narg, argv);
		}
	}
	return 0;
}

