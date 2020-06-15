//Question 1 - Solution

//A C program to implement the command line shell 

//including the necessary header files.
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<readline/readline.h>
#include<readline/history.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>


int recieve_input(char* str)
// Function To Read Input From The Command Line 
{

    char* input_string;
    char* username = getenv("USER");
    printf("\n@%s", username);
    input_string = readline(">>>");

    if (strlen(input_string) != 0) 
    {
        strcpy(str, input_string);
        add_history(input_string);
        return 0;
    } 
    else 
    {
        return 1;
    }

}

void promptd()
//Printing the Present Working Directory along with 
//the shell prompt for User's reference-

{

    char pwdprompt[200];

    getcwd(pwdprompt, sizeof(pwdprompt));

    printf("\nPWD: %s", pwdprompt);

}



void comm_exe(char** commands)
// Function for executing simple OS commands. (Not Piped Commands)
{

    // Creating a child process using fork()
    pid_t proc_id = fork();

    if (proc_id == -1) {

        printf("\nFailed forking child..");

        return;

    } else if (proc_id == 0) {

        if (execvp(commands[0], commands) < 0) {

            printf("\nCommand Execution Failed");
        }

        exit(0);

    } else {

        wait(NULL); // Wait for the child process to end execution

        return;

    }

}

void piped_comm_exe(char** commands, char** piped)
// Function for executing piped OS commands. (Piped Command Execution)
{

    //pipeDesc is used to return two file descriptors referring to the ends of the pipe where
    //0 = Read End of Pipe
    //1 = Write End of Pipe 

    int pipeDesc[2];

    pid_t proc1, proc2;

    if (pipe(pipeDesc) < 0) {

        printf("\nPipe could not be initialized");

        return;

    }

    //Creating 1st Child Process for piped command

    proc1 = fork();

    if (proc1 < 0) {

        printf("\nCould not fork");

        return;

    }

    if (proc1 == 0) {

        // Child 1 executing writing at the write end

        close(pipeDesc[0]);

        dup2(pipeDesc[1], STDOUT_FILENO);

        close(pipeDesc[1]);

        if (execvp(commands[0], commands) < 0) {

            printf("\nCould not execute command 1..");

            exit(0);

        }

    } else {

        //Creating 2nd Child Process for piped command
        proc2 = fork();

        if (proc2 < 0) {

            printf("\nCould not fork");

            return;

        }

        // Child 2 executing (reading at the read end)

        if (proc2 == 0) {

            close(pipeDesc[1]);

            dup2(pipeDesc[0], STDIN_FILENO);

            close(pipeDesc[0]);

            if (execvp(piped[0], piped) < 0) {

                printf("\nCould not execute command 2..");

                exit(0);

            }

        } else {

            // parent executing, waiting for the execution of two children processes

            wait(NULL);

            wait(NULL);

        }

    }

}


// Help Manual For Shell

void os_help()

{

    puts("\n*******************************"
        
        "\n***Welcome To The Help Menu***"

        "\n*******************************"

        "\n\nA Prototype made by Devesh"

        "\n\nList of Available Commands:"

        "\n1.cd"

        "\n2.ls"

        "\n3.mkdir"

        "\n4.rmdir"

        "\n5.rm"

        "\n6.cp"

        "\n7.touch"

        "\n8.cat"

        "\n9.date"

        "\n> The commands can be pipelined too!");

}


// Function to execute builtin LINUX commands on the shell

int Command_hndlr(char** parsed_comm)

{

    int No_Comm = 3, i, Argument_Switch = 0;

    char* Cmd_list[No_Comm];

    char* username;

    Cmd_list[0] = "exit";

    Cmd_list[1] = "cd";

    Cmd_list[2] = "help";


    for (i = 0; i < No_Comm; i++) {

        if (strcmp(parsed_comm[0], Cmd_list[i]) == 0) 
        {

            Argument_Switch = i + 1;

            break;

        }

    }

    switch (Argument_Switch) 
    {

    case 1:

        printf("\nGoodbye\n");

        exit(0);

    case 2:

        chdir(parsed_comm[1]);

        return 1;

    case 3:

        os_help();

        return 1;

    default:

        break;

    }

    return 0;

}



int find_pipe(char* buf, char** bufpiped)
// function for checking whether input commands are piped or not
// returns 1 if pipe is found.
// returns 0 if no pipe is found.
{

    int i;

    for (i = 0; i <= 1; i++) {

        bufpiped[i] = strsep(&buf, "|");

        if (bufpiped[i] == NULL)

            break;

    }

    if (bufpiped[1] == NULL)

        return 0; 

    else {

        return 1;

    }

}


void parsing_blank(char* buf, char** parsed_comm)
// function for parsing commands
{

    int a;

    //Assuming 50 is the maximum number of commands to be processed.

    for (a = 0; a < 50; a++) 
    {

        parsed_comm[a] = strsep(&buf, " ");

        if (parsed_comm[a] == NULL)

            break;

        if (strlen(parsed_comm[a]) == 0)

            a--;

    }

}

int process_string(char* buf, char** parsed_comm, char** ppipe)
// process_string returns 0 if there is no command or it is a Linux builtin command,
// 1 = Command Without Pipes, 2 = Command With Pipe
{

    char* strpipe[2];

    int piped_comm = 0;

    piped_comm = find_pipe(buf, strpipe);
    //searching for piped commands

    if (piped_comm) {
        //parsing commands in the event piped commands exist
        
        parsing_blank(strpipe[0], parsed_comm);

        parsing_blank(strpipe[1], ppipe);

    } else {

        parsing_blank(buf, parsed_comm);

    }

    if (Command_hndlr(parsed_comm))

        return 0;

    else

        return 1 + piped_comm;

}

void clrscr()
//clears screen when executed
{
    printf("\033[H\033[J");
}


void welcome()

{

    clrscr();

    printf("\n********************************************");

    printf("\n***WELCOME TO A SIMPLE COMMAND LINE SHELL***");

    printf("\n********************************************");

    char* username = getenv("USER");

    printf("\n\n\nThe User is: @%s", username);

    printf("\n");

    sleep(1);

}

int main()

{

    char input_str[600];
    
    char *parsedComm[50];

    char* parsedCommPiped[50];

    int exec_bit = 0; //Execution bit, used to determine what kind of command is input
                      //i.e. A piped command or a simple command

    int run_cond=1;

    welcome();

    while (run_cond) {

        // print shell prompt

        promptd();

        // taking input from the user

        if (recieve_input(input_str))

            continue;

        // process

        exec_bit = process_string(input_str,parsedComm, parsedCommPiped);

        
        // execution

        if (exec_bit == 1)
            //executing operations for a simple command
            comm_exe(parsedComm);

        if (exec_bit == 2)
            //executing operations for a piped command
            piped_comm_exe(parsedComm, parsedCommPiped);

    }

    return 0;

}

/***
 *References:

 *https://www.man7.org/linux/man-pages/man2/pipe.2.html
 *https://www.man7.org/linux/man-pages/man2/fork.2.html
 *https://brennan.io/2015/01/16/write-a-shell-in-c/
 *https://www.geeksforgeeks.org/making-linux-shell-c/

*/
