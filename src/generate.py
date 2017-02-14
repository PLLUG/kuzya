import datetime
import sys
import argparse

#set current date
def setDate(date):
    now_date = datetime.date.today()
    date.append(now_date.month)
    date.append(now_date.day)
    date.append(now_date.year)

    return date

#transform string data in int
def parsArg(string):
    int_array = []
    integ = ''

    for n in string:
        if n.isdigit():
            integ += n
        else:
            integ += ' '
    int_array = integ.split()

    return int_array

#create parser console argument
def createParser():
    parser = argparse.ArgumentParser()
    parser.add_argument('--version', '-v', dest = 'version', action = 'store', type = str)

    return parser

versArray = [1, 2, 3]
#pars console argument
if __name__ == '__main__':
    parser = createParser()
    args = parser.parse_args()
    if args.version:
        versArray = parsArg(args.version)

nowDate = []
setDate(nowDate)

#writing version.h
file = open('version.h', 'w')

file.write('#ifndef VERSION_H')
file.write('\n#define VERSION_H')
file.write('\n')
file.write('\n#define MAJORVER {}'.format(versArray[0]))
file.write('\n#define MINORVER {}'.format(versArray[1]))
file.write('\n#define REVISION {}'.format(versArray[2]))
file.write('\n#define BUILD_DAY {}'.format(nowDate[1]))
file.write('\n#define BUILD_MONTH {}'.format(nowDate[0]))
file.write('\n#define BUILD_YEAR {}'.format(nowDate[2]))
file.write('\n')
file.write('\n#endif')

file.close()
