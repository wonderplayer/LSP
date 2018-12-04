#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    int *sock = malloc(2 * sizeof(int));
    static const int parentsocket = 0;
    static const int childsocket = 1;
    pid_t pid;

    int ret = socketpair(AF_UNIX, SOCK_STREAM, 0, sock);
    if (ret) {
        perror("socketpair");
        return 1;
    }

    pid = fork();

    if (pid == 0) {
        /* in child code: read from socket, process arguments, write reply, exit*/
        char buf[256];
        read(sock[1], &buf, sizeof(buf));
        printf("Child: read '%s'\n", buf);

        int first = 0;
	int second = 0;
	sscanf(buf, "%d %d", &first, &second);
	printf("%s\n", buf);
	printf("%d %d %d\n", first, second, first + second);
        sprintf(buf, "%d", first + second);
        write(sock[1], &buf, sizeof(buf));

        close(sock[childsocket]);

    } else {
        /* in parent code: write to socket, wait for reply, read it, and output it*/

        char input[20];
        scanf("%[^\n]", input);

        write(sock[0], &input, sizeof(input));

        read(sock[0], &input, sizeof(input));
        printf("Parent: read '%s'\n", input);
        close(sock[parentsocket]);
    }

    return 0;
}
