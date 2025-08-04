#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

struct DataPoint {
    double acceleration[3];  
    double velocity[3];     
    double angle[3];      
};

int main() {
    // Create a directory named "values"
    const char* dirName = "values";
    if (mkdir(dirName, 0755) == -1) {
        if (errno == EEXIST) {
            printf("Directory '%s' already exists.\n", dirName);
        } else {
            perror("Error creating directory");
            return 1;
        }
    } else {
        printf("Directory '%s' created successfully.\n", dirName);
    }

    // File paths
    char acclPath[256], rotaPath[256], anglPath[256];
    snprintf(acclPath, sizeof(acclPath), "%s/accl.dat", dirName);
    snprintf(rotaPath, sizeof(rotaPath), "%s/rota.dat", dirName);
    snprintf(anglPath, sizeof(anglPath), "%s/angl.dat", dirName);

    // Creating files "accl.dat", "rota.dat", "angl.dat" in the "values" directory
    int fd_accl = open(acclPath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int fd_rota = open(rotaPath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    int fd_angl = open(anglPath, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd_accl < 0 || fd_rota < 0 || fd_angl < 0) {
        perror("Failed to create files");
        return 1;
    }

    // Open the "data.dat" file
    const char* inputFileName = "data.dat";
    int input_fd = open(inputFileName, O_RDONLY);
    if (input_fd < 0) {
        perror("Failed to open input file");
        return 1;
    }

    struct DataPoint data;
    ssize_t bytesRead;

    // Read X, Y, Z and store them into their respective file
    while ((bytesRead = read(input_fd, &data, sizeof(struct DataPoint))) == sizeof(struct DataPoint)) {

        write(fd_accl, data.acceleration, sizeof(data.acceleration));

        write(fd_angl, data.angle, sizeof(data.angle));

        write(fd_rota, data.velocity, sizeof(data.velocity));
    }

    if (bytesRead < 0) {
        perror("Error reading from input file");
    }

    close(input_fd);
    close(fd_accl);
    close(fd_rota);
    close(fd_angl);

    return 0;
}