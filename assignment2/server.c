// Server side C/C++ program to demonstrate Socket programming

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pwd.h>

#define PORT 80
int main(int argc, char **argv)
{
  int server_fd, new_socket, valread;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  char buffer[102] = {0};
  char *hello = "Hello from server";
  pid_t pid;
  int ch;
  char *pname = NULL;
  int wait_status;
  struct passwd *pw;
  char *name = "nobody";

  printf("execve=0x%p\n", execve);

  while ((ch = getopt(argc, argv, "P:F:")) != -1) {
    switch (ch) {
      case 'P':
        pname = optarg;
        break;
      case 'F':
        server_fd = atoi(optarg); // might throw an error
        break;
      default:
        perror("getopt");
        exit(EXIT_FAILURE);
    }
  }

  printf("######DEBUG######\n");
  printf("There are %d arguments\n", argc);
  int j;
  for (j = 0; j < argc; j++)
      printf("argv[%d]: %s\n", j, argv[j]);
  printf("#################\n\n");

  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons( PORT );

  if (pname != NULL) {

    /* In the child */

    printf("Listening...\n");
    if (listen(server_fd, 3) < 0)
    {
      perror("listen");
      exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
      perror("accept");
      exit(EXIT_FAILURE);
    }

    valread = read( new_socket , buffer, 1024);
    printf("%s\n",buffer );
    send(new_socket , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    return 0;
  } else {

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
      perror("socket failed");
      exit(EXIT_FAILURE);
    }

    // Attaching socket to port 80
    if (setsockopt(server_fd,
      SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
      &opt, sizeof(opt)))
    {
      perror("setsockopt");
      exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 80
    if (bind(server_fd, (struct sockaddr *)&address,
                                 sizeof(address))<0)
    {
      perror("bind failed");
      exit(EXIT_FAILURE);
    }

    char **nargv;
    int nargc, i;

    int length = snprintf( NULL, 0, "%d", server_fd );
    char* str = malloc( length + 1 );
    snprintf( str, length + 1, "%d", server_fd );

    nargv = calloc(argc + 5, sizeof(char *));
    if (nargv == NULL) {
      perror("calloc");
      exit(EXIT_FAILURE);
    }

    /* Copy the program name first. */
    nargc = 0;
    nargv[nargc++] = argv[0];

    /* Set the new process name */
    nargv[nargc++] = "-P";
    nargv[nargc++] = "child";

    /* copy the file descriptor */
    nargv[nargc++] = "-F";
    nargv[nargc++] = str;

    /* copy the original args */
    for (i = 1; i < argc; i++)
      nargv[nargc++] = argv[i];

    nargv[nargc] = NULL;

    char *newenviron[] = { NULL };

    // start fork
    switch(pid = fork()) {
      case -1:
        /* error on fork */

        perror("fork");
        exit(EXIT_FAILURE);
      case 0:
        /* child process */

      	// get uid of nobody
      	if ((pw = getpwnam(name)) == NULL)
      	{
      		perror("getpwnam failed");
      		exit(EXIT_FAILURE);
      	}

        printf("In forked child\n");
      	printf("Dropping to userid: %d\n", (*pw).pw_uid);

      	// drop from root to nobody
      	if (setuid((*pw).pw_uid) < 0)
      	{
      		perror("setuid failed");
      		exit(EXIT_FAILURE);
      	}

        printf("Replacing self...\n\n");
        execve(argv[0], nargv, newenviron);
        perror("execve");
        exit(EXIT_FAILURE);
      default:
        /* parent process */
        wait(&wait_status);
        return wait_status;
    }

  }
}
