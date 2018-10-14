
export nodes=2
export ntasks_per_node=4
export time_limit="00:20:00"
export units=8
export output="output.csv"
export log_file="output.log"
touch $log_file
touch $output






for slurm in ./slurm*.out; do 
	python generate_time.py --file="$slurm" --nodes=$nodes --ntasks-per-node=$ntasks_per_node --output_csv="$output"
	echo "$slurm"
done 

