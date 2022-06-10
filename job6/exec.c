#include "root.h"
#include "lex.h"
#include "parse.h"
#include "exec.h"

// echo abc >log
//
// redirect
//   basic
//   >
//   log
//
// Inside child process
//
// cat <input >ouput
// redirct
//   redirect
//     basic
//       cat
//     <
//     input
//  >
//  output
void tree_execute_redirect(tree_t *this)
{
    tree_t *body;
    tree_t *op;
    tree_t *file;
    body = tree_get_child(this,0);
    op = tree_get_child(this,1);
    file = tree_get_child(this,2);

    char *path = file->token;
    int fd1, fd2;
    if(token_is(op,"<"))
    {
        fd1 = open(path,O_RDONLY);
        fd2 = 0;
    }
    if(token_is(op,">"))
    {
		fd1 =creat(path,0666);
		fd2=1;
	}
	if(token_is(op,">>"))
    {
		fd1 = open(path,O_APPEND|O_WRONLY);
		fd2 =1 ;
	}
	assert(fd1 > 0);
	dup2(fd1,fd2);
	close(fd1);
	tree_execute(body);
}

#define MAX_ARGC 16
void tree_execute_basic(tree_t *this)
{
    int argc=0;
	char *argv[MAX_ARGC];
	
	int i;
	tree_t *child;
	vector_each(&this->child_vector,i,child)
		argv[argc++]=child->token;
	argv[argc]=NULL;
	execvp(argv[0],argv);
	exit(EXIT_FAILURE);
}

// echo abc | grep b
//
// pipe
//   basic
//     echo
//     abc
//   basic
//     grep
//     b
//
// cmdA | cmdB | cmdC
// pipe
//   pipe 
//     cmdA
//     cmdB
//   cmdC
void tree_execute_pipe(tree_t *this)
{
    int fd[2];
	pid_t pid;
	tree_t *left=tree_get_child(this,0);
	tree_t *right=tree_get_child(this,1);
	pipe(fd);
	pid=fork();
	if(pid==0)
    {
		close(1);
		dup(fd[1]);
		close(fd[1]);
		close(fd[0]);
		tree_execute(left);
		exit(EXIT_FAILURE);
	}
	close(0);
	dup(fd[0]);
	close(fd[0]);
	close(fd[1]);
	tree_execute(right);
}

// # line 
// "cd /bin"
//
// # tree
// TREE_BASIC
//   cd
//   /bin
int tree_execute_builtin(tree_t *this)
{
    	if(this->type!=TREE_BASIC)
	 	return 0;
	 
	int argc=this->child_vector.count;
	tree_t *child0=tree_get_child(this,0);
	char *arg0=child0->token;
	
	if(!strcmp(arg0,"exit"))
    {
		exit(0);
		return 1;
	}
	
	if(!strcmp(arg0,"pwd"))
    {
		char p[256];
		getcwd(p,256);
		puts(p);
		return 1;
	}
	
	if(!strcmp(arg0,"cd"))
    {
		if(argc==1)
			return 1;
		tree_t *child1=tree_get_child(this,1);
		char *arg1=child1->token;
		int err=chdir(arg1);
		return 1;
	}
    return 0;
}

// cc a-very-large-file.c &
// async
//   basic
//     cc
//     a-very-large-file
//
// cat file | grep string &
// async
//   pipe
//     basic
//       cat
//       file
//     basic
//       grep
//       string
//
// child count == 1
void tree_execute_async(tree_t *this)
{
    tree_t *body =tree_get_child(this,0);
	tree_execute(body);
}

// Inside child process
void tree_execute(tree_t *this)
{
    switch (this->type) {
        case TREE_ASYNC:
            tree_execute_async(this); 
            break;

        case TREE_PIPE:
            tree_execute_pipe(this); 
            break;

        case TREE_REDIRICT:
            tree_execute_redirect(this); 
            break;

        case TREE_BASIC:
              tree_execute_basic(this); 
            break;
    }
}

// Inside parent(shell) process
void tree_execute_wrapper(tree_t *this)
{
    if (tree_execute_builtin(this))
        return;

    int status;
    pid_t pid = fork();
    if (pid == 0) {
        tree_execute(this);
        exit(EXIT_FAILURE);
    }
   
    // cc a-large-file.c &
    if (this->type != TREE_ASYNC)
        wait(&status);
}

// cmd1 <input | cmd2 | cmd3 >output &
