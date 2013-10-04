// UCLA CS 111 Lab 1 command reading

#include "command.h"
#include "command-internals.h"
#include "alloc.h"

#include <error.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/* FIXME: You may need to add #include directives, macro definitions,
   static function definitions, etc.  */

/* FIXME: Define the type 'struct command_stream' here.  This should
   complete the incomplete type declaration in command.h.  */

struct command_stream {
  char x;
  struct command_stream *next;
  struct command_stream *previous;
};

command_stream_t
make_command_stream (int (*get_next_byte) (void *),
		     void *get_next_byte_argument)
{
  /* FIXME: Replace this with your implementation.  You may need to
     add auxiliary functions and otherwise modify the source code.
     You can also use external functions defined in the GNU C Library.  */
    struct command_stream *tail;
    tail = checked_malloc(sizeof(struct command_stream));
    tail->next = 0;
    tail->previous = 0;
    tail->x = '\0';
    int newline_spotted = 0;
    char curr_byte;
    struct command_stream *nextChar;

    curr_byte = get_next_byte(get_next_byte_argument);
    if(curr_byte == EOF) {
        return tail;
    } else {
        tail -> x = curr_byte;
    }

    while((curr_byte = get_next_byte(get_next_byte_argument)) != EOF) {
        if(curr_byte == '\n') {
            newline_spotted = 1;
            continue;
        }
        if(newline_spotted == 1) {
            newline_spotted = 0;
            tail -> next = checked_malloc(sizeof(struct command_stream));
            nextChar = tail -> next;
            nextChar -> previous = tail;
            nextChar -> next = 0;
            nextChar -> x = '\n';
            tail = nextChar;
            nextChar = 0;
        } else {
            if(curr_byte == '#') {
                while((curr_byte = get_next_byte(get_next_byte_argument)) != EOF) {
                    if(curr_byte == '\n') {
                        break;
                    }
                }
                continue;
            }
        }
        tail -> next = checked_malloc(sizeof(struct command_stream));
        nextChar = tail -> next;
        nextChar -> previous = tail;
        nextChar -> next = 0;
        nextChar -> x = curr_byte;
        tail = nextChar;
        nextChar = 0;
        newline_spotted = 0;
    }

    return tail;
}


/*
command_t
read_command_stream (command_stream_t s)
{
	struct command_stream curr = s;
	int inParen = 0; 
        while (curr != 0)
        {
                if (curr->x == ')')
		{
			inParen++;
			continue;
		}
		else if (inParen>0 && curr->x != '(')
		{
			continue;
		}
		else if (inParen > 0 && curr->x == '(')
		{
			inParen--;
			continue;
		}
		else if (curr->x == '&' && curr->previous->x == '&')
		{
			curr->next->previous = 0;
			return rcsHelp (curr->previous, s, AND_COMMAND);
		}
		else if (curr ->x == '|' && curr->previous->x = '|')
		{
                        curr->next->previous = 0;
                        return rcsHelp (curr->previous, s, OR_COMMAND);

		}
		else if (curr->x == ';' || curr->x == '\n')
		{
                        curr->next->previous = 0;
                        return rcsHelp (curr->previous, s, SEQUENCE_COMMAND);
		}
		else
		{
			curr = curr->previous;
		}
        }

  //error (1, 0, "command reading not yet implemented");
  return 0;
}

*/

command_t
read_command_stream (command_stream_t s)
{
        struct command_stream curr = s;
        int inParen = 0;
        while (curr != 0)
        {
                if (curr->x == ')')
                {
                        inParen++;
                        continue;
                }
                else if (inParen>0 && curr->x != '(')
                {
                        continue;
                }
                else if (inParen > 0 && curr->x == '(')
                {
                        inParen--;
                        continue;
                }
                else if (curr->x == '&' && curr->previous->x == '&')
                {
                        curr->next->previous = 0;
			struct command *andCom;
			andCom = checked_malloc(sizeof(struct command));
			andCom->type = AND_COMMAND;
			andCom->u.command[0] = read_command_stream(curr->previous);
			andCom->u.command[1] = read_command_stream(s);
			andCom->status = -1;
			andCom->input = 0;
			andCom->output = 0;
                        return *andCom;
                }
                else if (curr ->x == '|' && curr->previous->x = '|')
                {
                        curr->next->previous = 0;
                        struct command *orCom;
                       	orCom = checked_malloc(sizeof(struct command));
                        orCom->type = OR_COMMAND;
                        orCom->command[0] = read_command_stream(curr->previous);
                        orCom->command[1] = read_command_stream(s);
                        orCom->status = -1;
                        orCom->input = 0;
                        orCom->output = 0;
                        return *orCom;


                }
                else if (curr->x == ';' || curr->x == '\n')
                {
                        curr->next->previous = 0;
                        struct command *seqCom;
                        seqCom = checked_malloc(sizeof(struct command));
                        seqCom->type = SEQUENCE_COMMAND;
                        seqCom->command[0] = read_command_stream(curr->previous);
                        seqCom->command[1] = read_command_stream(s);
                        seqCom->status = -1;
                        seqCom->input = 0;
                        seqCom->output = 0;
                        return *seqCom;
                }
                else
                {
                        curr = curr->previous;
                }
        }
	curr = s;
	while (curr != 0)
	{
                if (curr->x == ')')
                {
                        inParen++;
			continue;
                }
                else if (inParen>0 && curr->x != '(')
                {
                        continue;
                }
                else if (inParen > 0 && curr->x == '(')
                {
                        inParen--;
                        continue;
                }
		else if (curr->x == '|')
		{
                        curr->next->previous = 0;
                        struct command *pipeCom;
                        pipeCom = checked_malloc(sizeof(struct command));
                        pipeCom->type = PIPE_COMMAND;
                        pipeCom->command[0] = read_command_stream(curr->previous);
                        pipeCom->command[1] = read_command_stream(s);
                        pipeCom->status = -1;
                        pipeCom->input = 0;
                        pipeCom->output = 0;
                        return *pipeCom;

		}
		else
		{
			curr = curr->previous;
		}
	}
	curr = s;	//maybe move this parentheses part to the end, after redirect and word
	while (curr != 0)
	{
                if (curr->x == ')')
                {
                        curr = curr->previous;
			struct command_stream parenTail = curr;
                        while (curr->previous != '(')
                        {
                                curr = curr->previous;
                                continue;
                        }
			curr->previous = 0;
			return read_command_stream (parenTail);
                }
		else
		{
			curr = curr->previous
		}
	}
	curr = s;
	int simpLength = 1;
	while (curr != 0)
	{
		if (curr->x == '<')
		{
			struct command_stream leftCur = curr->previous;
			struct command_stream rightCur = s;
			struct command_stream leftTail = leftCur;
			struct command_stream rightTail = rightCur;
			int charNumLeft = 1;
			int charNumRight = 1;
			while (leftCur->previous != 0)
			{
				charNumLeft++;
				leftCur = leftCur->previous;
			}
			char *leftString = checked_malloc(charNumLeft + 2);
			int indexL = 0;
			while(leftCur != leftTail)
			{
				leftString[indexL] = leftCur->x;
				leftCur = leftCur->next;
				indexL++;
			}
			leftString[indexL] = leftCur->x;
                        while (rightCur->previous != 0)
                        {
                                charNumRight++;
                                rightCur = rightCur->previous;
                        }
                        char *rightString = checked_malloc(charNumRight + 2);
                        int indexR = 0;
                        while(rightCur != rightTail)
                        {                               
                                rightString[indexR] = rightCur->x;
                               	rightCur = rightCur->next;
                                indexR++;
                        }
                        rightString[indexR] = rightCur->x;
                        struct command *redCom;
                        redCom = checked_malloc(sizeof(struct command));
                        redCom->type = SIMPLE_COMMAND;
                        redCom->input = rightString;
			redCom->output = leftString;
                        redCom->status = 0;
                        return *redCom;
		}
                else if (curr->x == '>')
                {
                        struct command_stream leftCur = curr->previous;
                        struct command_stream rightCur = s;
                        struct command_stream leftTail = leftCur;
                        struct command_stream rightTail = rightCur;
                        int charNumLeft = 1;
                        int charNumRight = 1;
                        while (leftCur->previous != 0)
                        {
                                charNumLeft++;
                                leftCur = leftCur->previous;
                        }
                        char *leftString = checked_malloc(charNumLeft + 2);
                        int indexL = 0;
                        while(leftCur != leftTail)
                        {
                                leftString[indexL] = leftCur->x;
                                leftCur = leftCur->next;
                                indexL++;
                        }
                        leftString[indexL] = leftCur->x;
                        while (rightCur->previous != 0)
                        {
                                charNumRight++;
                                rightCur = rightCur->previous;
                        }
                        char *rightString = checked_malloc(charNumRight + 2);
                        int indexR = 0;
                        while(rightCur != rightTail)
                        {
                                rightString[indexR] = rightCur->x;
                                rightCur = rightCur->next;
                                indexR++;
                        }
                        rightString[indexR] = rightCur->x;
                        struct command *redCom;
                        redCom = checked_malloc(sizeof(struct command));
                        redCom->type = SIMPLE_COMMAND;
                        redCom->input = leftString;
                        redCom->output = rightString;
                        redCom->status = 0;
                        return *redCom;
                }
		else
		{
			curr = curr->previous;
			simpLength++;
		}
	}

		char *simpString = checked_malloc(simpLength + 2);
                int indexS = 0;
                while(curr != s)
                {
                	simpString[indexS] = curr->x;
                        curr = curr->next;
                        indexS++;
                }
                simpString[indexS] = curr->x;
		char **wordS = simpString;
                struct command *simpCom;
                simpCom = checked_malloc(sizeof(struct command));
                simpCom->type = SIMPLE_COMMAND;
                simpCom->word = wordS;
                simpCom->status = 0;
                simpCom->input = 0;
                simpCom->output = 0;
                return *simpCom;
} 
