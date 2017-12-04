# Cshells by the Cshore

## Features
* Forks and executes commands!
* Parses multiple commands on one line!
* Redirects using >, <!
* Pipes using |!
* Will twist your tongue!

## Attempted Features
* Not requiring all items on the command line to be separated by a space
* Being able to parse more than 2 commands on one line
* Piping more than once

## Bugs
* Can't parse more than 2 commands in one line
* Redirecting `stdout` and `stdin` at the same time doesn't seem to work

## Functions

* `parse_args`
	* Arguments:
	    * `char* argstring`
	* Return Value:
		* `char**` that contains the elements of argstring split by a space.
	* What it Does:
	    * It will return a `char**`. All the elements in this `char**` are just the elements of argstring, split by a space.
* `execfriend`
	* Arguments:
	    * `char** cmd_line`
	* Return Value:
		* (none)
	* What it Does:
	    * It helps out with executing commands that are split by semicolons.
	    * It makes 2 arrays out of an arbitrary list of commands split by ;.
	    * 1 of those arrays is the 1st command and the 2nd is an array of
	    the rest of the commands, which may or may not have semicolons.
	    * It executes whatever is in the first array and calls itself
	    again on the 2nd.
	* Note:
	   * oooo recursion

* `cshell`
	* Arguments:
	    * (none)
	* Return Value:
		* 1 if exited, 0 otherwise
	* What it Does:
	    * It's essentially the loop of the shell. It gets the input from
	    the user and processes it. It's also where the cd lives.

* `main`
	* Arguments:
	    * (none)
	* Return Value:
		* 0
	* What it Does:
	   * It's the main.
