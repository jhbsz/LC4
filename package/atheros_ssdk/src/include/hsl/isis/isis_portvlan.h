/*
 * Copyright (c) 2007-2009 Atheros Communications, Inc.
 * All rights reserved.
 *
 */
#ifndef _ISIS_PORTVLAN_H_
#define _ISIS_PORTVLAN_H_

#ifdef __cplusplus
extern "C" {
#endif                          /* __cplusplus */

#include "fal/fal_portvlan.h"

sw_error_t isis_portvlan_init(a_uint32_t dev_id);

#ifdef IN_PORTVLAN
    #define ISIS_PORTVLAN_INIT(rv, dev_id) \
    { \
        rv = isis_portvlan_init(dev_id); \
        SW_RTN_ON_ERROR(rv); \
    }
#else
    #define ISIS_PORTVLAN_INIT(rv, dev_id)
#endif

#ifdef HSL_STANDALONG


HSL_LOCAL sw_error_t
isis_port_1qmode_set(a_uint32_t dev_id, fal_port_t port_id,
                       fal_pt_1qmode_t port_1qmode);


HSL_LOCAL sw_error_t
isis_port_1qmode_get(a_uint32_t dev_id, fal_port_t port_id,
                       fal_pt_1qmode_t * pport_1qmode);


HSL_LOCAL sw_error_t
isis_port_egvlanmode_set(a_uint32_t dev_id, fal_port_t port_id,
                           fal_pt_1q_egmode_t port_egvlanmode);


HSL_LOCAL sw_error_t
isis_port_egvlanmode_get(a_uint32_t dev_id, fal_port_t port_id,
                           fal_pt_1q_egmode_t * pport_egvlanmode);


HSL_LOCAL sw_error_t
isis_portvlan_member_add(a_uint32_t dev_id, fal_port_t port_id,
                           a_uint32_t mem_port_id);


HSL_LOCAL sw_error_t
isis_portvlan_member_del(a_uint32_t dev_id, fal_port_t port_id,
                           a_uint32_t mem_port_id);


HSL_LOCAL sw_error_t
isis_portvlan_member_update(a_uint32_t dev_id, fal_port_t port_id,
                              fal_pbmp_t mem_port_map);


HSL_LOCAL sw_error_t
isis_portvlan_member_get(a_uint32_t dev_id, fal_port_t port_id,
                           fal_pbmp_t * mem_port_map);


HSL_LOCAL sw_error_t
isis_port_force_default_vid_set(a_uint32_t dev_id, fal_port_t port_id,
                                  a_bool_t enable);


HSL_LOCAL sw_error_t
isis_port_force_default_vid_get(a_uint32_t dev_id, fal_port_t port_id, a_bool_t * enable);


HSL_LOCAL sw_error_t
isis_port_force_portvlan_set(a_uint32_t dev_id, fal_port_t port_id,
                               a_bool_t enable);


HSL_LOCAL sw_error_t
isis_port_force_portvlan_get(a_uint32_t dev_id, fal_port_t port_id, a_bool_t * enable);


HSL_LOCAL sw_error_t
isis_nestvlan_tpid_set(a_uint32_t dev_id, a_uint32_t tpid);


HSL_LOCAL sw_error_t
isis_nestvlan_tpid_get(a_uint32_t dev_id, a_uint32_t * tpid);


HSL_LOCAL sw_error_t
isis_port_invlan_mode_set(a_uint32_t dev_id, fal_port_t port_id,
                           fal_pt_invlan_mode_t mode);


HSL_LOCAL sw_error_t
isis_port_invlan_mode_get(a_uint32_t dev_id, fal_port_t port_id,
                           fal_pt_invlan_mode_t * mode);


HSL_LOCAL sw_error_t
isis_port_tls_set(a_uint32_t dev_id, fal_port_t port_id,
                           a_bool_t enable);


HSL_LOCAL sw_error_t
isis_port_tls_get(a_uint32_t dev_id, fal_port_t port_id,
                           a_bool_t * enable);


HSL_LOCAL sw_error_t
isis_port_pri_propagation_set(a_uint32_t dev_id, fal_port_t port_id,
                           a_bool_t enable);


HSL_LOCAL sw_error_t
isis_port_pri_propagation_get(a_uint32_t dev_id, fal_port_t port_id,
                           a_bool_t * enable);


HSL_LOCAL sw_error_t
isis_port_default_svid_set(a_uint32_t dev_id, fal_port_t port_id,
                           a_uint32_t vid);


HSL_LOCAL sw_error_t
isis_port_default_svid_get(a_uint32_t dev_id, fal_port_t port_id,
                           a_uint32_t * vid);

HSL_LOCAL sw_error_t
isis_port_default_cvid_set(a_uint32_t dev_id, fal_port_t port_id,
                           a_uint32_t vid);


HSL_LOCAL sw_error_t
isis_port_default_cvid_get(a_uint32_t dev_id, fal_port_t port_id,
                           a_uint32_t * vid);


HSL_LOCAL sw_error_t
isis_port_vlan_propagation_set(a_uint32_t dev_id, fal_port_t port_id,
                           fal_vlan_propagation_mode_t mode);


HSL_LOCAL sw_error_t
isis_port_vlan_propagation_get(a_uint32_t dev_id, fal_port_t port_id,
                           fal_vlan_propagation_mode_t * mode);


HSL_LOCAL sw_error_t
isis_port_vlan_trans_add(a_uint32_t dev_id, fal_port_t port_id, fal_vlan_trans_entry_t *entry);


HSL_LOCAL sw_error_t
isis_port_vlan_trans_del(a_uint32_t dev_id, fal_port_t port_id, fal_vlan_trans_entry_t *entry);


HSL_LOCAL sw_error_t
isis_port_vlan_trans_get(a_uint32_t dev_id, fal_port_t port_id, fal_vlan_trans_entry_t *entry);


HSL_LOCAL sw_error_t
isis_qinq_mode_set(a_uint32_t dev_id, fal_qinq_mode_t mode);


HSL_LOCAL sw_error_t
isis_qinq_mode_get(a_uint32_t dev_id, fal_qinq_mode_t * mode);


HSL_LOCAL sw_error_t
isis_port_qinq_role_set(a_uint32_t dev_id, fal_port_t port_id, fal_qinq_port_role_t role);


HSL_LOCAL sw_error_t
isis_port_qinq_role_get(a_uint32_t dev_id, fal_port_t port_id, fal_qinq_port_role_t * role);


HSL_LOCAL sw_error_t
isis_port_vlan_trans_iterate(a_uint32_t dev_id, fal_port_t port_id,
                        a_uint32_t * iterator, fal_vlan_trans_entry_t * entry);


#endif

#ifdef __cplusplus
}
#endif                          /* __cplusplus */
#endif                          /* _ISIS_PORTVLAN_H */

