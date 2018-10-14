import argparse
import csv
import time 

def write_to_output(f, nodes, ntasks, output): 
	
	flag = False
#	while not flag: 
#		fi= open(f, "r")
#		lines = ["DONE" in line for line in fi.readlines()] 
#		if True in lines: 
#			flag = True
#
#		else: 
#			time.sleep(5)
		
	f = open(f, "r")
	averages = []
	master = 0
	work = 0
	final = 0
	graph_size = 0 

	for line in f.readlines(): 
		if "graph" in line: 
			graph_size = int(line.split(":")[-1])
		if "master preprocessing. " in line: 
			master = float(line.split("master preprocessing. ")[-1][:-2])
		if "worker preprocessing. " in line: 
			work = float(line.split("worker preprocessing. ")[-1][:-2])

		if "slave" in line: 
			averages.append(float(line.split("slave: ")[-1]))

		if "Complete time:" in line: 
			final = float(line.split("Complete time: ")[-1][:-2])


	average = sum(averages) / float(len(averages))
	print("average, master, work, final, graph_size")
	print(average, master, work, final, graph_size)

	with open(output,'a') as fd:
		writer = csv.writer(fd)
		writer.writerow([nodes, ntasks, graph_size, final, average, work])


def main(): 
	parser = argparse.ArgumentParser()
	parser.add_argument("--file")
	parser.add_argument("--nodes")
	parser.add_argument("--ntasks-per-node")
	parser.add_argument("--output_csv") 

	args = parser.parse_args()	

	write_to_output(args.file, args.nodes, args.ntasks_per_node, args.output_csv)

if __name__ == '__main__': 
	main()

