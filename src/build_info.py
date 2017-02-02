import datetime
import sys
import argparse

#reding file and store that in array
with open('BUILD.DAT') as file:
    file_array = [row.strip() for row in file]

#set current date
def setDate(file_array):
    now_date = datetime.date.today()
    file_array[0] = now_date.month
    file_array[1] = now_date.day
    file_array[2] = now_date.year

    return file_array

#create parser console argument
def createParser():
    parser = argparse.ArgumentParser()
    parser.add_argument('major', action = 'store', const = 1, nargs = '?', default = False)
    parser.add_argument('minor', action = 'store', const = 1, nargs = '?', default = False)
    parser.add_argument('revision', action = 'store', const = 1, nargs = '?', default = False)

    return parser
 
#choise console argument
if __name__ == '__main__':
    parser = createParser()
    args = parser.parse_args()
    if args.major:
        file_array[3] = int (file_array[3])
        file_array[3] += 1
    if args.minor:
        file_array[4] = int (file_array[4])
        file_array[4] += 1
    if args.revision:
        file_array[5] = int (file_array[5])
        file_array[5] += 1

setDate(file_array)

#writing data in file
file = open('BUILD.DAT', 'w')

for i in file_array:
    file.write(str(i) + '\n')

file.close()
