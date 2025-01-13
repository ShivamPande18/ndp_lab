#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX 1024

void sort_array(int arr[], int n, int ascending) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if ((ascending && arr[j] > arr[j + 1]) || (!ascending && arr[j] < arr[j + 1])) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

void split_odd_even(int arr[], int n) {
    printf("Odd numbers: ");
    for (int i = 0; i < n; i++) {
        if (arr[i] % 2 != 0) {
            printf("%d ", arr[i]);
        }
    }
    printf("\nEven numbers: ");
    for (int i = 0; i < n; i++) {
        if (arr[i] % 2 == 0) {
            printf("%d ", arr[i]);
        }
    }
    printf("\n");
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[MAX] = {0};

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind the socket to the PORT
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d", PORT);

    while (1) {
        // Accept a new connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        while (1) {
            memset(buffer, 0, MAX);
            read(new_socket, buffer, MAX);
            if (strcmp(buffer, "exit") == 0) {
                printf("Client has exited.\n");
                break;
            }

            char *token = strtok(buffer, " ");
            int choice = atoi(token);
            int arr[100];
            int n = 0;

            while ((token = strtok(NULL, " ")) != NULL) {
                arr[n++] = atoi(token);
            }

            switch (choice) {
                case 1: // Search
                    printf("Searching for number %d", arr[0]);
                    int found = 0;
                    for (int i = 1; i < n; i++) {
                        if (arr[i] == arr[0]) {
                            found = 1;
                            break;
                        }
                    }
                    if (found) {
                        send(new_socket, "Number found", strlen("Number found"), 0);
                    } else {
                        send(new_socket, "Number not found", strlen("Number not found"), 0);
                    }
                    break;

                case 2: // Sort
                    printf("Sorting array");
                    sort_array(arr + 1, n - 1, 1); // Sort in ascending order
                    char sorted[MAX];
                    sprintf(sorted, "Sorted array: ");
                    for (int i = 1; i < n; i++) {
                        sprintf(sorted + strlen(sorted), "%d ", arr[i]);
                    }
                    send(new_socket, sorted, strlen(sorted), 0);
                    break;

                case 3: // Split odd and even
                    printf("Splitting odd and even numbers");
                    split_odd_even(arr + 1, n - 1);
                    send(new_socket, "Odd and even numbers printed on server", strlen("Odd and even numbers printed on server"), 0);
                    break;

                default:
                    printf("Invalid choice");
                    send(new_socket, "Invalid choice", strlen("Invalid choice"), 0);
                    break;
            }
        }
        close(new_socket);
    }
    return 0;
}
