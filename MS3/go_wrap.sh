
export nodes=6
export ntasks_per_node=6
export time_limit="00:20:00"
export units=36
export output="output.csv"
export log_file="output.log"
touch $log_file
touch $output


#export test_file="new_tests/9.txt"
#	sbatch --partition=macondo --nodes=$nodes --ntasks-per-node=$ntasks_per_node go.sh
#
#
#export test_file="new_tests/14.txt"
#	sbatch --partition=macondo --nodes=$nodes --ntasks-per-node=$ntasks_per_node go.sh
#
#
#export test_file="new_tests/19.txt"
#	sbatch --partition=macondo --nodes=$nodes --ntasks-per-node=$ntasks_per_node go.sh
#
#
#
#export test_file="new_tests/24.txt"
#	sbatch --partition=macondo --nodes=$nodes --ntasks-per-node=$ntasks_per_node go.sh


export test_file="new_tests/29.txt"
	sbatch --partition=macondo --nodes=$nodes --ntasks-per-node=$ntasks_per_node go.sh


#export test_file="new_tests/26.txt"
#	sbatch --partition=macondo --nodes=$nodes --ntasks-per-node=$ntasks_per_node go.sh
 
#export test_file="new_tests/31.txt"
# 	sbatch --partition=macondo --nodes=$nodes --ntasks-per-node=$ntasks_per_node go.sh
