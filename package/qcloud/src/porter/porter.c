/*
 * porter.c
 *
 *  Created on: Oct 29, 2012
 *      Author: ch
 */
#include <signal.h>

#include "std_hdr.h"
#include "comm_hdr.h"

#include "qid_system.h"
#include "porter_ev.h"

static void porter_cleanup(int sig_num)
{
  /*
   * stop porter's event loop
   */
  porter_ev_cleanup();

  // cleanup log
  qnap_log_cleanup();
}

/*
 * porter_signal_init()
 *
 * Setup signal handler.
 *
 * Input:
 *      None.
 *
 * Output:
 *      @ NO_ERROR   for successfully set up.
 *      @ ERR_SYSTEM for failed set up.
 */
static int porter_signal_init (void)
{
  int   rc = NO_ERROR;            // return code

  // this signal is ignored
  rc |= util_setup_signal(SIGPIPE, SIG_IGN);

  // do cleanup
  rc |= util_setup_signal(SIGQUIT, porter_cleanup);
  rc |= util_setup_signal(SIGTERM, porter_cleanup);
  rc |= util_setup_signal(SIGABRT, porter_cleanup);

  return(rc);
}

/*
 * porter_init()
 *
 * Init. each component.
 *
 * Input:
 *      @ daemon_name -- name of this program.
 *
 * Output:
 *      @ NO_ERROR for successful initialization.
 *      @ ERR_xxxx for any kind of error.
 */
static int porter_init (char *daemon_name)
{
  int   rc = NO_ERROR;            // return code

  /*
   * Set up signal handler.
   */
  rc |= porter_signal_init();

  /*
   * Setup event handling
   */
  rc |= porter_ev_init();

  return rc;
}

/*
 * main()
 *
 * Main entry routine.
 */
int main (int argc, char **argv)
{

  char  *daemon_name;                   // name of program
  int   rc;                             // return code

  /*
   * Retrieve program name without directory.
   */
  daemon_name = util_get_daemon_name(argv[0]);

  /*
   * Switch to daemon mode
   */
  util_daemonize();

  /*
   * Do initialization.
   */
  if ((rc = porter_init(daemon_name)) != NO_ERROR)
    goto out;


  /*
   * Start running server daemon.
   */
  porter_ev_run();

out:
  /*
   * Release resource
   */
  porter_cleanup(0);
  return(0);

}
