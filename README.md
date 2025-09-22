So, Minishell. This surely was a hard one, as it really put to test everything I had learned so far in Hive. 
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

**PARSING! :)**

So, lets say we have an input

`echo "hello" > outfile `

It needs to execute the command `echo`, taking `"hello"` as an argument, and handle `> outfile` as directing the output of the command into the outfile.

So first we have to see what is inside the prompt:

![Flowchart-4](https://github.com/user-attachments/assets/8d1e14cc-1392-423c-8d07-2211746a6c79)

Now as it can be seen, the first step is to take the input and separate portions that have their own purpose. I made this functionality that can be found from parsing.c, parsing_utils.c and parsing_utils2.c file. It has been perfected in a way that it knows to correctly slice crazy inputs like

`echo <<<<<<<<< infile >>>>>>>>>> outfile >>>> outfile2` 

![Flowchart-5](https://github.com/user-attachments/assets/3f70c563-5d18-447f-8101-7551ac087905)

or something more regular but a bit different like:

`echo 'So this is how bash parses things "hahaha not really" but it really really does' | grep "i" > outfile`.

![Flowchart-6](https://github.com/user-attachments/assets/483cbc49-82dd-4add-9680-58ded657a859)

`echo        \n\n\n      "So this is how bash parses things 'hahaha not really' but it really really does' | | grep 'i'         >            outfile`

![Flowchart-7](https://github.com/user-attachments/assets/ef241044-700f-4938-81a0-33a95542946a)

The input must always be properly split and cleaned up.

**TOKENIZATION :O**

So now, we go into lexing and tokenizing. First we have to know what kind of information we are looking for. Here are the "operations" that are in my opinion (you may have a different opinion) pretty much the core of bash.

`|` = `PIPE`

`<` = `RE_IN`

`>` = `RE_OUT`

`<<` = `HERE_DOC`

`>>` = `APPEND_OUT`

`word` = `CMD`

`word` = `ARG`


For claritys sake, lets have the same example as before:

`echo "hello" > outfile `

We must give each word of this array of strings (remember, it has been split now!), a name and make it a token. The name will describe the purpose of the word.

![Flowchart-10](https://github.com/user-attachments/assets/a21365e9-a458-4bce-b1b5-8a43e27e1f4b)

By the way, this must behave exactly the same way:

`echo > oufile "hello"`

![Flowchart-11](https://github.com/user-attachments/assets/e15a25ef-b70b-4fb6-a2f4-c1eb704eacca)

Oh and this too! So much fun (not):

`> outfile "hello" echo`

![Flowchart-12](https://github.com/user-attachments/assets/5023627e-778f-4d76-9cad-742edb44aaff)

So, what can be clearly seen here, is that bash is super agile with making sense of the weirdest orders of commands.

**EXPANSIONS**

During tokenization, in case there are ANY `$`-signs inside the input, the stuff that follows it until next whitespace or \0, goes to expansion.
For example `echo $ANIMAL > outfile`. If in environment (can be seen with cmd `env`), there is ANIMAL = 'vinnie the pooh' The tokenized command becomes `echo 'vinnie the pooh' > outfile`.

To add stuff into the environment you can simply do `export ANIMAL='vinnie the pooh'`.'

**AST MADNESS**

Ok, now when we have cleaned and tokenized the input, we now have it as a linked list. Each node contains the value (for example "hello" or "|"), and it's **TYPE** (ARG, CMD, RE_IN, RE_OUT, APPEND_OUT, HERE_DOC or PIPE).

We must build a binary tree. 

How I approached it was to look for first redirection or a pipe, and make it the root node. If there were no such things, then the first word become she root node and it's right child becomes the arguments for the command.

`echo "hello"`

![Flowchart-16](https://github.com/user-attachments/assets/802eafae-79a9-415f-928f-77b6f9152123)

`echo "hello" > outfile`

![Flowchart-15](https://github.com/user-attachments/assets/d92bef75-b632-437d-ba9d-939812d83524)

`echo <"./files/infile" | cat <"./files/infile"`

![Flowchart-17](https://github.com/user-attachments/assets/80c3afca-d39c-4e99-abbb-72ee8e99f445)

**SPECIAL HANLDING REQUIRED: HERE_DOC**

In a case of here_doc, we create it during the creation of AST. When facing `<<`, it becomes a root node, and the right child will be the delimiter. (whatever comes after `<<`).
Then we go into here_doc creation. Whatever is inputted in this stage will get saved in a **temp** file, and it's path will replace the delimiter at the right child of the `<<` node.

For example: `cat <<EOF >> file.txt`

So this:

![Flowchart-18](https://github.com/user-attachments/assets/7e2ce94a-94c3-4ebf-8312-2028686f83d2)

Becomes:

![Flowchart-19](https://github.com/user-attachments/assets/03725bda-dfd3-49ed-b1e2-21facc142316)

Little fun details about the here_doc behavior:

If the delimiter is closed by quotes like this (which is pretty broad idea in bash... :D):

`<<'EOF'` or `<<'E'OF` or `<<"EOF"` or `<<EO"F"` or any such kind of invention that really isnt closed quotes but bash thinks it is, 

any attempts to expand stuff ($) will not happen inside the here_doc. Otherwise expansions work just normally. You can actually have the delimiter as an expandable too:

`<<$USER`

**EXECUTION**

Now we have the AST, so lets execute. It basically goes through the tree starting from the root node. The tree is in reverse, meaning that the first outfiles and first infiles
will really be the last ones, so we can save those and redirect the output and input streams accordingly, and skip all the following infiles and outfiles. With outfiles,
there is a fun nyance: even if the outfile has already been chosen, all the others will still be created and left empty. With infiles there's no such silly thing. If there is a pipe anywhere, the output situations are separated between the processes.

Oh and this is where we check file permissions. If there are any trouble with the files, we will send exact same error message as bash, clean up, and return to the main prompt.

But here is the basic idea of the execution:

`cat >>EOF > outfile2 > outfile1` if there would be any arguments for the command, they would be places with the command during step4.

![Flowchart-20](https://github.com/user-attachments/assets/4b949f5d-2d7a-499a-9bb8-fbcf8dea5e46)

The input and output streams are controlled with dup() and dup2(), 
The execution is done with execve(). It is called like, execve(cmd_path, cmd+arguments as array of strings, envp).

So first we setup the input and output streams as the AST guides, and then execute.

The program does clean-up after every "input-loop", but it leaves the background processes untouched. Meaning that if local or environment variables have been created, they
stay until the minishell have been exited. During the program the variables can be removed by doing "unset" + removable variable. The cleanup was probably hardest to get right, because no file descriptors were allowed to be left open (except for std fds), and no memory leaks or still reachable/unreachables were allowed. It got really hard with error
handling, so that if even one malloc would fail, that if would safely inform about the failure and return to the mainloop without crashing and leaks. So all mallocs had to be 
intricately checked. 

And one more thing, the program had to give exactly the same exit-code and error-message as bash in every situation. No permissions for a file would be 126, and non existant 
file 127. Syntax errors 2, signal interruptions 130, basic malloc error for example 1. There were so many of those. Signals (ctrl+D etc.) had to be handled the same way as bash, which proved to be hard around the here_doc creation. Mostly for the cleanup also. But we made it through all 3 evaluations by the peers (second try. We forgot to clean up memory on the first try if "cd non_existant_path" was given which resulted in instant failure) which was a huge relief, finally we can move on to next projects. If you have any questions of the project let me know!
