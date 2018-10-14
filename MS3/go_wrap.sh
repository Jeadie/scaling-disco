
export nodes=2
export ntasks_per_node=4
export time_limit="00:20:00"
export units=8
export output="output.csv"
export log_file="output.log"
touch $log_file
touch $output


export test_file="new_tests/32.txt"
	sbatch --partition=macondo --nodes=$nodes --ntasks-per-node=$ntasks_per_node go.sh


export test_file="new_tests/34.txt"
	sbatch --partition=macondo --nodes=$nodes --ntasks-per-node=$ntasks_per_node go.sh

#export test_file="new_tests/26.txt"
#	sbatch --partition=macondo --nodes=$nodes --ntasks-per-node=$ntasks_per_node go.sh
 
#export test_file="new_tests/31.txt"
# 	sbatch --partition=macondo --nodes=$nodes --ntasks-per-node=$ntasks_per_node go.sh
