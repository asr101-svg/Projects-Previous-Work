#!/usr/bin/env python

'''
find_first_names.py


  Project: Lab Assignment 2 Script (Iteration 0)
  Date of Last Update: Jan 25, 2024.

  Functional Summary
      find_first_names.py takes in a CSV (comma separated version) file, as well as a file only consisting of names
      and prints out the names from the file consisting only of names with whether or not the name was found.
      If the name is found, it will print the line number or "ranking".
      If it is not found, it will print 0 beside the name.

      There are expected to be three fields:
          1. name
          2. sex (only F or M recorded in this US census data)
          3. number of people with this name

      The file represents the names of people in the population
      for a particular year of birth in the United States of America.
      Officially it is the "National Data on the relative frequency
      of given names in the population of U.S. births where the individual
      has a Social Security Number".

     Commandline Parameters: 2
        argv[1] = name of the input file containing the names
        argv[2] = name of the input file containing the names to search for

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
    #if len(argv) <= 2:
        #print("Usage: read_file.py <file name>")
        # we exit with a zero when everything goes well, so we choose
        # a non-zero value for exit in case of an error
        #sys.exit(1)
    #elif len(argv) != 5:
        #print("Missing file in argument")
        #sys.exit(1)

    fileName = argv[1]
    columnKeep = (int)argv[2]
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
    lineOne = []

    #putting yob file name and gender into table
    for row_data_fields in yobReader:
        nData = []#creating empty array
        nData.append(row_data_fields[0]) #appending the name
        nData.append(row_data_fields[1]) #appending the gender
        nData.append([])#creating an empty array, this will store the multiple frequencies
        for i in range(2,len(row_data_fields)):
            nData[2].append(row_data_fields[i])
        yobNames.append(nData)#appending all the data obtained to a "main" array.
    
    for i in yobNames:
        lineOne.append([])
        for j in range(0,columnKeep):
            lineOne[0].append(yobNames[j])
    
    printFormat = printFormat + "%s,%s"
    for i in yobNames:
        print(printFormat %)
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
