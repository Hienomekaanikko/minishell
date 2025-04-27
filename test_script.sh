#!/bin/bash

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
	"hel'llo what'sasasda"
)

for CMD in "${COMMANDS[@]}"; do
	BASH_OUTPUT=$(echo "$CMD" | bash 2>&1 | sed 's/^bash: line 1: //')
	MINISHELL_OUTPUT=$(echo -e "$CMD\nexit" | ./minishell 2>&1 | sed '/^minishell\$/d')

	echo "=== Command: $CMD ==="
	echo "Bash Output (filtered):"
	echo "$BASH_OUTPUT"
	echo "Minishell Output (filtered):"
	echo "$MINISHELL_OUTPUT"
	echo

	if [ "$BASH_OUTPUT" == "$MINISHELL_OUTPUT" ]; then
		echo "✅ Outputs match."
	else
		echo "❌ Outputs differ."
	fi
	echo
done
