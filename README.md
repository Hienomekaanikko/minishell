# Minishell (42-Network Project)

> **Minishell** is a project from the 42-Network school. The goal is to create a minimal shell replicating bash-like behavior, able to process and execute user inputs just like bash.

## 🧑‍💻 Teamwork & Workflow

- **Team of 2:**  
  - One focused on **parsing**
  - One focused on **execution**
- **Workload blended** towards the end for efficiency and bug-fixing.

## 📚 Allowed Functions

You may use your own **libft** and the following system functions:

```
readline, rl_clear_history, rl_on_new_line, rl_replace_line, rl_redisplay, add_history, printf, malloc, free, write, access, open, read, close, fork, wait, waitpid, wait3, wait4, signal, sigaction, sigemptyset, sigaddset, kill, exit, getcwd, chdir, stat, lstat, fstat, unlink, execve, dup, dup2, pipe, opendir, readdir, closedir, strerror, perror, isatty, ttyname, ttyslot, ioctl, getenv, tcsetattr, tcgetattr, tgetent, tgetflag, tgetnum, tgetstr, tgoto, tputs
```

## 📝 Parsing & Input Handling

- **Main function:** `readline()`  
  - Reads user input and displays prompt.
- **Parsing:**  
  - Cleans up input to mimic bash behavior.
  - Example:
    - Input: `echo he'llo'` → Output: `echo hello`
    - Input: `'echo "echo he'llo'"'` → Output: `echo hello`

```
# Example 1
Input:  echo he'llo'
Output: hello

# Example 2
Input:  'echo "echo he'llo'"'
Output: echo hello
```

## 🧩 Lexing & AST Construction

- **Lexing:**  
  - Assigns roles to each token (CMD, ARG, RE_IN, RE_OUT, APPEND_OUT, HERE_DOC, PIPE).
- **AST (Abstract Syntax Tree):**  
  - Built from the lexed tokens for recursive execution.
  - Redirections and pipes are represented as tree nodes.

### 📊 AST Visualization

#### Example 1:  
`echo <"./test_files/infile" <missing <"./test_files/infile"`

```
        RE_IN
       /     \
    RE_IN     ./test_files/infile
   /     \
RE_IN    missing
/    \
echo  ./test_files/infile
```

#### Example 2:  
`grep hi <./test_files/infile_big <./test_files/infile`

```
      RE_IN
     /     \
  RE_IN     ./test_files/infile
 /     \
grep   ./test_files/infile_big
```

## 🔄 Execution Phase

- **Tree traversal:**  
  - Start from the root (last redirection).
  - Set up redirections using `dup2()`.
  - Only the last redirection of each type is effective.
- **Redirection example:**
  - `cat >missing1 >missing2 >outfile <Makefile`
    - Only `outfile` receives the output.
    - `missing1` and `missing2` are created but remain empty.

## 💡 Expansions

- **Environment variables:**  
  - `$USER`, `$PWD`, etc. are expanded from the environment.
  - Expansion is disabled with single quotes.
- **Here-documents:**  
  - Expansion can be controlled with quoting the delimiter:  
    - `<<"EOF"` or `<<'EOF'`
- **Invalid variables:**  
  - `$INVALID` → empty line
  - `> $INVALID` → error: `Unambiguous input`

## 🧪 Edge Cases & Details

- **No memory leaks** allowed.
- **No open file descriptors** in parent or child processes.
- **Here-doc** creates a temp file during AST creation.

## 🎉 Reflections

> This was by far the hardest project so far but also the most rewarding.  
> We passed on the second try (first attempt failed due to a memory leak).

## 📈 Project Diagram

```
graph TD
    A[Input (readline)] --> B[Parsing]
    B --> C[Lexing]
    C --> D[AST Construction]
    D --> E[Execution (Recursive)]
    E --> F[Redirections & Expansions]
    F --> G[Output]
```

## 🚀 Example Usage

```
$ ./minishell
minishell$ echo 'Hello, $USER!'
Hello, $USER!
minishell$ echo "Hello, $USER!"
Hello, johndoe
minishell$ cat < Makefile > output.txt
```

**Feel free to copy-paste this directly into your GitHub README.md!**
```
