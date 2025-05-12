#ifndef ERROR_MESSAGES_H
# define ERROR_MESSAGES_H

// Custom error messages

# define ERR_NOT_ENOUGH_ARGS "not enough arguments"
# define ERR_NOT_SET "not set"
# define ERR_NUMERIC_ARG_REQUIRED "numeric argument required"
# define ERR_TOO_MANY_ARGS "too many arguments"
# define ERR_INVALID_IDENTIFIER "not a valid identifier"
# define ERR_SYNTAX "syntax error"
# define ERR_MALLOC "memory allocation failed"
# define ERR_REDIRECT "redirection failed"
# define ERR_PIPE "pipe creation failed"
# define ERR_BROKEN_PIPE "broken pipe"
# define ERR_PERMISSION "Permission denied"
# define ERR_NOT_FOUND "No such file or directory"
# define ERR_IS_DIR "is a directory"

// Signal error messages
# define ERR_SIGINT "Interrupted"
# define ERR_SIGSEGV "Segmentation fault"
# define ERR_SIGBUS "Bus error"
# define ERR_SIGFPE "Floating point exception"
# define ERR_SIGILL "Illegal instruction"
# define ERR_SIGTERM "Terminated"
# define ERR_SIGKILL "Killed"
# define ERR_SIGPIPE "Broken pipe"
# define ERR_SIG_UNKNOWN "Unknown signal"

// Prefix for error messages
# define ERR_PREFIX "minishell: "

#endif