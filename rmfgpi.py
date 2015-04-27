# rmfgpi.py - Noah Sandman
# 04/26/2015

# This script was extracted #
# from the old GPM (Python) # 
# -Noah                     #
def delete_gpi(a, b):
	# Open file with read access only
	v = open(a, 'r')
	f = v.readlines() # Open the file line by line
	v.close() # Close for the next open
	# Now open file with write access
	v = open(a, 'w')
	for g in f:
		if g != b:
			v.write(line) # Write line back if it *doesn't* equal the line to remove
	v.close() # Close the file
	return 0 # Return error code 0
