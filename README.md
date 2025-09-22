cat Makefile | grep ".c" > file1 > file2 > file3
![Flowchart](https://github.com/user-attachments/assets/3fb39e3d-8d27-4f0a-b689-13f017ed6cab)

So, Minishell. The notorious 42-network project. This surely was a hard one, as it really put to test everything we had learned so far in Hive. 
The project is first one that was done in pairs, so it was important to understand the basics of git and it's functionalities fast. It was nice 
to have daily repetition on making branches and adding features, combined with communication with the team mate of what had been done and what 
we're working on etc. Felt a bit like what work life could possible be. How we approached the team work was that we set goals and areas that each
of us were working on for a week or so, and then when we had something to add for the main, we did a merge to put things together. From that we 
took new branches with new set of tasks to complete.

The project itself is to make a program that runs like bash. It includes a lot of functionalities, including basic commands, expansions, redirections, pipes, 
here_doc, builtin functions etc. The list felt endless. So, we split the work into two categories: one of us started with working on parsing (me), and my team-mate
started to work on execution. We decided on the basic structure of the project really fast, which was basically that I parse the input (which has insane little details, i don't know what the bash-makers were thinking...), and then I send the parsed input into lexer, which decides what is the role of each part in the input. Lexer outputs a linked list
with value (the actual part of the line) and type of the input (CMD, ARG, RE_IN, RE_OUT, APPEND_OUT, HERE_DOC), which then goes in to ast-builder (abstract syntax tree). Ast-builder creates a binary tree which can be then recursively read by the executioner. So I send the root-node of the AST-tree to my team-mate, and his side then executes everything. 

The program does clean-up after every "input-loop", but it leaves the background processes untouched. Meaning that if local or environment variables have been created, they
stay until the minishell have been exited. During the program the variables can be removed by doing "unset" + removable variable. The cleanup was probably hardest to get right, because no file descriptors were allowed to be left open (except for std fds), and no memory leaks or still reachable/unreachables were allowed. It got really hard with error
handling, so that if even one malloc would fail, that if would safely inform about the failure and return to the mainloop without crashing and leaks. So all mallocs had to be 
intricately checked. 

And one more thing, the program had to give exactly the same exit-code and error-message as bash in every situation. No permissions for a file would be 126, and non existant 
file 127. Syntax errors 2, signal interruptions 130, basic malloc error for example 1. There were so many of those. Signals had to be handled the same way as bash, which proved
to be hard around the here_doc creation. Mostly for the cleanup also. But we made it through all 3 evaluations by the peers (second try. We forgot to clean up memory on the first try if "cd non_existant_path" was inputted which resulted in instant failure) which was a huge relief, finally we can move on to next ones. If you have any questions of the project let me know!
