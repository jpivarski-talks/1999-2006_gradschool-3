#!/usr/bin/env python

import os,sys,string

def n_lines(_map_file_):
	do = "cat "+_map_file_+"| wc -l"
	return string.atoi(os.popen(do).read())

def split_map_file(_map_file_):
	lines=n_lines(_map_file_)/2
	do = "cat "+_map_file_+"| head -n "+"%s " % lines
	job=do+"> "+_map_file_+"_top"
	os.system(job)
	# do other half
	lines=n_lines(_map_file_)-n_lines(_map_file_)/2
	do = "cat "+_map_file_+"| tail -n "+"%s " % lines
	job=do+"> "+_map_file_+"_bottom"
	os.system(job)
	
# main program
if __name__ == '__main__':
	split_map_file(sys.argv[1])



