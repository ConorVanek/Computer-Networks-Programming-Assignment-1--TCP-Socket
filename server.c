#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <ctype.h>
#include <unistd.h>
#define MAX 256
#define SA struct sockaddr


char** createNumberArray() {
	// Number array for replacing numbers with word numbers
    char** numbers = (char**)malloc(10 * sizeof(char*));

    if (numbers == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL;
    }

    numbers[0] = strdup("zero");
    numbers[1] = strdup("one");
    numbers[2] = strdup("two");
    numbers[3] = strdup("three");
    numbers[4] = strdup("four");
    numbers[5] = strdup("five");
    numbers[6] = strdup("six");
    numbers[7] = strdup("seven");
    numbers[8] = strdup("eight");
    numbers[9] = strdup("nine");

    return numbers;
}

void freeNumberArray(char** numbers) {
	// For freeing the allocated memory of the number array
    for (int i = 0; i < 10; i++) {
        free(numbers[i]);
    }
    free(numbers);
}

char* replaceSubstring(const char* str, char* find, const char* replace) {
	// Find all occurances of a substring within a string and replace it with another substring
    int findLen = strlen(find);
    int replaceLen = strlen(replace);

    // Count the number of occurrences of the find substring
    int count = 0;
    const char* p = str;
    while ((p = strstr(p, find)) != NULL) {
        count++;
        p += findLen;
    }

    // Calculate the length of the resulting string
    int resultLen = strlen(str) + (replaceLen - findLen) * count;

    // Allocate memory for the result
    char* result = (char*)malloc((resultLen + 1) * sizeof(char));
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return NULL;
    }

    // Perform the replacement
    const char* source = str;
    char* destination = result;

    while (1) {
        p = strstr(source, find);
        if (p == NULL) {
            strcpy(destination, source);
            break;
        }
        int len = p - source;
        strncpy(destination, source, len);
        destination += len;
        strcpy(destination, replace);
        destination += replaceLen;
        source = p + findLen;
    }

    return result;
}

int countUpper(char *buff) {
	int numUpper = 0;
	char current;
	for(int i=0; buff[i]!='\0'; i++) {
		current = buff[i];
		if(isupper(current) != 0) {
			numUpper++;
		}
	}
	return numUpper;
}

int countLower(char *buff) {
	int numLower = 0;
	char current;
	for(int i=0; buff[i]!='\0'; i++) {
		current = buff[i];
		if(islower(current) != 0) {
			numLower++;
		}
	}
	return numLower;
}

int countSpecial(char *buff) {
	int numSpecial = 0;
	for(int i=0; buff[i]!='\0'; i++) {
		if(buff[i]=='@'||buff[i]=='#'||buff[i]=='$'||buff[i]=='%'||buff[i]=='^'||buff[i]=='&') {
			numSpecial++;
		}
	}
	return numSpecial;
}

// Function designed for chat between client and server.
void func(int connfd)
{
    char buff[MAX];
	char *output;
	int numUpper, numLower, numSpecial;
	char str[] = "Hello";
	const char rep[] = "Hello";
	char nums[10][2];
	// Assign values to the array
    for (int i = 0; i < 10; i++) {
        nums[i][0] = '0' + i;
		nums[i][1] = '\0';
    }
	char **numrep = createNumberArray();
	

    // infinite loop for chat
    for (;;) {
        bzero(buff, MAX);
   
        // read the message from client and copy it in buffer
        read(connfd, buff, sizeof(buff));
		if (strncmp("quit", buff, 4) != 0) {
			numUpper = countUpper(buff);
			numLower = countLower(buff); 
			numSpecial = countSpecial(buff);
			// print buffer which contains the client contents
			printf("From client: %s\n", buff);
			output = replaceSubstring(buff,str,rep);
			for (int i = 0; i<10; i++) {
				output = replaceSubstring(output,nums[i],numrep[i]);
			}
			char send[MAX];
			sprintf(send, "%s\nUppercase alphabets: %d\nLowercase alphabets: %d\nSpecial Characters: %d\n", output, numUpper, numLower, numSpecial);
	   
			//and send that buffer to client
			 write(connfd, send, sizeof(send));
		}
        // if msg contains "Exit" then server exit and chat ended.
        else {
			write(connfd, buff, sizeof(buff));
            printf("Server Exit...\n");
            break;
        }
		bzero(buff, MAX);
    }
}
   
// Driver function
int main(int argc, char *argv[])
{
    int sockfd, connfd, len, port;
    struct sockaddr_in servaddr, cli;
    port = atoi(argv[1]);
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Failed to create socket...\n");
        exit(0);
    }
    else
        printf("Socket created..\n");
    bzero(&servaddr, sizeof(servaddr));
   
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
   
    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");
   
    // Now server is ready to listen
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");
    len = sizeof(cli);
   
    // Accept the data packet from client and verification
    connfd = accept(sockfd, (SA*)&cli, &len);
    if (connfd < 0) {
        printf("server accept failed...\n");
        exit(0);
    }
    else
        printf("server accept the client...\n");
   
    // Function for chatting between client and server
    func(connfd);
   
    // After chatting close the socket
    close(sockfd);
	return 0;
}