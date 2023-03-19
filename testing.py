#!/usr/bin/env python
import csv
import subprocess

success = 0
failed = 0
open('results.txt', 'w').close()
res = open('results.txt', 'a')
line = 1
with open('ciphertext.csv') as csv_file:
    csv_reader = csv.reader(csv_file, delimiter=',')
    for row in csv_reader:
        f = open("ciphertext.txt", "w")
        f.write(row[1])
        f.close()
        call = "./kry -c -o opentext.txt -f ciphertext.txt"
        subprocess.call(call, shell=True)
        f = open("opentext.txt", "r")
        output = f.read()
        f.close()
        if output == "":
            failed = failed + 1
            res.write("failed " + str(line) + "\n")
        else:
            success = success + 1
        line = line + 1
    res.write("Success: " + str(success) + "\n")
    res.write("Failed: " + str(failed) + "\n")
    res.close()