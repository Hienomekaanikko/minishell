Minishell is project in 42-Network school. The subject requires creating a replicate of bash, where by running the program, it is possible to run any kind of inputs with the same outcome as bash. The project is done in teams of 2 persons, and we decided to split the workload so that one does parsing and one does execution. Towards the end, the responsibilities got more blended together as the issues we faced became fewer and fewer and we wanted to use the time as wisely as possible.

We were allowed to use libft, (our selfmade library of functions), and the following functions outside of it:

readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay, add_history, printf, malloc, free, write, access, open, read, close, fork, wait, waitpid, wait3, wait4, signal, sigaction, sigemptyset, sigaddset, kill, exit, getcwd, chdir, stat, lstat, fstat, unlink, execve, dup, dup2, pipe, opendir, readdir, closedir, strerror, perror, isatty, ttyname, ttyslot, ioctl, getenv, tcsetattr, tcgetattr, tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs

The most important function was readline(), to take the inputs and keep the prompt waiting for inputs. From there we went to parsing, where we "clean up" any kind of input in a way that bash does. It does certain things in strange way. For example "echo he'llo'" is cleaned up to "echo hello", and 'echo "echo he'llo'"' is cleaned up to 'echo "echo hello"'. But these are just the simplest examples.

Example 1: echo he'llo', OUTPUT: hello
Example 2: 'echo "echo he'llo'"', OUTPUT: echo hello

After parsing, we go to lexing. Lexing is where we decide what is the role of each part in the input. First "word" in the input is always command (CMD), unless it is redirection-in (RE_IN), redirection-out (RE_OUT), append-out (APPEND_OUT), here-doc (HERE_DOC) or pipe (PIPE). Second word is an ARG, unless its one of the previously mentioned. What comes after redirection, we flag as ARG, but we'll read it as the file for the redirection. First words on either side of pipes are regarded as commands.

Then with the linked list created in the lexing, an AST-tree is created. We create this so that in execution phase, it is as simple as possible to recursively execute the entire input.

echo <"./test_files/infile" <missing <"./test_files/infile

grep hi <./test_files/infile_big <./test_files/infile
  
The one at the top (in both cases RE_IN), serves as the root node, even though it's the last in the commandline. So we traverse it backwards in the execution phase. This is how we set-up the redirections correctly, as the last RE_OUT, RE_IN, APPEND_OUT or HERE_DOC is the one that stays as the infile or outfile. When we face a redirection in the tree, we'll know that the right child of it is the file related to it. Then with dup2(), we update STDIN or STDOUT to that file. We also use a flag to mark that an infile or outfile has already been set, so we will not redirect towards the latter ones of same category. There are quite interesting twists in this, for example if we have an input like "cat >missing1 >missing2 >outfile <Makefile", the contents of the Makefile will only go to the outfile (if it does not exist, it will be created), but also the missing1 and missing2 will be created, though they will stay empty. Here-doc will be created during the creation of AST-tree, where we will create a temp-file with the lines we inputted.

Then we have to handle expansions. For example keys $USER, $PWD etc. If input has those keys, it needs to be expanded to the value attached to the key. It's found in the environment (just type env, and you'll see the environment copied by us). Also here_doc must expand those keys. The only way to disable expansion is to use single quotes around the input, and in the case of here-doc, you can just close the delimiter to quotes. <<"EOF" or <<'EOF' or <<E'O'F etc. If inputting a $INVALID, it will output empty line. UNLESS you do > $INVALID, it'll print error message "Unambiguous input". There are so many of these little details in this project.

This was by far the hardest project so far but very rewarding. We passed it on a second try. In the first attempt there was a leak that is not allowed. There must be no leaks, and no open file descriptors in the parent process nor in any of the child processes.
