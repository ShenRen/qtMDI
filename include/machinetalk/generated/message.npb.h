/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.0 at Sun Feb  1 12:40:11 2015. */

#ifndef PB_MESSAGE_NPB_H_INCLUDED
#define PB_MESSAGE_NPB_H_INCLUDED
#include <pb.h>

#include <machinetalk/generated/types.npb.h>
#include <machinetalk/generated/object.npb.h>
#include <machinetalk/generated/log.npb.h>
#include <machinetalk/generated/motcmds.npb.h>
#include <machinetalk/generated/task.npb.h>
#include <machinetalk/generated/canon.npb.h>
#include <machinetalk/generated/value.npb.h>
#include <machinetalk/generated/rtapicommand.npb.h>
#include <machinetalk/generated/rtapi_message.npb.h>
#include <machinetalk/generated/config.npb.h>
#include <machinetalk/generated/preview.npb.h>
#include <machinetalk/generated/status.npb.h>
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Enum definitions */
/* Struct definitions */
typedef struct _pb_Container {
    pb_ContainerType type;
    bool has_tsc;
    int64_t tsc;
    bool has_motcmd;
    pb_MotionCommand motcmd;
    bool has_motstat;
    pb_MotionStatus motstat;
    bool has_rtapi_message;
    pb_RTAPI_Message rtapi_message;
    bool has_ticket_update;
    pb_TicketUpdate ticket_update;
    bool has_tv_sec;
    int32_t tv_sec;
    bool has_tv_nsec;
    int32_t tv_nsec;
    pb_callback_t topic;
    bool has_reply_required;
    bool reply_required;
    pb_callback_t interp_name;
    bool has_rsvp;
    int32_t rsvp;
    bool has_in_reply_to;
    pb_ContainerType in_reply_to;
    bool has_rcs_status;
    pb_RCS_STATUS rcs_status;
    bool has_status;
    pb_StatusType status;
    bool has_serial;
    int32_t serial;
    bool has_reply_serial;
    int32_t reply_serial;
    bool has_ticket;
    int32_t ticket;
    bool has_reply_ticket;
    int32_t reply_ticket;
    bool has_sequence;
    int32_t sequence;
    bool has_credit;
    int32_t credit;
    bool has_line_number;
    int32_t line_number;
    pb_callback_t note;
    bool has_retcode;
    int32_t retcode;
    bool has_trace;
    bool trace;
    bool has_instance;
    int32_t instance;
    bool has_traj_set_g5x;
    pb_Emc_Traj_Set_G5x traj_set_g5x;
    bool has_traj_set_g92;
    pb_Emc_Traj_Set_G92 traj_set_g92;
    bool has_traj_set_rotation;
    pb_Emc_Traj_Set_Rotation traj_set_rotation;
    bool has_traj_linear_move;
    pb_Emc_Traj_Linear_Move traj_linear_move;
    bool has_traj_probe;
    pb_Emc_Traj_Probe traj_probe;
    bool has_traj_circular_move;
    pb_Emc_Traj_Circular_Move traj_circular_move;
    bool has_traj_rigid_tap;
    pb_Emc_Traj_Rigid_Tap traj_rigid_tap;
    bool has_traj_set_term_cond;
    pb_Emc_Traj_Set_Term_Cond traj_set_term_cond;
    bool has_traj_set_spindlesync;
    pb_Emc_Traj_Set_Spindlesync traj_set_spindlesync;
    bool has_traj_delay;
    pb_Emc_Traj_Delay traj_delay;
    bool has_spindle_on;
    pb_Emc_Spindle_On spindle_on;
    bool has_spindle_speed;
    pb_Emc_Spindle_Speed spindle_speed;
    bool has_spindle_orient;
    pb_Emc_Spindle_Orient spindle_orient;
    bool has_spindle_wait_orient_complete;
    pb_Emc_Spindle_Wait_Orient_Complete spindle_wait_orient_complete;
    bool has_tool_set_offet;
    pb_Emc_Tool_Set_Offset tool_set_offet;
    bool has_traj_set_offset;
    pb_Emc_Traj_Set_Offset traj_set_offset;
    bool has_tool_prepare;
    pb_Emc_Tool_Prepare tool_prepare;
    bool has_tool_set_number;
    pb_Emc_Tool_Set_Number tool_set_number;
    bool has_traj_set_fo_enable;
    pb_Emc_Traj_Set_Fo_Enable traj_set_fo_enable;
    bool has_traj_set_so_enable;
    pb_Emc_Traj_Set_So_Enable traj_set_so_enable;
    bool has_traj_set_fh_enable;
    pb_Emc_Traj_Set_Fh_Enable traj_set_fh_enable;
    bool has_motion_adaptive;
    pb_Emc_Motion_Adaptive motion_adaptive;
    bool has_operator_display;
    pb_Emc_Operator_Display operator_display;
    bool has_operator_text;
    pb_Emc_Operator_Text operator_text;
    bool has_operator_error;
    pb_Emc_Operator_Error operator_error;
    bool has_motion_set_dout;
    pb_Emc_Motion_Set_Dout motion_set_dout;
    bool has_motion_set_aout;
    pb_Emc_Motion_Set_Aout motion_set_aout;
    bool has_aux_input_wait;
    pb_Emc_Aux_Input_Wait aux_input_wait;
    bool has_exec_plugin_call;
    pb_Emc_Exec_Plugin_Ca1l exec_plugin_call;
    bool has_io_plugin_call;
    pb_Emc_Io_Plugin_Call io_plugin_call;
    bool has_emc_status_config;
    pb_EmcStatusConfig emc_status_config;
    bool has_emc_status_motion;
    pb_EmcStatusMotion emc_status_motion;
    bool has_emc_status_io;
    pb_EmcStatusIo emc_status_io;
    bool has_emc_status_task;
    pb_EmcStatusTask emc_status_task;
    bool has_emc_status_interp;
    pb_EmcStatusInterp emc_status_interp;
    bool has_emc_command_params;
    pb_EmcCommandParameters emc_command_params;
} pb_Container;

/* Default values for struct fields */

/* Initializer values for message structs */
#define pb_Container_init_default                {(pb_ContainerType)0, false, 0, false, pb_MotionCommand_init_default, false, pb_MotionStatus_init_default, false, pb_RTAPI_Message_init_default, false, pb_TicketUpdate_init_default, false, 0, false, 0, {{NULL}, NULL}, false, 0, {{NULL}, NULL}, false, 0, false, (pb_ContainerType)0, false, (pb_RCS_STATUS)0, false, (pb_StatusType)0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, {{NULL}, NULL}, false, 0, false, 0, false, 0, false, pb_Emc_Traj_Set_G5x_init_default, false, pb_Emc_Traj_Set_G92_init_default, false, pb_Emc_Traj_Set_Rotation_init_default, false, pb_Emc_Traj_Linear_Move_init_default, false, pb_Emc_Traj_Probe_init_default, false, pb_Emc_Traj_Circular_Move_init_default, false, pb_Emc_Traj_Rigid_Tap_init_default, false, pb_Emc_Traj_Set_Term_Cond_init_default, false, pb_Emc_Traj_Set_Spindlesync_init_default, false, pb_Emc_Traj_Delay_init_default, false, pb_Emc_Spindle_On_init_default, false, pb_Emc_Spindle_Speed_init_default, false, pb_Emc_Spindle_Orient_init_default, false, pb_Emc_Spindle_Wait_Orient_Complete_init_default, false, pb_Emc_Tool_Set_Offset_init_default, false, pb_Emc_Traj_Set_Offset_init_default, false, pb_Emc_Tool_Prepare_init_default, false, pb_Emc_Tool_Set_Number_init_default, false, pb_Emc_Traj_Set_Fo_Enable_init_default, false, pb_Emc_Traj_Set_So_Enable_init_default, false, pb_Emc_Traj_Set_Fh_Enable_init_default, false, pb_Emc_Motion_Adaptive_init_default, false, pb_Emc_Operator_Display_init_default, false, pb_Emc_Operator_Text_init_default, false, pb_Emc_Operator_Error_init_default, false, pb_Emc_Motion_Set_Dout_init_default, false, pb_Emc_Motion_Set_Aout_init_default, false, pb_Emc_Aux_Input_Wait_init_default, false, pb_Emc_Exec_Plugin_Ca1l_init_default, false, pb_Emc_Io_Plugin_Call_init_default, false, pb_EmcStatusConfig_init_default, false, pb_EmcStatusMotion_init_default, false, pb_EmcStatusIo_init_default, false, pb_EmcStatusTask_init_default, false, pb_EmcStatusInterp_init_default, false, pb_EmcCommandParameters_init_default}
#define pb_Container_init_zero                   {(pb_ContainerType)0, false, 0, false, pb_MotionCommand_init_zero, false, pb_MotionStatus_init_zero, false, pb_RTAPI_Message_init_zero, false, pb_TicketUpdate_init_zero, false, 0, false, 0, {{NULL}, NULL}, false, 0, {{NULL}, NULL}, false, 0, false, (pb_ContainerType)0, false, (pb_RCS_STATUS)0, false, (pb_StatusType)0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, false, 0, {{NULL}, NULL}, false, 0, false, 0, false, 0, false, pb_Emc_Traj_Set_G5x_init_zero, false, pb_Emc_Traj_Set_G92_init_zero, false, pb_Emc_Traj_Set_Rotation_init_zero, false, pb_Emc_Traj_Linear_Move_init_zero, false, pb_Emc_Traj_Probe_init_zero, false, pb_Emc_Traj_Circular_Move_init_zero, false, pb_Emc_Traj_Rigid_Tap_init_zero, false, pb_Emc_Traj_Set_Term_Cond_init_zero, false, pb_Emc_Traj_Set_Spindlesync_init_zero, false, pb_Emc_Traj_Delay_init_zero, false, pb_Emc_Spindle_On_init_zero, false, pb_Emc_Spindle_Speed_init_zero, false, pb_Emc_Spindle_Orient_init_zero, false, pb_Emc_Spindle_Wait_Orient_Complete_init_zero, false, pb_Emc_Tool_Set_Offset_init_zero, false, pb_Emc_Traj_Set_Offset_init_zero, false, pb_Emc_Tool_Prepare_init_zero, false, pb_Emc_Tool_Set_Number_init_zero, false, pb_Emc_Traj_Set_Fo_Enable_init_zero, false, pb_Emc_Traj_Set_So_Enable_init_zero, false, pb_Emc_Traj_Set_Fh_Enable_init_zero, false, pb_Emc_Motion_Adaptive_init_zero, false, pb_Emc_Operator_Display_init_zero, false, pb_Emc_Operator_Text_init_zero, false, pb_Emc_Operator_Error_init_zero, false, pb_Emc_Motion_Set_Dout_init_zero, false, pb_Emc_Motion_Set_Aout_init_zero, false, pb_Emc_Aux_Input_Wait_init_zero, false, pb_Emc_Exec_Plugin_Ca1l_init_zero, false, pb_Emc_Io_Plugin_Call_init_zero, false, pb_EmcStatusConfig_init_zero, false, pb_EmcStatusMotion_init_zero, false, pb_EmcStatusIo_init_zero, false, pb_EmcStatusTask_init_zero, false, pb_EmcStatusInterp_init_zero, false, pb_EmcCommandParameters_init_zero}

/* Field tags (for use in manual encoding/decoding) */
#define pb_Container_type_tag                    1
#define pb_Container_tsc_tag                     7
#define pb_Container_motcmd_tag                  9
#define pb_Container_motstat_tag                 10
#define pb_Container_rtapi_message_tag           13
#define pb_Container_ticket_update_tag           15
#define pb_Container_tv_sec_tag                  21
#define pb_Container_tv_nsec_tag                 22
#define pb_Container_topic_tag                   23
#define pb_Container_reply_required_tag          24
#define pb_Container_interp_name_tag             26
#define pb_Container_rsvp_tag                    30
#define pb_Container_in_reply_to_tag             33
#define pb_Container_rcs_status_tag              35
#define pb_Container_status_tag                  40
#define pb_Container_serial_tag                  45
#define pb_Container_reply_serial_tag            50
#define pb_Container_ticket_tag                  55
#define pb_Container_reply_ticket_tag            60
#define pb_Container_sequence_tag                62
#define pb_Container_credit_tag                  65
#define pb_Container_line_number_tag             66
#define pb_Container_note_tag                    68
#define pb_Container_retcode_tag                 69
#define pb_Container_trace_tag                   75
#define pb_Container_instance_tag                77
#define pb_Container_traj_set_g5x_tag            300
#define pb_Container_traj_set_g92_tag            310
#define pb_Container_traj_set_rotation_tag       320
#define pb_Container_traj_linear_move_tag        330
#define pb_Container_traj_probe_tag              340
#define pb_Container_traj_circular_move_tag      350
#define pb_Container_traj_rigid_tap_tag          360
#define pb_Container_traj_set_term_cond_tag      370
#define pb_Container_traj_set_spindlesync_tag    380
#define pb_Container_traj_delay_tag              390
#define pb_Container_spindle_on_tag              400
#define pb_Container_spindle_speed_tag           410
#define pb_Container_spindle_orient_tag          420
#define pb_Container_spindle_wait_orient_complete_tag 430
#define pb_Container_tool_set_offet_tag          440
#define pb_Container_traj_set_offset_tag         450
#define pb_Container_tool_prepare_tag            460
#define pb_Container_tool_set_number_tag         470
#define pb_Container_traj_set_fo_enable_tag      480
#define pb_Container_traj_set_so_enable_tag      490
#define pb_Container_traj_set_fh_enable_tag      500
#define pb_Container_motion_adaptive_tag         510
#define pb_Container_operator_display_tag        520
#define pb_Container_operator_text_tag           530
#define pb_Container_operator_error_tag          540
#define pb_Container_motion_set_dout_tag         550
#define pb_Container_motion_set_aout_tag         560
#define pb_Container_aux_input_wait_tag          570
#define pb_Container_exec_plugin_call_tag        580
#define pb_Container_io_plugin_call_tag          590
#define pb_Container_emc_status_config_tag       600
#define pb_Container_emc_status_motion_tag       601
#define pb_Container_emc_status_io_tag           602
#define pb_Container_emc_status_task_tag         603
#define pb_Container_emc_status_interp_tag       604
#define pb_Container_emc_command_params_tag      610

/* Struct field encoding specification for nanopb */
extern const pb_field_t pb_Container_fields[63];

/* Maximum encoded size of messages (where known) */

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
