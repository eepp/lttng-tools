/*
 * Copyright (C) 2019 Jérémie Galarneau <jeremie.galarneau@efficios.com>
 * Copyright (C) 2019 Mathieu Desnoyers <mathieu.desnoyers@efficios.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 */

#ifndef LTTNG_CLEAR_H
#define LTTNG_CLEAR_H

#include <lttng/lttng-error.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
@addtogroup api_session_clear
@{
*/

/*!
@struct lttng_clear_handle

@brief
    Recording session clearing handle (opaque type).
*/
struct lttng_clear_handle;

/*!
@brief
    Clears the recording session named \lt_p{session_name}.

Depending on the mode of the recording session \lt_var{RS}
named \lt_p{session_name}, LTTng attempts to:

<dl>
  <dt>Local
  <dt>Network streaming
  <dd>
    Delete the contents of the recording buffers of \lt_var{RS}.

    Delete the local/remote data stream files of \lt_var{RS}.

  <dt>Snapshot
  <dd>
    Delete the contents of the recording buffers of \lt_var{RS}.

    LTTng doesn't touch prior snapshots of \lt_var{RS}.

  <dt>Live
  <dd>
    Delete the contents of the recording buffers of \lt_var{RS}.

    Delete the remote data stream files of \lt_var{RS}.

    Any active live reader currently reading a data stream packet
    may continue to read said packet completely.
</dl>

This function guarantees that any trace data which LTTng produced
\em before you call it won't be part of any trace, except:

- Preexisting recording session snapshots.
- Preexisting recording session archives.

It's possible that trace data which LTTng produces while you're calling
this function makes it to a trace.

Use \lt_p{*handle} to wait for the completion of the recording session
clearing operation.

@param[in] session_name
    Name of the recording session to clear.
@param[out] handle
    @parblock
    <strong>On success</strong>, this function sets \lt_p{*handle} to
    a handle which identifies this recording session clearing operation.

    May be \c NULL.

    Wait for the completion of this clearing operation with
    lttng_clear_handle_wait_for_completion().

    Destroy \lt_p{*handle} with lttng_clear_handle_destroy().
    @endparblock

@returns
    @parblock
    #LTTNG_OK on success, or a \em negative enumerator otherwise.

    Notable return values:

    <table>
      <tr>
        <td>-#LTTNG_ERR_CLEAR_RELAY_DISALLOWED
        <td>
          The relay daemon doesn't allow recording session clearing
          operations.

      <tr>
        <td>-#LTTNG_ERR_CLEAR_NOT_AVAILABLE_RELAY
        <td>
          The relay daemon doesn't support the recording session
          clearing operation.

      <tr>
        <td>-#LTTNG_ERR_CLEAR_FAIL_CONSUMER
        <td>
          A consumer daemon failed to clear the recording session named
          \lt_p{session_name}.
    </table>

@lt_pre_conn
@lt_pre_not_null{session_name}
@lt_pre_sess_exists{session_name}
@pre
    No clearing operation is in progress for the recording session named
    \lt_p{session_name}.
*/
extern enum lttng_error_code lttng_clear_session(const char *session_name,
		struct lttng_clear_handle **handle);

/// @}

#ifdef __cplusplus
}
#endif

#endif /* LTTNG_CLEAR_H */
c
