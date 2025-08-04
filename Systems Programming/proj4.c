#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

struct DataPoint {double angle[3];};
int main() {
    const char* acclPath = "values/angl.dat";
    int input_fd = open(acclPath, O_RDONLY);
    if (input_fd < 0) {
        perror("Failed to open file");
        return 1;
    }
    struct DataPoint data;
    ssize_t bytesRead;
    int count = 0 ;
    struct timespec req = {1, 0};
    char* outside = "Out"; char* inside = "In"; const char* words[] = {"Roll", "Pitch","Yaw"};
    printf("Contents of angl.dat:\n");
    while ((bytesRead = read(input_fd, &data, sizeof(struct DataPoint))) == sizeof(struct DataPoint)) {
        count++;
        printf("%d: ",count);
        printf("Roll: %.8f, Pitch: %.8f, Yaw: %.8f\n", data.angle[0], data.angle[1], data.angle[2]);
        for (int i = 0; i < 3; i++) {
            if ((data.angle[i] < -20) || (data.angle[i] > 20)) {
                printf("%s: %s  ", words[i], outside);
            }
            else {
                printf("%s: %s  ", words[i], inside);
            }
        }
        printf("\n");
        nanosleep(&req, NULL);
    }
    if (bytesRead < 0) perror("Error reading");
    close(input_fd);
    return 0;
}