cat Makefile | grep ".c" > file1 > file2 > file3

So, Minishell. The notorious 42-network project. This surely was a hard one, as it really put to test everything I had learned so far in Hive. 
The project is first one that was done in teams of two persons, so it was important to understand the basics of git and it's functionalities fast. It was nice 
to have daily repetition on making branches and adding features, combined with communication with the team mate of what had been done and what 
we're working on etc. Felt a bit like what work life could possible be. How we approached the team work was that we set goals and areas that each
of us were working on for a week or so, and after we had something that compiled, we merged the project. From there we 
took new branches with new set of tasks to complete. (We ended up with A LOT of commits and branches as we were still pretty new to git :D ).

So we sketched the basic idea like this:

![Flowchart-3](https://github.com/user-attachments/assets/5b9732e4-aa8a-4c5a-a668-6e4aa2ef95ec)

There are two main features: parsing and excecution.

The main goal of the assignment was to make a program that runs like bash. It includes a lot of functionalities, including basic commands, expansions, redirections, pipes, 
here_doc, builtin functions etc. The list felt endless. So initially, we split the work in half: one of us started with working on parsing (me), and my team-mate
started to work on execution. We decided on the basic structure of the project really fast, which was basically that I parse the input, make an AST of it, and hand over the root node of that for the excecution.

The program does clean-up after every "input-loop", but it leaves the background processes untouched. Meaning that if local or environment variables have been created, they
stay until the minishell have been exited. During the program the variables can be removed by doing "unset" + removable variable. The cleanup was probably hardest to get right, because no file descriptors were allowed to be left open (except for std fds), and no memory leaks or still reachable/unreachables were allowed. It got really hard with error
handling, so that if even one malloc would fail, that if would safely inform about the failure and return to the mainloop without crashing and leaks. So all mallocs had to be 
intricately checked. 

And one more thing, the program had to give exactly the same exit-code and error-message as bash in every situation. No permissions for a file would be 126, and non existant 
file 127. Syntax errors 2, signal interruptions 130, basic malloc error for example 1. There were so many of those. Signals had to be handled the same way as bash, which proved
to be hard around the here_doc creation. Mostly for the cleanup also. But we made it through all 3 evaluations by the peers (second try. We forgot to clean up memory on the first try if "cd non_existant_path" was inputted which resulted in instant failure) which was a huge relief, finally we can move on to next ones. If you have any questions of the project let me know!
