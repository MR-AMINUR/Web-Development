************* CRC *************

/* receiver.c */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int s, c;
    struct sockaddr_in a;
    char msg[100];

    s = socket(AF_INET, SOCK_STREAM, 0);

    a.sin_family = AF_INET;
    a.sin_port = htons(8080);
    a.sin_addr.s_addr = INADDR_ANY;

    bind(s, (struct sockaddr*)&a, sizeof(a));
    listen(s, 1);

    printf("Receiver waiting...\n");

    c = accept(s, NULL, NULL);

    read(c, msg, sizeof(msg));

    printf("Received Data: %s\n", msg);

    if (msg[strlen(msg)-1] == '0')
        printf("No Error Detected\n");
    else
        printf("Error Detected\n");

    close(c);
    close(s);
}


/* sender.c */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int s, choice;
    struct sockaddr_in a;
    char msg[100];

    s = socket(AF_INET, SOCK_STREAM, 0);

    a.sin_family = AF_INET;
    a.sin_port = htons(8080);
    a.sin_addr.s_addr = INADDR_ANY;

    connect(s, (struct sockaddr*)&a, sizeof(a));

    printf("Enter binary data: ");
    scanf("%s", msg);

    strcat(msg, "0");   // CRC bit

    printf("Introduce error? (1=yes 0=no): ");
    scanf("%d", &choice);

    if (choice == 1)
        msg[strlen(msg)-1] = '1';

    write(s, msg, strlen(msg));

    printf("Sent: %s\n", msg);

    close(s);
}



******************* calculator **************

/*client.c*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char data[100];
    float a, b;
    char op;

    // 1. Create TCP socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // 2. Configure server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // 3. Connect
    connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // 4. Input
    printf("Enter expression (example: 10 + 5): ");
    scanf("%f %c %f", &a, &op, &b);

    // 5. Send data
    sprintf(data, "%f %c %f", a, op, b);
    write(client_socket, data, strlen(data));

    // 6. Receive result
    memset(data, 0, sizeof(data));
    read(client_socket, data, sizeof(data));

    printf("Result = %s\n", data);

    close(client_socket);

    return 0;
}


/*server.c*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr;
    char data[100];
    float a, b, result;
    char op;

    // 1. Create TCP socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // 2. Configure server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // 3. Bind
    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // 4. Listen
    listen(server_socket, 1);

    printf("Calculator Server waiting...\n");

    // 5. Accept client
    client_socket = accept(server_socket, NULL, NULL);

    // 6. Receive data
    read(client_socket, data, sizeof(data));

    // Format: number operator number
    sscanf(data, "%f %c %f", &a, &op, &b);

    // 7. Perform calculation
    switch(op) {
        case '+': result = a + b; break;
        case '-': result = a - b; break;
        case '*': result = a * b; break;
        case '/': result = a / b; break;
        default: result = 0;
    }

    // 8. Send result
    sprintf(data, "%.2f", result);
    write(client_socket, data, strlen(data));

    close(client_socket);
    close(server_socket);

    return 0;
}



********************* command execution ****************

/*client.c*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char cmd[100], result[1024];

    // 1. Create TCP socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // 2. Configure server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // 3. Connect to server
    connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // 4. Input command
    printf("Enter command: ");
    fgets(cmd, sizeof(cmd), stdin);
    cmd[strcspn(cmd, "\n")] = 0;

    // 5. Send command
    write(client_socket, cmd, strlen(cmd));

    // 6. Receive result
    memset(result, 0, sizeof(result));
    read(client_socket, result, sizeof(result));

    printf("Command Output:\n%s\n", result);

    close(client_socket);

    return 0;
}


/*server.c*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr;
    char cmd[100], output[1024];
    FILE *fp;

    // 1. Create TCP socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // 2. Configure server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // 3. Bind socket
    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // 4. Listen
    listen(server_socket, 1);

    printf("Server waiting...\n");

    // 5. Accept client
    client_socket = accept(server_socket, NULL, NULL);

    // 6. Receive command
    memset(cmd, 0, sizeof(cmd));
    read(client_socket, cmd, sizeof(cmd));

    printf("Command received: %s\n", cmd);

    // 7. Execute command
    fp = popen(cmd, "r");

    memset(output, 0, sizeof(output));

    while (fgets(output + strlen(output),
                 sizeof(output) - strlen(output), fp) != NULL);

    pclose(fp);

    // 8. Send output to client
    write(client_socket, output, strlen(output));

    close(client_socket);
    close(server_socket);

    return 0;
}




************************* chat app ****************

/*persion1.c*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char msg[100];

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Configure server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Connect
    connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    while (1) {
        // Send message
        printf("Client: ");
        fgets(msg, sizeof(msg), stdin);
        msg[strcspn(msg, "\n")] = 0;

        write(client_socket, msg, strlen(msg));

        if (strcmp(msg, "exit") == 0)
            break;

        // Receive reply
        memset(msg, 0, sizeof(msg));
        read(client_socket, msg, sizeof(msg));

        if (strcmp(msg, "exit") == 0)
            break;

        printf("Server: %s\n", msg);
    }

    close(client_socket);

    return 0;
}


/*persion2.c*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr;
    char msg[100];

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Configure server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind
    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // Listen
    listen(server_socket, 1);

    printf("Server waiting...\n");

    // Accept client
    client_socket = accept(server_socket, NULL, NULL);

    while (1) {
        memset(msg, 0, sizeof(msg));

        // Receive from client
        read(client_socket, msg, sizeof(msg));

        if (strcmp(msg, "exit") == 0)
            break;

        printf("Client: %s\n", msg);

        // Send reply
        printf("Server: ");
        fgets(msg, sizeof(msg), stdin);
        msg[strcspn(msg, "\n")] = 0;

        write(client_socket, msg, strlen(msg));

        if (strcmp(msg, "exit") == 0)
            break;
    }

    close(client_socket);
    close(server_socket);

    return 0;
}




****************** dns lookup ******************

#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>

int main() {
    char domain[100];
    struct hostent *host;
    struct in_addr addr;

    // Input domain name
    printf("Enter domain name: ");
    scanf("%s", domain);

    // DNS lookup
    host = gethostbyname(domain);

    if (host == NULL) {
        printf("DNS lookup failed\n");
        return 1;
    }

    // Convert address
    addr = *(struct in_addr *)host->h_addr;

    printf("IP Address: %s\n", inet_ntoa(addr));

    return 0;
}




******************* DayTime SERVER *********************

/*client.c*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char msg[100];

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Configure server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Connect to server
    connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // Receive datetime
    memset(msg, 0, sizeof(msg));
    recv(client_socket, msg, sizeof(msg), 0);

    printf("Current Date and Time: %s", msg);

    close(client_socket);

    return 0;
}

/*server.c*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr;
    char datetime[100];
    time_t now;

    // Create TCP socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Configure server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // Listen
    listen(server_socket, 1);

    printf("DateTime Server waiting...\n");

    // Accept client
    client_socket = accept(server_socket, NULL, NULL);

    // Get current time
    now = time(NULL);

    // Convert to readable format
    snprintf(datetime, sizeof(datetime), "%s", ctime(&now));

    // Send to client
    send(client_socket, datetime, strlen(datetime), 0);

    close(client_socket);
    close(server_socket);

    return 0;
}





****************** File Transfer ************************

/*client.c*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[1024];
    FILE *fp;

    // 1. Create TCP socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // 2. Configure server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // 3. Connect to server
    connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // 4. Open file to send
    fp = fopen("send.txt", "r");

    if (fp == NULL) {
        printf("File not found\n");
        return 1;
    }

    // 5. Send file data
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        write(client_socket, buffer, strlen(buffer));
    }

    printf("File sent successfully\n");

    fclose(fp);
    close(client_socket);

    return 0;
}


/*server.c*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr;
    char buffer[1024];
    FILE *fp;

    // 1. Create TCP socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // 2. Configure server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // 3. Bind
    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // 4. Listen
    listen(server_socket, 1);

    printf("Server waiting for file...\n");

    // 5. Accept client
    client_socket = accept(server_socket, NULL, NULL);

    // 6. Open file to save received data
    fp = fopen("received.txt", "w");

    // 7. Receive file data
    while (read(client_socket, buffer, sizeof(buffer)) > 0) {
        fputs(buffer, fp);
        memset(buffer, 0, sizeof(buffer));
    }

    printf("File received successfully\n");

    fclose(fp);
    close(client_socket);
    close(server_socket);

    return 0;
}

/*send.txt*/
Hello from file transfer
This is test data


/*recieved.txt*/
Hello from file transfer
This is test data
,"u





******************* Go Back NRQ *******************

/* receiver.c */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    char frame[100], ack[100];

    srand(time(0));

    /* create socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    /* set address */
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    address.sin_addr.s_addr = INADDR_ANY;

    /* bind */
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    /* listen */
    listen(server_fd, 1);

    printf("Receiver waiting...\n");

    /* accept connection */
    client_fd = accept(server_fd, NULL, NULL);

    while (1) {

        memset(frame, 0, sizeof(frame));
        read(client_fd, frame, sizeof(frame));

        if (strcmp(frame, "END") == 0)
            break;

        printf("Received: %s\n", frame);

        /* random frame loss */
        int x = rand() % 3;

        if (x == 0) {
            printf("Frame lost! No ACK sent\n\n");
        } else {
            strcpy(ack, "ACK");
            write(client_fd, ack, strlen(ack));
            printf("ACK sent\n\n");
        }
    }

    close(client_fd);
    close(server_fd);

    return 0;
}


/* sender.c */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

int main() {
    int sock, n, window, base = 1;
    struct sockaddr_in server;
    char frame[100], ack[100];

    /* create socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);

    /* server details */
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    /* connect */
    connect(sock, (struct sockaddr *)&server, sizeof(server));

    printf("Enter total frames: ");
    scanf("%d", &n);

    printf("Enter window size: ");
    scanf("%d", &window);

    while (base <= n) {

        int end = base + window - 1;
        if (end > n)
            end = n;

        printf("\nSending frames: ");

        /* send window frames */
        for (int i = base; i <= end; i++) {
            sprintf(frame, "Frame %d", i);
            write(sock, frame, strlen(frame));
            printf("%d ", i);
        }

        printf("\n");

        int success = 1;

        /* wait ACK for all frames */
        for (int i = base; i <= end; i++) {

            fd_set set;
            struct timeval timeout;

            FD_ZERO(&set);
            FD_SET(sock, &set);

            timeout.tv_sec = 3;
            timeout.tv_usec = 0;

            int rv = select(sock + 1, &set, NULL, NULL, &timeout);

            if (rv == 1) {
                memset(ack, 0, sizeof(ack));
                read(sock, ack, sizeof(ack));
                printf("ACK received for Frame %d\n", i);
            } else {
                printf("Timeout at Frame %d\n", i);
                printf("Go-Back-N: Resending from Frame %d\n", i);
                base = i;
                success = 0;
                break;
            }
        }

        if (success)
            base = end + 1;
    }

    strcpy(frame, "END");
    write(sock, frame, strlen(frame));

    close(sock);

    return 0;
}







*********************** HTTP client ***********************

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

int main() {
    int sock;
    struct sockaddr_in server_addr;
    struct hostent *server;
    char request[200];
    char response[1024];
    char host[] = "example.com";

    // 1. DNS lookup
    server = gethostbyname(host);

    // 2. Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // 3. Configure server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);
    memcpy(&server_addr.sin_addr.s_addr,
           server->h_addr,
           server->h_length);

    // 4. Connect to web server
    connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // 5. Create HTTP GET request
    sprintf(request,
            "GET / HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",
            host);

    // 6. Send request
    write(sock, request, strlen(request));

    // 7. Receive response
    while (read(sock, response, sizeof(response)-1) > 0) {
        printf("%s", response);
        memset(response, 0, sizeof(response));
    }

    close(sock);

    return 0;
}





*********************** Hamming Code *********************

/* receiver.c - Hamming Code */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int s, c;
    struct sockaddr_in a;
    char msg[100];

    s = socket(AF_INET, SOCK_STREAM, 0);

    a.sin_family = AF_INET;
    a.sin_port = htons(8080);
    a.sin_addr.s_addr = INADDR_ANY;

    bind(s, (struct sockaddr*)&a, sizeof(a));
    listen(s, 1);

    printf("Receiver waiting...\n");

    c = accept(s, NULL, NULL);

    read(c, msg, sizeof(msg));

    printf("Received Data: %s\n", msg);

    int p1 = (msg[0]-'0') ^ (msg[2]-'0') ^ (msg[4]-'0') ^ (msg[6]-'0');
    int p2 = (msg[1]-'0') ^ (msg[2]-'0') ^ (msg[5]-'0') ^ (msg[6]-'0');
    int p4 = (msg[3]-'0') ^ (msg[4]-'0') ^ (msg[5]-'0') ^ (msg[6]-'0');

    int error = p4*4 + p2*2 + p1;

    if (error == 0)
        printf("No Error Detected\n");
    else
        printf("Error at position %d\n", error);

    close(c);
    close(s);
}


/* sender.c - Hamming Code */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int s, choice;
    struct sockaddr_in a;
    char msg[100];

    s = socket(AF_INET, SOCK_STREAM, 0);

    a.sin_family = AF_INET;
    a.sin_port = htons(8080);
    a.sin_addr.s_addr = INADDR_ANY;

    connect(s, (struct sockaddr*)&a, sizeof(a));

    printf("Enter 7-bit Hamming code: ");
    scanf("%s", msg);

    printf("Introduce error? (1=yes 0=no): ");
    scanf("%d", &choice);

    if (choice == 1)
        msg[2] = (msg[2] == '0') ? '1' : '0';

    write(s, msg, strlen(msg));

    printf("Sent Data: %s\n", msg);

    close(s);
}







************************** Multi Client Chat *********************

/*client*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char msg[100];

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Configure server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Connect
    connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    while (1) {
        printf("Enter message: ");
        fgets(msg, sizeof(msg), stdin);
        msg[strcspn(msg, "\n")] = 0;

        write(client_socket, msg, strlen(msg));

        if (strcmp(msg, "exit") == 0)
            break;
    }

    close(client_socket);

    return 0;
}


/*server.c*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

void *chat(void *arg) {
    int client = *(int *)arg;
    char msg[100];

    while (1) {
        memset(msg, 0, sizeof(msg));
        read(client, msg, sizeof(msg));

        if (strcmp(msg, "exit") == 0)
            break;

        printf("Client: %s\n", msg);
    }

    close(client);
    return NULL;
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr;
    pthread_t t;

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Configure server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind
    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // Listen
    listen(server_socket, 5);

    printf("Multi-client server waiting...\n");

    while (1) {
        // Accept clients
        client_socket = accept(server_socket, NULL, NULL);

        // Create thread for each client
        pthread_create(&t, NULL, chat, &client_socket);
    }

    close(server_socket);

    return 0;
}







********************** open port **********************

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sock, port;
    struct sockaddr_in target;
    char ip[50];

    // Input target IP
    printf("Enter target IP: ");
    scanf("%s", ip);

    // Scan ports 1 to 10000
    for (port = 1; port <= 10000; port++) {

        // Create socket
        sock = socket(AF_INET, SOCK_STREAM, 0);

        target.sin_family = AF_INET;
        target.sin_port = htons(port);
        inet_pton(AF_INET, ip, &target.sin_addr);

        // Try connection
        if (connect(sock, (struct sockaddr*)&target, sizeof(target)) == 0) {
            printf("Port %d is OPEN\n", port);
        }

        close(sock);
    }

    return 0;
}



**************************** Parity checking ******************

/* receiver.c - Parity Check */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int countOnes(char s[]) {
    int count = 0;
    for (int i = 0; s[i]; i++)
        if (s[i] == '1')
            count++;
    return count;
}

int main() {
    int s, c;
    struct sockaddr_in a;
    char msg[100];

    s = socket(AF_INET, SOCK_STREAM, 0);

    a.sin_family = AF_INET;
    a.sin_port = htons(8080);
    a.sin_addr.s_addr = INADDR_ANY;

    bind(s, (struct sockaddr*)&a, sizeof(a));
    listen(s, 1);

    printf("Receiver waiting...\n");

    c = accept(s, NULL, NULL);

    read(c, msg, sizeof(msg));

    printf("Received Data: %s\n", msg);

    if (countOnes(msg) % 2 == 0)
        printf("No Error Detected (Even Parity)\n");
    else
        printf("Error Detected\n");

    close(c);
    close(s);
}


/* sender.c - Parity Check */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int countOnes(char s[]) {
    int count = 0;
    for (int i = 0; s[i]; i++)
        if (s[i] == '1')
            count++;
    return count;
}

int main() {
    int s, choice;
    struct sockaddr_in a;
    char msg[100];

    s = socket(AF_INET, SOCK_STREAM, 0);

    a.sin_family = AF_INET;
    a.sin_port = htons(8080);
    a.sin_addr.s_addr = INADDR_ANY;

    connect(s, (struct sockaddr*)&a, sizeof(a));

    printf("Enter binary data: ");
    scanf("%s", msg);

    /* add parity bit */
    if (countOnes(msg) % 2 == 0)
        strcat(msg, "0");
    else
        strcat(msg, "1");

    printf("Introduce error? (1=yes 0=no): ");
    scanf("%d", &choice);

    if (choice == 1)
        msg[0] = (msg[0] == '0') ? '1' : '0';

    write(s, msg, strlen(msg));

    printf("Sent Data: %s\n", msg);

    close(s);
}






*********************** piggy backing ********************

/* receiver.c - Piggybacking */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int s, c, n;
    struct sockaddr_in a;
    char msg[100], reply[100];

    s = socket(AF_INET, SOCK_STREAM, 0);

    a.sin_family = AF_INET;
    a.sin_port = htons(8080);
    a.sin_addr.s_addr = INADDR_ANY;

    bind(s, (struct sockaddr*)&a, sizeof(a));
    listen(s, 1);

    printf("Receiver waiting...\n");

    c = accept(s, NULL, NULL);

    while (1) {

        memset(msg, 0, sizeof(msg));
        read(c, msg, sizeof(msg));

        if (strcmp(msg, "END") == 0)
            break;

        sscanf(msg, "%d", &n);

        printf("Received Frame %d from Sender\n", n);

        /* send own data + ACK together */
        sprintf(reply, "Frame %d + ACK %d", n + 1, n);
        write(c, reply, strlen(reply));
    }

    close(c);
    close(s);
}


/* sender.c - Piggybacking */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int s, n, i;
    struct sockaddr_in a;
    char msg[100], reply[100];

    s = socket(AF_INET, SOCK_STREAM, 0);

    a.sin_family = AF_INET;
    a.sin_port = htons(8080);
    a.sin_addr.s_addr = INADDR_ANY;

    connect(s, (struct sockaddr*)&a, sizeof(a));

    printf("Enter number of frames: ");
    scanf("%d", &n);

    for (i = 1; i <= n; i++) {

        sprintf(msg, "%d", i);
        write(s, msg, strlen(msg));

        printf("Sent Frame %d\n", i);

        memset(reply, 0, sizeof(reply));
        read(s, reply, sizeof(reply));

        printf("Received: %s\n", reply);
        sleep(1);
    }

    strcpy(msg, "END");
    write(s, msg, strlen(msg));

    close(s);
}






*********************** selective window protocol *********************

/* receiver.c  - Selective Repeat ARQ (stable demo) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int s, c, n;
    struct sockaddr_in a;
    char msg[100], ack[100];
    int lost_once[100] = {0};   // frame will fail only first time

    s = socket(AF_INET, SOCK_STREAM, 0);

    a.sin_family = AF_INET;
    a.sin_port = htons(8080);
    a.sin_addr.s_addr = INADDR_ANY;

    bind(s, (struct sockaddr*)&a, sizeof(a));
    listen(s, 1);

    printf("Receiver waiting...\n");

    c = accept(s, NULL, NULL);

    while (1) {

        memset(msg, 0, sizeof(msg));
        read(c, msg, sizeof(msg));

        if (strcmp(msg, "END") == 0)
            break;

        sscanf(msg, "%d", &n);

        /* fail frame 3 only first time */
        if (n == 3 && lost_once[3] == 0) {
            printf("Frame %d lost\n", n);
            lost_once[3] = 1;
        }
        else {
            printf("Frame %d received\n", n);

            sprintf(ack, "%d", n);
            write(c, ack, strlen(ack));
        }
    }

    close(c);
    close(s);
}


/* sender.c - Selective Repeat ARQ (stable demo) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

int main() {
    int s, i, n, left;
    struct sockaddr_in a;
    char msg[100], ack[100];
    int done[100] = {0};

    s = socket(AF_INET, SOCK_STREAM, 0);

    a.sin_family = AF_INET;
    a.sin_port = htons(8080);
    a.sin_addr.s_addr = INADDR_ANY;

    connect(s, (struct sockaddr*)&a, sizeof(a));

    printf("Enter number of frames: ");
    scanf("%d", &n);

    left = n;

    while (left > 0) {

        /* send only missing frames */
        for (i = 1; i <= n; i++) {
            if (!done[i]) {
                sprintf(msg, "%d", i);
                write(s, msg, strlen(msg));
                printf("Sent Frame %d\n", i);
                usleep(300000);
            }
        }

        while (1) {

            fd_set set;
            struct timeval t;

            FD_ZERO(&set);
            FD_SET(s, &set);

            t.tv_sec = 2;
            t.tv_usec = 0;

            if (select(s + 1, &set, NULL, NULL, &t) <= 0)
                break;

            memset(ack, 0, sizeof(ack));
            read(s, ack, sizeof(ack));

            int x = atoi(ack);

            if (!done[x]) {
                done[x] = 1;
                left--;
                printf("ACK %d received\n", x);
            }
        }

        printf("Resending only lost frames...\n\n");
    }

    strcpy(msg, "END");
    write(s, msg, strlen(msg));

    close(s);
}






******************** sliding window *********************

/* receiver.c  - Sliding Window */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int s, c, n;
    struct sockaddr_in a;
    char msg[100], ack[100];

    s = socket(AF_INET, SOCK_STREAM, 0);

    a.sin_family = AF_INET;
    a.sin_port = htons(8080);
    a.sin_addr.s_addr = INADDR_ANY;

    bind(s, (struct sockaddr*)&a, sizeof(a));
    listen(s, 1);

    printf("Receiver waiting...\n");

    c = accept(s, NULL, NULL);

    while (1) {

        memset(msg, 0, sizeof(msg));
        read(c, msg, sizeof(msg));

        if (strcmp(msg, "END") == 0)
            break;

        sscanf(msg, "%d", &n);

        printf("Received Frame %d\n", n);

        sprintf(ack, "%d", n);
        write(c, ack, strlen(ack));
    }

    close(c);
    close(s);
}


/* sender.c - Sliding Window */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int s, n, w, base = 1, i;
    struct sockaddr_in a;
    char msg[100], ack[100];

    s = socket(AF_INET, SOCK_STREAM, 0);

    a.sin_family = AF_INET;
    a.sin_port = htons(8080);
    a.sin_addr.s_addr = INADDR_ANY;

    connect(s, (struct sockaddr*)&a, sizeof(a));

    printf("Enter total frames: ");
    scanf("%d", &n);

    printf("Enter window size: ");
    scanf("%d", &w);

    while (base <= n) {

        printf("\nCurrent Window: ");

        for (i = base; i < base + w && i <= n; i++) {
            printf("%d ", i);

            sprintf(msg, "%d", i);
            write(s, msg, strlen(msg));

            memset(ack, 0, sizeof(ack));
            read(s, ack, sizeof(ack));
        }

        printf("\nWindow Slid Forward\n");

        base++;
        sleep(1);
    }

    strcpy(msg, "END");
    write(s, msg, strlen(msg));

    close(s);
}








*********************** stop and wait *************************

/*reciever*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

int main() {
    int server_fd, client_fd;
    struct sockaddr_in address;
    char frame[100];
    char ack[] = "ACK";

    srand(time(0));

    /* create socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    /* set address */
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    address.sin_addr.s_addr = INADDR_ANY;

    /* bind */
    bind(server_fd, (struct sockaddr *)&address, sizeof(address));

    /* listen */
    listen(server_fd, 1);

    printf("Receiver waiting...\n");

    /* accept connection */
    client_fd = accept(server_fd, NULL, NULL);

    while (1) {

        memset(frame, 0, sizeof(frame));
        read(client_fd, frame, sizeof(frame));

        if (strcmp(frame, "END") == 0)
            break;

        printf("Received: %s\n", frame);

        /* random frame failure */
        int x = rand() % 2;

        if (x == 0) {
            printf("Frame lost! No ACK sent\n\n");
        } else {
            write(client_fd, ack, strlen(ack));
            printf("ACK sent\n\n");
        }
    }

    close(client_fd);
    close(server_fd);

    return 0;
}


/* sender.c */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>

int main() {
    int sock, n, i = 1;
    struct sockaddr_in server;
    char frame[100], ack[100];

    /* create socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);

    /* set server details */
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    /* connect */
    connect(sock, (struct sockaddr *)&server, sizeof(server));

    printf("Enter number of frames: ");
    scanf("%d", &n);

    while (i <= n) {

        sprintf(frame, "Frame %d", i);

        /* send frame */
        write(sock, frame, strlen(frame));
        printf("Sent: %s\n", frame);

        /* wait for ACK with timeout */
        fd_set set;
        struct timeval timeout;

        FD_ZERO(&set);
        FD_SET(sock, &set);

        timeout.tv_sec = 3;
        timeout.tv_usec = 0;

        int rv = select(sock + 1, &set, NULL, NULL, &timeout);

        if (rv == 1) {
            memset(ack, 0, sizeof(ack));
            read(sock, ack, sizeof(ack));
            printf("ACK received for Frame %d\n\n", i);
            i++;
        } else {
            printf("Timeout! Resending Frame %d\n\n", i);
        }
    }

    /* end signal */
    strcpy(frame, "END");
    write(sock, frame, strlen(frame));

    close(sock);

    return 0;
}






******************** tcp echo server ***********************

/*client*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char msg[100], reply[100];

    // 1. Create TCP socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // 2. Set server details
    server_addr.sin_family = AF_INET;           // IPv4
    server_addr.sin_port = htons(8080);         // Same port as server
    server_addr.sin_addr.s_addr = INADDR_ANY;   // Local machine

    // 3. Connect to server
    connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // 4. Take message input
    printf("Enter message: ");
    scanf("%s", msg);

    // 5. Send message to server
    write(client_socket, msg, strlen(msg));

    // 6. Receive echoed message
    memset(reply, 0, sizeof(reply));
    read(client_socket, reply, sizeof(reply));

    printf("Echo from server: %s\n", reply);

    // 7. Close socket
    close(client_socket);

    return 0;
}


/*server*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr;
    char msg[100];

    // 1. Create TCP socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // 2. Set server address details
    server_addr.sin_family = AF_INET;           // IPv4
    server_addr.sin_port = htons(8080);         // Port number
    server_addr.sin_addr.s_addr = INADDR_ANY;   // Accept from any IP

    // 3. Bind socket to address and port
    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // 4. Listen for incoming client connection
    listen(server_socket, 1);

    printf("Echo Server waiting...\n");

    // 5. Accept client connection
    client_socket = accept(server_socket, NULL, NULL);

    // 6. Receive message from client
    memset(msg, 0, sizeof(msg));
    read(client_socket, msg, sizeof(msg));

    printf("Received from client: %s\n", msg);

    // 7. Send same message back to client (echo)
    write(client_socket, msg, strlen(msg));

    // 8. Close sockets
    close(client_socket);
    close(server_socket);

    return 0;
}





************************ TCP ***************

/*client.c*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char msg[] = "Hello from Client";
    char reply[100];

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Connect to server
    connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // Send message
    write(client_socket, msg, strlen(msg));

    // Receive reply
    read(client_socket, reply, sizeof(reply));

    printf("Message from server: %s\n", reply);

    close(client_socket);

    return 0;
}


/*server.c*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr;
    char msg[100];

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // Configure server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // Listen for client
    listen(server_socket, 1);

    printf("Server waiting for connection...\n");

    // Accept client
    client_socket = accept(server_socket, NULL, NULL);

    // Receive message
    read(client_socket, msg, sizeof(msg));

    printf("Message from client: %s\n", msg);

    // Send reply
    char reply[] = "Hello from Server";
    write(client_socket, reply, strlen(reply));

    close(client_socket);
    close(server_socket);

    return 0;
}







**************************** test ************************* 

/*reciever*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int s, c;
    struct sockaddr_in a;
    char msg[100];

    // create socket
    s = socket(AF_INET, SOCK_STREAM, 0);

    // setup address
    a.sin_family = AF_INET;
    a.sin_port = htons(8080);
    a.sin_addr.s_addr = INADDR_ANY;

    // bind + listen
    bind(s, (struct sockaddr*)&a, sizeof(a));
    listen(s, 1);

    printf("Receiver waiting...\n");

    // accept sender
    c = accept(s, NULL, NULL);

    // receive message
    read(c, msg, sizeof(msg));

    printf("Message received: %s\n", msg);

    close(c);
    close(s);

    return 0;
}

/*sender*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int s;
    struct sockaddr_in a;
    char msg[] = "Hello from Sender";

    // create socket
    s = socket(AF_INET, SOCK_STREAM, 0);

    // setup receiver address
    a.sin_family = AF_INET;
    a.sin_port = htons(8080);
    a.sin_addr.s_addr = INADDR_ANY;

    // connect to receiver
    connect(s, (struct sockaddr*)&a, sizeof(a));

    // send message
    write(s, msg, strlen(msg));

    printf("Message sent\n");

    close(s);

    return 0;
}

/*text.txt*/
700135






*************************** UDP******************** 

/*client.c*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char msg[] = "Hello from UDP Client";
    char reply[100];
    socklen_t len = sizeof(server_addr);

    // Create UDP socket
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);

    // Configure server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Send message
    sendto(client_socket, msg, strlen(msg), 0,
           (struct sockaddr*)&server_addr, len);

    // Receive reply
    recvfrom(client_socket, reply, sizeof(reply), 0,
             NULL, NULL);

    printf("Message from server: %s\n", reply);

    close(client_socket);

    return 0;
}

/*server.c*/
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_socket;
    struct sockaddr_in server_addr, client_addr;
    char msg[100];
    socklen_t len = sizeof(client_addr);

    // Create UDP socket
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);

    // Configure server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    printf("UDP Server waiting...\n");

    // Receive message
    recvfrom(server_socket, msg, sizeof(msg), 0,
             (struct sockaddr*)&client_addr, &len);

    printf("Message from client: %s\n", msg);

    // Send reply
    char reply[] = "Hello from UDP Server";

    sendto(server_socket, reply, strlen(reply), 0,
           (struct sockaddr*)&client_addr, len);

    close(server_socket);

    return 0;
}



