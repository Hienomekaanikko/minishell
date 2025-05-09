#ifndef ERROR_MESSAGES_H
# define ERROR_MESSAGES_H

// Generic error messages (for use as: "minishell: <cmd>: <error>")
# define ERR_PERMISSION "Permission denied"
# define ERR_NOT_FOUND "command not found"
# define ERR_MALLOC "memory allocation failed"
# define ERR_OPEN "No such file or directory"
# define ERR_EXEC "Exec format error"
# define ERR_REDIRECT "Ambiguous redirect"
# define ERR_GETCWD_FAILED "getcwd failed"
# define ERR_INVALID_IDENTIFIER "not a valid identifier"
# define ERR_MEMORY_ERROR "memory error"
# define ERR_NOT_ENOUGH_ARGS "not enough arguments"
# define ERR_NOT_SET "not set"
# define ERR_NUMERIC_ARG_REQUIRED "numeric argument required"
# define ERR_TOO_MANY_ARGS "too many arguments"

// Syntax error messages (for use as: "minishell: <error>")
# define ERR_SYNTAX "syntax error near unexpected token"

// Signal error messages (for use as: "minishell: <cmd>: <error>")
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