#include <stdio.h>
#include <stdlib.h>
#include <openmpi/mpi.h>
#include "../include/tarjan.h"
#include "../include/THTopen.h"

int main(int argc, char *argv[])
{
    // Get process rank and size
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Check number of processes
    if (size != 1 && size != 2 && size != 4 && size != 8)
    {
        if (rank == 0)
            fprintf(stderr, "The number of processes must be 1 or 2 or 4 or 8\n");
        MPI_Finalize();
        exit(EXIT_FAILURE);
    }

    // Parse parameters
    if (argc != 2)
    {
        if (rank == 0)
            fprintf(stderr, "Usage:\n\t%s [num_vertices]\n",
                    argv[0]);
        MPI_Finalize();
        exit(EXIT_FAILURE);
    }
    int num_vertices = atoi(argv[1]);

    // Check number of vertices
    if (num_vertices != 24000 && num_vertices != 32000 && num_vertices != 41000 && num_vertices != 48000)
    {
        if (rank == 0)
            fprintf(stderr, "The number of vertices must be 24000 or 32000 or 41000 or 48000\n");
        MPI_Finalize();
        exit(EXIT_FAILURE);
    }

    char in_file_name[100];
    sprintf(in_file_name, "../data/par/graph%d/size%d/process%d.bin", num_vertices, size, rank);
    char out_file_name[100];
    sprintf(out_file_name, "../data/par/graph%d/size%d/result.bin", num_vertices, size);

    double read_start_time = MPI_Wtime();
    MPI_File in;
    MPI_File_open(MPI_COMM_WORLD, in_file_name, MPI_MODE_RDONLY, MPI_INFO_NULL, &in);
    MPI_Offset in_size;
    MPI_File_get_size(in, &in_size);
    int *buffer = (int *)malloc(in_size);
    MPI_File_read(in, buffer, in_size / sizeof(int), MPI_INT, MPI_STATUS_IGNORE);
    MPI_File_close(&in);
    Graph *subgraph = from_buffer(buffer, num_vertices / size);
    double read_end_time = MPI_Wtime();

    double tarjan_start_time = MPI_Wtime();
    SccStack stack = tarjan(subgraph);
    double tarjan_end_time = MPI_Wtime();

    MPI_Barrier(MPI_COMM_WORLD);

    double communication_start_time = MPI_Wtime();
    int *data = get_info(&stack);
    int num_int = data[0];
    int num_scc = data[1];
    int *num_int_buf = malloc(sizeof(int) * size);
    int *num_scc_buf = malloc(sizeof(int) * size);

    MPI_Gather(&num_int, 1, MPI_INT, num_int_buf, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Gather(&num_scc, 1, MPI_INT, num_scc_buf, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int *displs = NULL;
    int *rcv_buf = NULL;

    if (rank == 0)
    {
        int dim_rcv_buf = 0;
        displs = malloc(size * sizeof(int));
        displs[0] = 0;
        dim_rcv_buf += num_int_buf[0];

        for (int i = 1; i < size; i++)
        {
            dim_rcv_buf += num_int_buf[i];
            displs[i] = displs[i - 1] + num_int_buf[i - 1];
        }

        rcv_buf = malloc(sizeof(int) * dim_rcv_buf);
    }

    MPI_Gatherv(data + 2, num_int, MPI_INT, rcv_buf, num_int_buf, displs, MPI_INT, 0, MPI_COMM_WORLD);
    double communication_end_time = MPI_Wtime();

    double aggregation_start_time = 0;
    double aggregation_end_time = 0;
    double write_start_time = 0;
    double write_end_time = 0;

    if (rank == 0)
    {
        aggregation_start_time = MPI_Wtime();
        int bp = 0;
        int tot_scc = 0;
        for (int i = 0; i < size; i++)
        {
            tot_scc += num_scc_buf[i];
        }
        Graph *final = graphCreate(tot_scc);
        THT *pairs = HTcreate(num_vertices);
        int dim_scc, degree_scc, id = 1000, elem, *components;
        for (int i = 0; i < tot_scc; i++)
        {
            dim_scc = rcv_buf[bp++];
            degree_scc = rcv_buf[bp++];
            components = (int *)malloc(sizeof(int) * dim_scc);
            id++;
            for (int j = 0; j < dim_scc; j++)
            {
                elem = rcv_buf[bp++];
                components[j] = elem;
                HTinsert(pairs, elem, id);
            }
            Node super = superNodeCreate(id, degree_scc, components, dim_scc);
            for (int k = 0; k < degree_scc; k++)
            {
                super->adj[k] = rcv_buf[bp++];
            }
            addNode(final, super, i);
        }

        for (int i = 0; i < final->V; i++)
        {
            Node current = final->nodes[i];
            for (int j = 0; j < current->degree; j++)
            {
                int *super_id = HTsearch(pairs, current->adj[j]);
                if (*super_id == final->nodes[i]->id)
                {
                    current->adj[j] = -1;
                }
                else
                {
                    current->adj[j] = *super_id;
                }
            }
        }

        SccStack result = tarjan(final);
        aggregation_end_time = MPI_Wtime();

        write_start_time = MPI_Wtime();
        FILE *out;
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
                Node node = s->nodes.items[i];
                for (int j = 0; j < node->dim; j++)
                {
                    fwrite(&node->components[j], sizeof(int), 1, out);
                }
            }
            fwrite(&separator, sizeof(int), 1, out);
            free(s);
        }
        fclose(out);
        write_end_time = MPI_Wtime();

        HTdestroy(pairs);
        graphDestroy(final);
        SccStackDestroy(&result);
    }

    double read_time = read_end_time - read_start_time;
    double tarjan_time = tarjan_end_time - tarjan_start_time;
    double communication_time = communication_end_time - communication_start_time;
    double aggregation_time = aggregation_end_time - aggregation_start_time;
    double write_time = write_end_time - write_start_time;

    double global_read_time, global_tarjan_time, global_communication_time, global_aggregation_time, global_write_time;
    MPI_Reduce(&read_time, &global_read_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&tarjan_time, &global_tarjan_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&communication_time, &global_communication_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&aggregation_time, &global_aggregation_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Reduce(&write_time, &global_write_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    double global_elapsed = global_read_time + global_tarjan_time + global_communication_time + global_aggregation_time + global_write_time;

    if (rank == 0)
    {
        printf("%d,%d,%.3f,%.3f,%.3f,%.3f,%.3f,%.3f\n", num_vertices, size, global_read_time, global_tarjan_time, global_communication_time, global_aggregation_time, global_write_time, global_elapsed);
    }

    free(buffer);
    graphDestroy(subgraph);
    SccStackDestroy(&stack);
    free(data);
    free(num_int_buf);
    free(num_scc_buf);
    free(displs);
    free(rcv_buf);

    return MPI_Finalize();
}