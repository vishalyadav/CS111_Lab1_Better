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
                        return rcsHelp (curr->previous, s, PIPE_COMMAND);
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
	while (curr != 0)








	return 0;
} 
