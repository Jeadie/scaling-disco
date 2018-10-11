
export nodes=1
export ntasks_per_node=4
export time_limit="00:10:00"
export units=4
export output="output.csv"
export log_file="output.log"
touch $log_file
touch $output



for filename in new_tests/{5..10}.txt; do
	export test_file="$filename"
	sbatch --partition=macondo --nodes=$nodes --ntasks-per-node=$ntasks_per_node go.sh
	python generate_time.py --file="$log_file" --nodes=$nodes --ntasks-per-node=$ntasks_per_node --output_csv="$output"
	echo "$filename"
done


for slurm in ./slurm*.out; do 
	python generate_time.py --file="$slurm" --nodes=$nodes --ntasks-per-node=$ntasks_per_node --output_csv="$output"
	echo "$slurm"
done 

rm ./slurm* 
