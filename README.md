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



The program does clean-up after every "input-loop", but it leaves the background processes untouched. Meaning that if local or environment variables have been created, they
stay until the minishell have been exited. During the program the variables can be removed by doing "unset" + removable variable. The cleanup was probably hardest to get right, because no file descriptors were allowed to be left open (except for std fds), and no memory leaks or still reachable/unreachables were allowed. It got really hard with error
handling, so that if even one malloc would fail, that if would safely inform about the failure and return to the mainloop without crashing and leaks. So all mallocs had to be 
intricately checked. 

And one more thing, the program had to give exactly the same exit-code and error-message as bash in every situation. No permissions for a file would be 126, and non existant 
file 127. Syntax errors 2, signal interruptions 130, basic malloc error for example 1. There were so many of those. Signals had to be handled the same way as bash, which proved
to be hard around the here_doc creation. Mostly for the cleanup also. But we made it through all 3 evaluations by the peers (second try. We forgot to clean up memory on the first try if "cd non_existant_path" was inputted which resulted in instant failure) which was a huge relief, finally we can move on to next ones. If you have any questions of the project let me know!
