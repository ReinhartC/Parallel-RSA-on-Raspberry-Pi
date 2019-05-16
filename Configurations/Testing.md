# Raspberry Pi Nodes Testing
***(Do the following on master node)***

## Testing the nodes connection
1. Ping from each node to each other with `ping IPADDRESS` with the IP address of each other nodes
2. SSH from the **master node** to the slave nodes, and from the **slave nodes** to the master node with `ssh pi@HOSTNAME` with the hostname of each other nodes. If the SSH is successful without password prompt, then the connection setup is successful.
3. Create a test folder
4. Create a `machinefile` and insert the IP addresses of all the nodes in it. *E.g.*
```
192.168.0.50
192.168.0.51
192.168.0.52
```
5. Create the CPI test with `nano cpi.c` and insert the following:
```c
/* (C) 2001 by Argonne National Laboratory. */

#include "mpi.h"
#include <stdio.h>
#include <math.h>

double f(double);

double f(double a)
{
    return (4.0 / (1.0 + a * a));
}

int main(int argc, char *argv[])
{
    int n, myid, numprocs, i;
    double PI25DT = 3.141592653589793238462643;
    double mypi, pi, h, sum, x;
    double startwtime = 0.0, endwtime;
    int namelen;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Get_processor_name(processor_name, &namelen);

    fprintf(stdout, "Process %d of %d is on %s\n", myid, numprocs, processor_name);
    fflush(stdout);

    n = 10000;
    if (myid == 0)
        startwtime = MPI_Wtime();

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    h = 1.0 / (double) n;
    sum = 0.0;

    for (i = myid + 1; i <= n; i += numprocs) {
        x = h * ((double) i - 0.5);
        sum += f(x);
    }
    mypi = h * sum;

    MPI_Reduce(&mypi, &pi, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (myid == 0) {
        endwtime = MPI_Wtime();
        printf("pi is approximately %.16f, Error is %.16f\n", pi, fabs(pi - PI25DT));
        printf("wall clock time = %f\n", endwtime - startwtime);
        fflush(stdout);
    }

    MPI_Finalize();
    return 0;
}
```
6. Compile it with `mpicc -o cpi cpi.c`.
7. Then, run it with `mpirun -f machinefile -n X ./cpi` and change the `X` with the amount of nodes in the cluster.
8. It should output with a response from every node. *E.g.*
```
Process 0 of 3 is on master_node
Process 1 of 3 is on slave_node_1
Process 2 of 3 is on slave_node_2
pi is approximately 3.1415926544231318, Error is 0.0000000008333387
wall clock time = 0.006050
```
9. Check if all of the nodes that are included in the machinefile are on the output.
10. If all is fulfilled, the parallel raspberry cluster with MPICH is successfully set up.


<p align="center">
    <a href="https://github.com/ReinhartC/Parallel-RSA-on-Raspberry-Pi/tree/master/Configurations">
        Return to Configurations
    </a>  
</p>