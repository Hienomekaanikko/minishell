*** Settings ***
Library    Process
Library    String
Library    Collections

*** Variables ***
@{BASIC}
...    echo hello
...    echo hello world
...    pwd
...    /bin/echo hi

@{PIPES}
...    echo hello | cat
...    echo hello | grep h
...    echo hello | wc -c

@{REDIRECTIONS}
...    echo hi > file1; cat file1
...    echo hi >> file1; cat file1
...    cat < file1

@{ERRORS}
...    nonexistent
...    ls nonexistentfile
...    cat nonexistentfile

@{COMBINED}
...    echo hello | grep h > out; cat out
...    echo hi > a; cat a | wc -c

@{QUOTES}
...    echo "hello world"
...    echo 'hello world'
...    echo "a   b"
...    echo "$HOME"
...    echo '$HOME'

@{SPACING}
...    echo     hello
...       echo hello
...    echo hello    

@{EDGE}
...    echo
...    echo ""
...    echo ''
...    false
...    true
...    false | true
...    true | false
...    echo hi > a > b

*** Keywords ***
Run In Both Shells
    [Arguments]    ${input}

    ${mini}=    Run Process    bash    -c    "printf '%s\n' \"${input}\" | ./minishell"    stdout=PIPE    stderr=PIPE
    ${bash}=    Run Process    bash    -c    "${input}"    stdout=PIPE    stderr=PIPE

    ${mini_out}=    Normalize    ${mini.stdout}
    ${bash_out}=    Normalize    ${bash.stdout}

    ${mini_err}=    Normalize    ${mini.stderr}
    ${bash_err}=    Normalize    ${bash.stderr}

    # Normalize common prefix differences
    ${mini_err}=    Replace String    ${mini_err}    minishell:    bash:

    Log    COMMAND: ${input}
    Log    MINI OUT: ${mini_out}
    Log    BASH OUT: ${bash_out}
    Log    MINI ERR: ${mini_err}
    Log    BASH ERR: ${bash_err}

    Should Be Equal    ${mini_out}    ${bash_out}
    Should Be Equal    ${mini_err}    ${bash_err}
    Should Be Equal As Integers    ${mini.rc}    ${bash.rc}

Normalize
    [Arguments]    ${text}
    ${text}=    Replace String    ${text}    \r    ${EMPTY}
    ${text}=    Strip String    ${text}
    RETURN    ${text}

Run Command List
    [Arguments]    @{commands}
    FOR    ${cmd}    IN    @{commands}
        Run In Both Shells    ${cmd}
    END

*** Test Cases ***
Basic Commands
    Run Command List    @{BASIC}

Pipes
    Run Command List    @{PIPES}

Redirections
    Run Command List    @{REDIRECTIONS}

Errors
    Run Command List    @{ERRORS}

Combined Cases
    Run Command List    @{COMBINED}

Quoting
    Run Command List    @{QUOTES}

Spacing
    Run Command List    @{SPACING}

Edge Cases
    Run Command List    @{EDGE}