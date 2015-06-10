/*
 * EZP Upgraded Reboot
 */

#include <signal.h>

int main(){
	kill(1, SIGKILL);
	return 0;
}
