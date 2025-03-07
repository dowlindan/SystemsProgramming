1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

This implementation makes sure that every child process is complete before teh shell continues accepting user input. This is accomplished by using waitpid(). If we forget to call waitpiod() on all child processes, the shell could continue to accept input, which could cause strange behavior.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

The dup2() funciton redirects input and output file descriptors to the pipe ends. It is absolutely necessary to close unused pipe ends to prevent resource leaks. If the pipes are left open, it can cause issues such as file descriptor exhausting, and the blocking of processes that are waiting for input or output. The shell can hang or exhibit unpredictable behavior.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

The cd command is a built-in because it changes the current working directory of the shell process itself. If it were implemented as an external command, it would only change the working directory of the child process and not the shell. This would make it impossible to change the shell's directory. 

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

To support any number of piped commands, we could modify the implementation to use dynamic allocation for storing pipe commands. We could use malloc to allocate an array and realloc to resize it as needed. This approach balances memory usage as well as the number of reallocations. The tradeoff ist hat it introduces complexity in managing memory and, proper cleanup is necessary to avoid memory leaks. Additionally, a pointer is needed to be maintained to the array for handling of the commands.
