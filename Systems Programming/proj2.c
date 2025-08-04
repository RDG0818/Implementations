#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>     
#include <unistd.h>    
#include <stdint.h>    
#include <string.h>     

struct DataPoint {
    double acceleration[3];  
    double velocity[3];     
    double angle[3];  
};

struct DataPoint decompress(const int8_t packet[20]) {
    struct DataPoint data;

    // Combine the bytes to form signed 16-bit integers
    for (int i = 0; i < 3; i++) {

        int16_t accelVal = (int16_t)((packet[(2 * i) + 2] & 0xFF) | (packet[(2 * i) + 3] << 8));
        data.acceleration[i] = accelVal / 32768.0 * 16;

        int16_t velocityVal = (int16_t)((packet[(2 * i) + 8] & 0xFF) | (packet[(2 * i) + 9] << 8));
        data.velocity[i] = velocityVal / 32768.0 * 2000;

        int16_t angleVal = (int16_t)((packet[(2 * i) + 14] & 0xFF) | (packet[(2 * i) + 15] << 8));
        data.angle[i] = angleVal / 32768.0 * 180;
    }

    return data;
}

int main() {
    const char* inputFileName = "raw.dat"; 
    const char* outputFileName = "data.dat";  
    int count = 0;

    int input_fd = open(inputFileName, O_RDONLY);
    if (input_fd < 0) {
        perror("Failed to open input file");
        return 1;
    }
    
    // Open the output file using file descriptors (create if it doesn't exist)
    int output_fd = open(outputFileName, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (output_fd < 0) {
        perror("Failed to open output file");
        close(input_fd);
        return 1;
    }

    int8_t packet[20];  // Buffer for reading the 20-byte data packet
    struct DataPoint data;  

    ssize_t bytesRead;
    while ((bytesRead = read(input_fd, packet, sizeof(packet))) == sizeof(packet)) {
        count++;
        printf("Packet %d\n", count);

        data = decompress(packet);

        // Print decompressed data to the console 
        printf("Acceleration: Ax = %.8f Ay = %.8f Az = %.8f\n",
               data.acceleration[0], data.acceleration[1], data.acceleration[2]);
        printf("Angular Velocity: Wx = %.8f Wy = %.8f Wz = %.8f\n",
               data.velocity[0], data.velocity[1], data.velocity[2]);
        printf("Angle: Roll = %.8f Pitch = %.8f Yaw = %.8f\n\n",
               data.angle[0], data.angle[1], data.angle[2]);

        // Write decompressed data to the output binary file
        ssize_t bytesWritten = write(output_fd, &data, sizeof(struct DataPoint));
        if (bytesWritten != sizeof(struct DataPoint)) {
            perror("Failed to write to output file");
            close(input_fd);
            close(output_fd);
            return 1;
        }
    }

    if (bytesRead < 0) {
        perror("Error reading from input file");
    }

    close(input_fd);
    close(output_fd);

    return 0;
}