#include <iostream>
#include <string.h>
#include <thread>
#include <mutex>
#include <semaphore>
#include <signal.h>
#include <unistd.h>
#include "CircularBuffer.h"

#define UNUSED(arg) (void)(arg)

#define  FRAME_SIZE 1316
#define  RAND_50_MOD (rand() % 50)

#define USE_THREADS

int signalReceived      =0;
CircularBuffer* c_buff  =NULL;
static bool streaming   =false;

void sighandler(int signal)
{
    signalReceived = signal;
    printf("signal received, Terminating!!!\n");
    exit(0);
}


void run_write_thread(CircularBuffer* c_buffer, std::mutex* rd_wr)
{
    UNUSED(rd_wr);
    uint8_t temp_buffer_in[(int)2e3]; //2k Buffer
    unsigned read_bytes;
    FILE *TS_file = fopen("bb.ts", "rb");
    if (!TS_file)
        perror("Error opening TS file\n");
    streaming = true;
    printf("Reading TS file\n");
    while(!feof(TS_file))
    {
    read_bytes = FRAME_SIZE + RAND_50_MOD;
    fread(temp_buffer_in, sizeof(uint8_t), read_bytes, TS_file);
    // rd_wr->lock();
    // resource.acquire();
    c_buffer->write(temp_buffer_in, read_bytes);
    // resource.relase();
    // rd_wr->unlock();
    usleep(1e-9);
    }
    streaming = false;
    printf("TS file reading stopped\n");
}

void run_read_thread(CircularBuffer* c_buffer, std::mutex* rd_wr)
{
    UNUSED(rd_wr);
    uint8_t temp_buffer_out[(int)2e3];
    FILE *TS_out = fopen("output_with_thread.ts", "wb");
    if (!TS_out)
        perror("Error creating TS file\n"); 
    while(streaming)
    {
        // rd_wr->lock();
        // resource.acquire();
        c_buffer->read(temp_buffer_out, FRAME_SIZE);
        // resource.release();
        // rd_wr->unlock();
        usleep(1e-9);
        fwrite(temp_buffer_out, sizeof(uint8_t), FRAME_SIZE, TS_out);
    }
    printf("Stoping write to TS file\n");
}

int main (int argc, char** argv)
{
    c_buff = new CircularBuffer((size_t) 100e6); //100 M buffer
    if(!c_buff)
        perror("Error Creating Circular Buffer");
    else
        printf("Circular Buffer Created Successfully.\n");
    #ifdef USE_THREADS
        std::mutex rd_wr;
        std::thread write_thread(run_write_thread, c_buff, &rd_wr);
        std::thread read_thread(run_read_thread, c_buff, &rd_wr);
        write_thread.join();
        read_thread.join();
    #else
        uint8_t temp_buffer_in[(int)2e3]; //2K buffers
        uint8_t temp_buffer_out[(int)2e3];
        unsigned read_bytes;
        signal(SIGINT, sighandler);
        printf("Creating Circular Buffer\n");
        printf("Opening TS file\n");
        FILE *TS_file = fopen("bb.ts", "rb");
        if (!TS_file)
            perror("Error opening TS file\n");
        FILE *TS_out = fopen("output.ts", "wb");
        if (!TS_out)
            perror("Error creating TS file\n");
        printf("Starting Loop\n");
        while(!feof(TS_file) && !signalReceived)
        {
            try
            {
                read_bytes = FRAME_SIZE + RAND_50_MOD;
                fread(temp_buffer_in, sizeof(uint8_t), read_bytes, TS_file);
                c_buff->write(temp_buffer_in, read_bytes);
                c_buff->read(temp_buffer_out, FRAME_SIZE);
                fwrite(temp_buffer_out, sizeof(uint8_t), FRAME_SIZE, TS_out);
            }
            catch(...)
            {
                perror("Exception, Error occured!\n");
            }
        }
    #endif
    printf("Loop terminated.\n");
    return 0;
}