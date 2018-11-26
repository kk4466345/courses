#!/bin/bash
# Basic while loop
counter=1
while [ $counter -le 10 ]
do
wget https://www.cse.iitk.ac.in/users/nsrivast/CS252/lec0$counter.pdf
echo $counter
((counter++))
done
echo All done
