#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Notification of surver down successful" {

    run ./dsh -c <<EOF
EOF
    
    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="Theserverisdown.socketclientmode:addr:127.0.0.1:1337"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 1 ]
}

@test "Server and client successfully starts up, client successfully runs builtin: exit" {
    ./dsh -s &
    server_pid=$!

    run ./dsh -c <<EOF
exit
EOF
    
    kill $server_pid


    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1337dsh4>clientexited:gettingnextconnection...cmdloopreturned0"
    
    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Client successfuly runs builtin: exit (allows another connection)" {
    ./dsh -s &
    server_pid=$!

    run ./dsh -c <<EOF
exit
EOF

run ./dsh -c <<EOF
exit
EOF
    
    kill $server_pid


    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1337dsh4>clientexited:gettingnextconnection...cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Client successfuly runs builtin: stop-server" {
    ./dsh -s &
    server_pid=$!

    run ./dsh -c <<EOF
stop-server
EOF


    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1337dsh4>clientrequestedservertostop,stopping...cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Client successfuly runs builtin: cd - no args" {

    current=$(pwd)

    cd ~/tmp
    mkdir -p dsh-test
    
    ${current}/dsh -s &
    server_pid=$!

    run ${current}/dsh -c <<EOF
pwd
cd dsh-test
pwd
stop-server
EOF


    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1337dsh4>$HOME/tmpdsh4>dsh4>/home/djd439/tmp/dsh-testdsh4>clientrequestedservertostop,stopping...cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Client successfuly runs builtin: cd - 1 arg" {

    current=$(pwd)

    cd ~/tmp
    mkdir -p dsh-test
    
    ${current}/dsh -s &
    server_pid=$!

    run ${current}/dsh -c <<EOF
pwd
cd
pwd
stop-server
EOF


    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1337dsh4>$HOME/tmpdsh4>dsh4>/home/djd439/tmpdsh4>clientrequestedservertostop,stopping...cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Client successfuly runs ls" {
    ./dsh -s &
    server_pid=$!

    run ./dsh -c <<EOF
ls
exit
EOF
    
    kill $server_pid


    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1337dsh4>batsdshdsh_cli.cdshlib.cdshlib.hmakefilersh_cli.crshlib.hrsh_server.cdsh4>clientexited:gettingnextconnection...cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Client successfuly runs simple pipe: ls | grep dsh" {
    ./dsh -s &
    server_pid=$!

    run ./dsh -c <<EOF
ls | grep dsh
exit
EOF
    
    kill $server_pid


    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1337dsh4>dshdsh_cli.cdshlib.cdshlib.hdsh4>clientexited:gettingnextconnection...cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Client successfuly runs which" {
    ./dsh -s &
    server_pid=$!

    run ./dsh -c <<EOF
which which
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1337dsh4>/usr/bin/whichdsh4>clientrequestedservertostop,stopping...cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Client successfuly runs echo (with quotes spaces)" {
    ./dsh -s &
    server_pid=$!

    run ./dsh -c <<EOF
echo " hello    world  "
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '\t\n\r\f\v')

    # Expected output with all whitespace removed for easier matching
    expected_output="socket client mode:  addr:127.0.0.1:1337dsh4>  hello    world  dsh4> client requested server to stop, stopping...cmd loop returned 0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Client successfully warned if no command is given" {
    ./dsh -s &
    server_pid=$!

    run ./dsh -c <<EOF

stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d [:space:])

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1337dsh4>warning:nocommandsprovideddsh4>clientrequestedservertostop,stopping...cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Client successfully warned if too many args" {
    ./dsh -s &
    server_pid=$!

    run ./dsh -c <<EOF
ls -l -l -l -l -l -l -l -l -l
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d [:space:])

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1337dsh4>Error:Commandorargstoolargedsh4>clientrequestedservertostop,stopping...cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Client successfully warned if too many pipe commands" {
    ./dsh -s &
    server_pid=$!

    run ./dsh -c <<EOF
a | b | c | d | e | f | g | h | i | j | k | l | m | n | o | p | q | r | s | t | u | v | w | x | y | z
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d [:space:])

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1337dsh4>error:pipinglimitedto%dcommandsdsh4>clientrequestedservertostop,stopping...cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Client successfully warned if command or args too large" {
    ./dsh -s &
    server_pid=$!

    run ./dsh -c <<EOF
a | b | c | d | e | f | g | h | i | j | k | l | m | n | o | p | q | r | s | t | u | v | w | x | y | z
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d [:space:])

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1337dsh4>error:pipinglimitedto%dcommandsdsh4>clientrequestedservertostop,stopping...cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "argv doesn't spill into other commands" {
    ./dsh -s &
    server_pid=$!

    run ./dsh -c <<EOF
echo "hello"
echo
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d [:space:])

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1337dsh4>hellodsh4>dsh4>clientrequestedservertostop,stopping...cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Client notified of nonexistent command" {
    ./dsh -s &> /dev/null &
    server_pid=$!

    run ./dsh -c <<EOF
nonexistent
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d [:space:])

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1337dsh4>socketservermode:addr:0.0.0.0:1337->Single-ThreadedModerdsh-error:commandexecutionerrordsh4>clientrequestedservertostop,stopping...cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Multiple lines and pipes executed successfully" {
    current=$(pwd)

    ./dsh -s &> /dev/null &
    server_pid=$!

    run ./dsh -c <<EOF
echo jimmy
pwd
ls | grep ".c"
ls | grep .c | head -n 2
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d [:space:])

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1337dsh4>jimmydsh4>${current}dsh4>dsh_cli.cdshlib.crsh_cli.crsh_server.cdsh4>dsh_cli.cdshlib.cdsh4>clientrequestedservertostop,stopping...cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Improper command in pipe" {
    current=$(pwd)

    ./dsh -s &> /dev/null &
    server_pid=$!

    run ./dsh -c <<EOF
ls | grep .c | head -n 2 | stewart | tail -n 1
stop-server
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d [:space:])

    # Expected output with all whitespace removed for easier matching
    expected_output="socketclientmode:addr:127.0.0.1:1337dsh4>->Single-ThreadedModerdsh-error:commandexecutionerrordsh4>clientrequestedservertostop,stopping...cmdloopreturned0"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"
    echo "${stripped_output} -> ${expected_output}"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]
}

