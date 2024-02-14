#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/times.h>
#include "../include/tarjan.h"

int main(int argc, char *argv[])
{
    // Parse parameters
    if (argc != 2)
    {
        fprintf(stderr, "Usage:\n\t%s [num_vertices]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    int num_vertices = atoi(argv[1]);

    // Check number of vertices
    if (num_vertices != 24000 && num_vertices != 32000 && num_vertices != 41000 && num_vertices != 48000)
    {
        fprintf(stderr, "The number of vertices must be 24000 or 32000 or 8000 or 41000 or 48000\n");
        exit(EXIT_FAILURE);
    }

    int *buffer;
    FILE *in;
    FILE *out;
    char in_file_name[100];
    char out_file_name[100];
    int in_size = 0;
    int out_size = 0;
    sprintf(in_file_name, "../data/seq/graph%d.bin", num_vertices);
    sprintf(out_file_name, "../data/seq/result-graph%d.bin", num_vertices);

    struct tms read_start_times;
    clock_t read_start_etime;
    read_start_etime = times(&read_start_times);

    in = fopen(in_file_name, "rb");
    if (in == NULL)
    {
        fprintf(stderr, "Error opening file %s\n", in_file_name);
        exit(EXIT_FAILURE);
    }
    fseek(in, 0, SEEK_END);
    in_size = ftell(in) / sizeof(int);
    rewind(in);
    buffer = (int *)malloc(in_size * sizeof(int));
    fread(buffer, sizeof(int), in_size, in);
    fclose(in);
    Graph *g = from_buffer(buffer, num_vertices);

    struct tms read_end_times;
    clock_t read_end_etime;
    read_end_etime = times(&read_end_times);

    struct tms tarjan_start_times;
    clock_t tarjan_start_etime;
    tarjan_start_etime = times(&tarjan_start_times);

    SccStack result = tarjan(g);

    struct tms tarjan_end_times;
    clock_t tarjan_end_etime;
    tarjan_end_etime = times(&tarjan_end_times);

    struct tms write_start_times;
    clock_t write_start_etime;
    write_start_etime = times(&write_start_times);

    out = fopen(out_file_name, "wb");
    if (out == NULL)
    {
        fprintf(stderr, "Error opening file\n");
        exit(EXIT_FAILURE);
    }
    int separator = -1;
    fwrite(&result.numSCC, sizeof(int), 1, out);
    while (!SccStackIsEmpty(&result))
    {
        SCC s = SccStackPop(&result);
        for (int i = 0; i < s->nodes.length; i++)
        {
            fwrite(&s->nodes.items[i]->id, sizeof(int), 1, out);
        }
        fwrite(&separator, sizeof(int), 1, out);
        destroySCC(s);
    }
    fclose(out);

    struct tms write_end_times;
    clock_t write_end_etime;
    write_end_etime = times(&write_end_times);

    long clktck = 0;
    if ((clktck = sysconf(_SC_CLK_TCK)) < 0)
    {
        fprintf(stderr, "ERROR: filed to get slock ticks per sec\n");
        exit(EXIT_FAILURE);
    }

    double read_elapsed = (read_end_etime - read_start_etime) / (double)clktck;
    double tarjan_elapsed = (tarjan_end_etime - tarjan_start_etime) / (double)clktck;
    double write_elapsed = (write_end_etime - write_start_etime) / (double)clktck;
    double elapsed = read_elapsed + tarjan_elapsed + write_elapsed;
    printf("%d,%d,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\n", num_vertices, 0, read_elapsed, tarjan_elapsed, 0.000, 0.000, write_elapsed, elapsed);

    free(buffer);
    SccStackDestroy(&result);
    graphDestroy(g);

    exit(EXIT_SUCCESS);
}