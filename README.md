# NanoShell

<br>
NanoShell is the clone of the Bash shell. It is written in C, and supports various features that we can see in a traditional Bash shell. 

&nbsp;

## Instructions To Run The Shell

#### 1. Clone the repo and navigate to the folder
```sh
git clone https://github.com/vedanivas/NanoShell.git
cd NanoShell/
```
#### 2. Compile the code and run the executable
```sh
make
```

#### 3. Input Commands
Once the code is up and running, the commands for the included features can be executed. Multiple commands can also be entered in a single line, separated by `;`. Random spaces and tabs in commands are accounted for too.

&nbsp;
## Features

#### Command Prompt

NanoShell has a command prompt that is similar to the one in a Bash shell. The prompt is coloured and is displayed in the form of `<username@system_name:curr_dir>`.
The root directory is the one from which the shell is started.


#### 1. Command Pipelines

NanoShell supports piping with commands. One or more commands can be piped using `|`, and any number of pipes are supported.

```sh
cat <filename.txt> | head -7 | tail -5
```

#### 2. Input/Output Redirection

NanoShell also provides for input/output redirection of commands. The output of commands can be redirected to another file or command instead of stdout, the input can be taken from a file other than stdin, or the output of a command can be appended to a file. These redirections are done using the `<`, `>`, and `>>` symbols.  
Both input and output redirections can be used simultaneously too. 

```sh
echo "hey" > output.txt
sort < output.txt > sorted.txt
```


 #### 3. Input/Output Redirection within Command Pipelines

Input/Output redirection can be implemented within command pipelines.  

```sh
cat < in.txt | wc -l > lines.txt
```

&nbsp;
## Built-In Commands

#### 1. cd
Changes the current directory to another directory specified and displays an error if the specified directory does not exist.
`.`, `..`, `~`, and `-` are all valid directory names.

```sh
cd <path_to_destination_directory>
```


#### 2.  pwd
Ouputs the path of the current working directory.

```sh
pwd
```


#### 3.  echo
Prints the specified message on the terminal. It accounts for any random spaces and tabs.

```sh
echo <message>
```


#### 4.  ls
Lists all the files and directories in the given directory. It handles multiple directories as arguments. Flags can be used to provide more functionalities to the command.  
 
```sh
ls <path_to_directory> 
```
##### Flags supported  
- `-a`: Lists all the files and directories in the given directory, along with symbolic links and hidden files and folders.

```sh
ls -a <path_to_directory> 
```
- `-l`: Lists all the files and directories in the given directory with detailed information.

```sh
ls -l <path_to_directory> 
```

These flags can be used together in different orders while giving the intended output.

#### 5. history
Diplays the history of the commands run. If no argument is specified, the last 10 commands are listed. The history persists even after the shell is exited.

```sh
history <no_of_commands>
```



#### 6. pinfo
Displays information about the process, including its PID, status, memory, and executable path.

```sh
pinfo <PID>
```
Running just `pinfo` without any arguments will display information about the shell program.



#### 7. repeat
Executes the given instruction multiple times. The first argument to the command specifies the number of times the following command is to be run.

```sh
repeat <no_of_times> <input_command>
```

#### 8. fg
Brings the running or stopped background job corresponding to the job number​ to the foreground and changes its state to ​running​.

```sh
fg <job_number>
```


#### 9. bg
Changes the state of a stopped background job to running (in the background). It has no effect on foreground processes and already running background processes.

```sh
bg <job_number>
```


#### 10. jobs
Prints a list of all currently running background processes spawned by the shell in alphabetical order of the command name, along with
their job numbers, process IDs, and states (either running or stopped).

```sh
jobs
```
##### Flags supported  
- `-r`: Prints only the running processes.

```sh
jobs -r
```
- `-s`: Prints only the stopped processes.

```sh
jobs -s
```

These flags can be used at the same time in different orders while giving the intended output.


#### 11. sig

Takes the job number of a running job and sends the signal corresponding to ​signal number​signal number​ to that process.

```sh
sig <job_number> <signal_number>
```

#### 12.  replay
Executes a particular command in fixed time interval for a certain period 
 
```
replay -command <input_command> -interval <time_interval> -period <time_period>
```

All the flags here are required and can be implemented in any order. 
Background processes are also supported. 

#### 13.  baywatch
##### Flags supported  
- `-n`: The time interval in which to execute the command periodically.

##### Commands

All three commands run until the key `q` is pressed.

- `interrupt`: Prints the number of times the CPU(s) has(ve) been interrupted by the keyboardcontroller (i8042 with ​IRQ 1​).

```sh
baywatch -n <time_interval> interrupt
```
- `newborn`: Prints the PID of the process that was most recently created on the system.

```sh
baywatch -n <time_interval> newborn
```
- `dirty`: Prints the size of the part of the memory that is dirty.

```sh
baywatch -n <time_interval> dirty
```

&nbsp;
## Processes

There are two kinds of processes that can be run in NanoShell.
#### 1. Foreground Processes
To run processes in the foreground, just enter the command and the process will start up.
Magush will wait for this process to complete and regain control when this process exits.
If the given command is not found, an error is displayed. 
For example, `vim` and `gedit` are foreground processes.

```sh
<input_command>
```

#### 2. Background Processes
To run processes in the background, enter an `&` after the command and the process will start up.
Magush will spawn the process and doesn't wait for it to exit. It will keep taking other user commands.
After the background process exits, the shell displays the exit status on the terminal.

```sh
<input_command> &
``` 
or 
```
<input_command>&
```


 &nbsp;
## Signal Handling

NanoShell allows the handling of signal, each of which serves a different purpose.


#### 1. Ctrl-Z

Pushes any currently running foreground job into the background and changes its state from running to stopped by sending the **SIGTSTP**. It has no effect on the shell if there is no foreground process running.

#### 2. Ctrl-C

Interrupts any currently running foreground job by sending the **SIGINT** signal. It has no effect on the shell if there is no foreground process running.

#### 3. Ctrl-D

Log the user out of the shell, without having any effect on the actual terminal.



