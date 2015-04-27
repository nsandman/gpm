# rmfgpi.py - Noah Sandman
# 04/26/2015

# This script was extracted #
# from the old GPM (Python) # 
# -Noah                     #
from os import path
from sys import argv

def delete_gpi(to_open, which_run, look_for):
	# Open file with read access only
	v = open(to_open, 'r')
	lines = v.readlines() # Open the file line by line
	v.close() # Close for the next open
	# Now open file with write access
	v = open(to_open, 'w')
	for line in lines:
		if line != look_for:
			v.write(line) # Write line back if it *doesn't* equal the line to remove
	v.close() # Close the file
	return 0 # Return error code 0