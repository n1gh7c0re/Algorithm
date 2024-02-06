#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define N 20 // Board size

// Define a struct for a square
typedef struct {
    int x;
    int y;
    int number;
} Square;

// Function to check if a position is inside the board
int Inside(int x, int y) {
    if (x >= 0 && x < N && y >= 0 && y < N) return 1;
    return 0;
}

// Function to find the minimum number of steps for a knight to reach the target position
int Solution(int Knight[2], int Target[2]) {
    // Initialize the visited matrix
    int visited[N][N];
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            visited[i][j] = 0;
        }
    }

    // Define the possible moves for the knight
    int Moves[8][2] = { {1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1} };

    // Initialize the start position and the queue
    Square Start = { Knight[0], Knight[1], 0 };
    Square* queue = (Square*)malloc(N * N * sizeof(Square));
    int front = 0, rear = 0;
    queue[rear++] = Start;

    while (front != rear) {
        Square curr = queue[front++];
        if (curr.x == Target[0] && curr.y == Target[1]) { // Target was reached
            free(queue);
            return curr.number;
        }
        for (int i = 0; i < 8; i++) {
            int x_new = curr.x + Moves[i][0];
            int y_new = curr.y + Moves[i][1];
            // Finding the minimum number of steps to reach the target
            if (Inside(x_new, y_new) && !visited[x_new][y_new]) {
                Square new_square = { x_new, y_new, curr.number + 1 };
                queue[rear++] = new_square;
                visited[x_new][y_new] = 1;
            }
        }
    }
    free(queue);
    return -1;  // Target position is not reachable
}

int main() {
    int Knight[2]; // Knight position
    printf("Enter knight position: ");
    scanf("%d %d", &Knight[0], &Knight[1]);
    int Target[2]; // Target position
    printf("Enter target position: ");
    scanf("%d %d", &Target[0], &Target[1]);
    int number = Solution(Knight, Target);
    if (number != -1) printf("Number of steps: %d", number);
    else printf("Impossible to reach the position");
    return 0;
}