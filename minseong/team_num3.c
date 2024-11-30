#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>

int go_flag = 0;

void signal_exit(int sig) {
	printf("\n종료\n");
	exit(0);
}

void signal_stop(int sig) {
	printf("\n일시중지\n");
	go_flag = 0;
}

void set_signal() {
	struct sigaction sigaction_exit, sigaction_stop;

	sigaction_exit.sa_handler = signal_exit;
    	sigfillset(&sigaction_exit.sa_mask);
    	sigaction_exit.sa_flags = 0;
    	sigaction(SIGINT, &sigaction_exit, NULL);

    	sigaction_stop.sa_handler = signal_stop;
    	sigfillset(&sigaction_stop.sa_mask);
    	sigaction_stop.sa_flags = 0;
    	sigaction(SIGTSTP, &sigaction_stop, NULL);
}

int getargs(char *cmd, char **argv) {
	int narg = 0;
	while (*cmd) {
		if (*cmd == ' ' || *cmd == '\t') {
			 *cmd++ = '\0';
		}
		else {
			argv[narg++] = cmd++;
			while (*cmd != '\0' && *cmd != ' ' && *cmd != '\t') {
				cmd++;
			}
		}
	}
	argv[narg] = NULL;
	return narg;
}

void process_command() {
	char buf[100];
	char *argv[50];
	int narg;

	printf("입력> ");
	fgets(buf, sizeof(buf), stdin);
    	buf[strcspn(buf, "\n")] = '\0';
    	narg = getargs(buf, argv);

	if (strcmp(argv[0], "go") == 0) {
		go_flag = 1;
	}
}

void loop() {
	while (1) {
		if (go_flag) {
			printf("go\n");
			sleep(2);
		}
		else {
			process_command();
		}
	}
}

int main() {
	set_signal();
	loop();

	return 0;
}













