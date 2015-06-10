/*
 * porter_ev.h
 *
 *  Created on: Oct 29, 2012
 *      Author: ch
 */

#ifndef PORTER_EV_H_
#define PORTER_EV_H_

/*
 * Structure for event control block.
 */
typedef struct _porter_ev_cb_st {
  // a timestamp, for checking device status
  time_t            check_time;

  // a timestamp, for updating device status to mycloudnas server.
  time_t            update_time;

  // last updated lan ip & external ip
  lan_ip_info_st    lan_info;
  char              external_ip[IP_STR_LEN+1];

  // newly get lan ip & external ip
  lan_ip_info_st    new_lan_info;
  char              new_external_ip[IP_STR_LEN+1];

  // device status {0: checked out, 1: Validating, 2: Validated, 3: CheckedIn}
  char              device_status[2];

  // flag to check if exit
  int               exit_flag;
} PORTER_EV_CB_T;

/*
 * Forward functions declaration.
 */
extern int  porter_ev_init(void);
extern void porter_ev_cleanup(void);
extern int porter_ev_run(void);

#endif /* PORTER_EV_H_ */
