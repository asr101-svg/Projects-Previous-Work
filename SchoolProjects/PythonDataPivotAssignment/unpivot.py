#!/usr/bin/env python

'''
unpivot.py


  Project: Lab Assignment 5 Script (Iteration 0)
  Date of Last Update: Feb 15, 2024.

  Functional Summary
      unpivot.py takes a table and unpivots it based on the given arguments

      The file represents the names of people in the population
      for a particular year of birth in the United States of America.
      Officially it is the "National Data on the relative frequency
      of given names in the population of U.S. births where the individual
      has a Social Security Number".

     Commandline Parameters: 2
        argv[1] = name of the input file containing the names
        argv[2] = The N amount you want to pivot by
        argv[3] = Header name for start of the row
        argv[4] = Cell value name for start of the row
     References
        Name files from http://www.ssa.gov/OACT/babynames/limits.html
'''

#
#   Packages and modules
#

# The 'sys' module gives us access to system tools, including the
# command line parameters, as well as standard input, output and error
import sys

# The 'csv' module gives us access to a tool that will read CSV
# (Comma Separated Value) files and provide us access to each of
# the fields on each line in turn
import csv

#
# Define any "constants" for the file here.
# Names of constants should be in UPPER_CASE.
#

def main(argv):
    '''
    Main function in the script. Putting the body of the
    script into a function allows us to separate the local
    variables of this function from the global constants
    declared outside.
    '''
    #Check that we have been given the right number of parameters,
    #and store the single command line argument in a variable with
    #a better name
    
    if len(argv) != 5:
        print("Missing arguments")
        sys.exit(1)

    fileName = argv[1]
    columnKeep = int(argv[2])
    header = argv[3]
    values = argv[4]

    #
    # Open the name data input file.  The encoding argument
    # indicates that we want to handle the BOM (if present)
    # by simply ignoring it.
    #
    try:
        yobFile = open(fileName, encoding="utf-8-sig")

    except IOError as err:
        # Here we are using the python format() function.
        # The arguments passed to format() are placed into
        # the string it is called on in the order in which
        # they are given.
        print("Unable to open names file '{}' : {}".format(
                fileName, err), file=sys.stderr)
        sys.exit(1)

    #
    # Create a CSV (Comma Separated Value) reader based on this
    # open file handle.  We can use the reader in a loop iteration
    # in order to access each line in turn.
    #
    yobReader = csv.reader(yobFile)
    yobNames = []
    v = 0

    #putting yob file name and gender into table
    
    for row_data_fields in yobReader:
        yobNames.append(row_data_fields)#appending the entire row into one element
    for i in range(0,columnKeep):#for the first row in the table
        if (i < columnKeep):
            print("%s" % yobNames[0][i], end = ",")
    print("%s,%s" % (header,values))
    
    for i in range(1,len(yobNames)):
        for n in range(columnKeep,len(yobNames[i])): #goes through each row
            if yobNames[i][n] != "": #if the value is not blank
                for j in range(0,columnKeep):#for the amount of things you want "unpivoted"
                    print("%s," % yobNames[i][j], end = "")
                print("%s,%s" % (yobNames[0][n],yobNames[i][n]))
    #
    #   End of Function
    #
    
##
## Call our main function, passing the system argv as the parameter
##
main(sys.argv)


#
#   End of Script
#
