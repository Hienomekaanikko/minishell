#!/bin/bash

# List of commands to run
COMMANDS=(
	"echo hello"
	"pwd"
	"ls -l"
	"echo world"
	"echo 'world'"
	"he'he'he"
	"'arg1 arg2'"
	"'asd'ert''"
	"''asd'ert'"
	"'''asd''ert'"
	"'''asd''er''t'"
	"''''''''asd''''''''"
	"asdasdasdas'rhrrh12'"
	"'wegweggewgw''''wegewg'"
	"'faefeaf  rgwrgwrg''wgwrg 'wrg''''"
)

# Run the commands in both bash and minishell, capturing their outputs
for CMD in "${COMMANDS[@]}"; do
    # Run command in Bash and filter out "bash: line 1: "
    BASH_OUTPUT=$(echo "$CMD" | bash 2>&1 | sed 's/^bash: line 1: //')

    # Run command in Minishell and filter out lines starting with "minishell$" (including leading spaces)
    MINISHELL_OUTPUT=$(echo -e "$CMD\nexit" | ./minishell 2>&1 | sed '/^minishell\$/d')

    # Optional: show the outputs
    echo "=== Command: $CMD ==="
    echo "Bash Output (filtered):"
    echo "$BASH_OUTPUT"
    echo "Minishell Output (filtered):"
    echo "$MINISHELL_OUTPUT"
    echo

    # Compare outputs
    if [ "$BASH_OUTPUT" == "$MINISHELL_OUTPUT" ]; then
        echo "✅ Outputs match."
    else
        echo "❌ Outputs differ."
    fi
    echo
done
