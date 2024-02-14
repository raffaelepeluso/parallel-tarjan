NMEASURES=40

ARRAY_SIZE=(24000 32000 41000 48000)
ARRAY_PROC_NUM=(0 1 2 4 8)
ARRAY_OPT=(0 1 2 3)

trap "exit" INT

SCRIPT_PATH=$( cd -- "$(dirname "$0")" > /dev/null 2>&1 ; pwd -P)

for opt in "${ARRAY_OPT[@]}"; do
    for size in "${ARRAY_SIZE[@]}"; do
        for num_proc in "${ARRAY_PROC_NUM[@]}"; do
            
            np_string=$(printf "%02d" $num_proc)
            OUTPUT_FILE=$SCRIPT_PATH/measures/size$size/OPT_$opt/SIZE-$size-NP-$np_string-OPT$opt.csv

            mkdir -p $(dirname $OUTPUT_FILE) 2> /dev/null

            if [[ $num_proc -eq 0 ]]; then
                echo "Generating measures sequential version of $size verticies graph with optimization $opt"
            else
                echo "Generating $(basename $OUTPUT_FILE) measures with optimization $opt"
            fi

            echo "size_graph,NP,read_time,tarjan_time,communication_time,aggregation_time,write_time,global_time" > $OUTPUT_FILE

            for ((i = 0; i < NMEASURES; i++)); do
                if [[ $num_proc -eq 0 ]]; then
                    $1/seq_tarjan$opt $size >> $OUTPUT_FILE
                    printf "\r> %d/%d %3.1d%% " $(expr $i + 1) $NMEASURES $(expr \( \( $i + 1 \) \* 100 \) / $NMEASURES)
					printf "#%.0s" $(seq -s " " 1 $(expr \( $i \* 40 \) / $NMEASURES))
                else
                    mpirun -np $num_proc $1/mpi_tarjan$opt $size >> $OUTPUT_FILE
                    printf "\r> %d/%d %3.1d%% " $(expr $i + 1) $NMEASURES $(expr \( \( $i + 1 \) \* 100 \) / $NMEASURES)
					printf "#%.0s" $(seq -s " " 1 $(expr \( $i \* 40 \) / $NMEASURES))
                fi
            done
            printf "\n"
        done
    done
done