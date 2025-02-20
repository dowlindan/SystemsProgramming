1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**: Fork creates a new process, and allows the parent to keep running while the child executes the command. Using fork with execvp allows us to handle multiple command surnning currently.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  If work fails it will return a negative value, which indicates the system is unable to make a new process. Here an error is printed and the command is not attempted to execute.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  execvp looks through the directories through the PATH environment variables.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  The wait() function ensures that the parent waits for the child proces to finish before contiuing. Without it, the parent might continue executing while the child runs, and this brings up resource management issues.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  WEXITSTATIS() gets the exit status of the child process. It is important to determine if the child process was successful for error handling.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  build_cmd_buff() handles quote arguments by using a state machine that determines if a quote was encountered. This is necessary because we have to know if we want to replace a space with a null terminator, or keep it literally as it is

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  The changes with the parsing logic included omitting the previous pipe parsing logic, and adjusting to the new struct. Some unexpected challenges included having to handle whitespace better.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**:  Signals in Linux are used for communication between processes. This is important for handling events like interrupts or termination. This differes from IPC like because IPC allows for more complex and bidirectional communication. 

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  SIGKILL terminates a process and cannot be caught or ignored, and is typically used to kill unresponsive processes. SIGTERM is used to request termination of a process more gracefully, which allows cleanup before exiting. SIGINT is used whent he user interrupts a process, typically by pressing Ctrl+C, to ask for the process to be terminated

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  SIGSTOP pauses a process's execution and cannot be caught or ignored, as it is a signal that immediately stops the process's execution state. SIGINT is able to be processed.
