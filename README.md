# Multithreaded MapReduce

The purpose of this program is to implement *word count statistics* application in multi-threading. The program contains a master thread, one producer thread and number of consumers as an argument. The producer and consumers will share a queue for data transferring. To synchronize the threads, mutex locks and condition signals were used around global variables.  The master thread initializes the shared queue, histogram, producer and consumer threads. The producer thread will read the input file, split the data into smaller pieces and feed into the shared queue. The consumer will read a line from the shared queue and finish the word count statistics. After producer and all consumers complete their work, the master thread writes the final result into the output file. This is a group project and I worked with one partner.

In order to compile the program, run the shell command *make* inside the *src* directory. *make clean* will remove any files generated by the compilation process. You should type the following syntax as the commend line after *make* : "*./wcs #consumer filename [option] [#queue_size]*". Note that you have to include the file path, including "/" at the beginning of the filename(file path).

The example of the commend line :

* Executing without log and using unbounded buffer -
"*./wcs 5 /testing/test3.txt*"

* Executing with log and using unbounded buffer
"*./wcs 5 /testing/test3.txt -p*"

* Executing without log and using bounded buffer
"*./wcs 5 /testing/test3.txt -b 2*"

* Executing with log and using bounded buffer
"*./wcs 5 /testing/test3.txt -bp 2*"
