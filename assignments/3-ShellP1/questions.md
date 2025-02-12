1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  fgets() is a good choice because it gets at most one line of input

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  Dynamically allocating memory for cmd_buff is necessary because it is repeatedly used, allowing more flexibility and preventing stack overflows.


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  Trimming leading and trailing space is necessary because parsing arguments is based on spaces, so different parsing errors can occur without ensuring the trailing and leading spaces are removed

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**:  Redirection examples: ls > outputfile.txt, echo "Hello" >> file, gcc foo.c> error.log. Challenges with implementing these redirection examples could include ensuring that the correct output is used, whether it be stdin, stdout, or sterr, as well as the idea of rerouting output in general especially if printf is used throughout our program.

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  Redirection allows control of input and output by sending data to and from files. Piping connects the output of one command directly to the input of another

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  It is important to keep these separate most importantly to not confuse the error. It is crucial for one to know whether their program is producing OK output or if there is an error present.

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**: It seems that the best way to handle errors from commands that fail should have a way to merge stdout and stderr together. Going down this route it would be crucial that error messages have a unique and clearly identifiable signature so the user knows it is an error. Perhaps using printerr from the C library could help. Additionally, stdout output should still display if there is an error, showing that merging is the best way.