#include <setjmp.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <setjmp.h>

// TODO: jūsu kods te!
jmp_buf buf;

void termination_handler ()
{
  longjmp(buf, 666);
}

int main(void)
{
    // TODO: jūsu kods te!
	struct sigaction new_action;
	sigemptyset (&new_action.sa_mask);

	new_action.sa_flags = 0;
	new_action.sa_handler = termination_handler;	
	sigaction (SIGALRM, &new_action, NULL);
	int i = setjmp(buf);
	if(i == 666){
		return 0;
	}
    // taalaako kodu nemainiit!

    // taimeris uz 3 sekundeem
    alarm(3);

    // bezgaliigs cikls
    while (1);

    // atgriezh kaut kaadu nenulles kodu, lai kompilators nesuudzeetos
    return -1;
}