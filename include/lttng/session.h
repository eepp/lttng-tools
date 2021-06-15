/*
 * Copyright (C) 2014 David Goulet <dgoulet@efficios.com>
 * Copyright (C) 2019 Jérémie Galarneau <jeremie.galarneau@efficios.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 */

#ifndef LTTNG_SESSION_H
#define LTTNG_SESSION_H

#ifdef __cplusplus
extern "C" {
#endif

/*!
@addtogroup api_session
@{
*/

#include <lttng/constant.h>

struct lttng_handle;
struct lttng_session_descriptor;
struct lttng_destruction_handle;

#define LTTNG_SESSION_PADDING1             8

/*!
@brief
    Recording session summary.

The purpose of such a structure is to provide information about a
recording session itself, but not about its
\ref api-channel-domain "tracing domains" and
\ref api-channel-channel "channels".

lttng_list_sessions() sets a pointer to an array of all the available
recording session summaries.
*/
struct lttng_session {
	/// Name.
	char name[LTTNG_NAME_MAX];

	/// Human-readable representation of the output (local or remote).
	char path[PATH_MAX];

	/*!
	@brief
	    1 if this recording session is active (started), or 0
	    otherwise.
	*/
	uint32_t enabled;	/* enabled/started: 1, disabled/stopped: 0 */

	/*!
	@brief
	    1 if this recording session was created in
	    \ref api-session-snapshot-mode "snapshot mode",
	    or 0 otherwise.

	@note
	    If this member is 1, then the
	    lttng_session::live_timer_interval member is 0.
	*/
	uint32_t snapshot_mode;

	/*!
	@brief
	    LTTng live timer period (µs), or 0 if this recording session
	    wasn't created in \ref api-session-live-mode "live mode".

	@note
	    If this member is \em not 0, then the
	    lttng_session::snapshot_mode member is 0.
	*/
	unsigned int live_timer_interval;	/* usec */

	/*
	 * End of public attributes.
	 * The remaining fields are used to deal with ABI management concerns.
	 */

	/*
	 * 32-bit architectures are already naturally aligned on 4 bytes after
	 * 'live_timer_interval'. However, the offset does not result in a
	 * natural alignment on 64-bit architectures. Adding 4 bytes of
	 * padding here results in an aligned offset after 'alignement_padding'
	 * for both bitnesses.
	 *
	 * This was added since not all compilers appear to align unions in the
	 * same way. Some (e.g. MSVC) do not seem to impose an alignement
	 * constraint while others (e.g. gcc, clang, icc) seem to align it to
	 * ensure 'ptr' is naturally aligned.
	 */
	char alignment_padding[4];
	union {
		/*
		 * Ensure the 'extended' union has the same size for both
		 * 32-bit and 64-bit builds.
		 */
		char padding[LTTNG_SESSION_PADDING1];
		void *ptr;
	} extended;
};

/*
 * Create a session on the session daemon from a session descriptor.
 *
 * See the session descriptor API description in session-descriptor.h
 *
 * Note that unspecified session descriptor parameters, such as a session's
 * name, are updated in the session descriptor if the creation of the session
 * succeeds. This allows users to query the session's auto-generated name
 * after its creation. Note that other attributes can be queried using the
 * session listing API.
 *
 * Returns LTTNG_OK on success. See lttng-error.h for the meaning of the other
 * return codes.
 */

/*!
@brief
    Creates a recording session from the recording session descriptor
    \lt_p{session_descriptor}.

See \ref api_session_descr to learn how to create a recording session
descriptor.

On success, this function sets unspecified properties of
\lt_p{session_descriptor} to their actual values. For example, if you
don't specify a name when you create \lt_p{session_descriptor}, then,
after successfully calling this function, you
can call lttng_session_descriptor_get_session_name() to get the
automatically generated name of the created recording session.

@param[in] session_descriptor
    Descriptor from which to create a recording session.

@returns
    #LTTNG_OK on success, or a \em negative enumerator otherwise.

@lt_pre_conn
@lt_pre_not_null{session_descriptor}
@pre
    If the name property of \lt_p{session_descriptor} is set, then no
    available recording session has this name.

@sa \ref api_session_descr.
*/
extern enum lttng_error_code lttng_create_session_ext(
		struct lttng_session_descriptor *session_descriptor);

/*!
@brief
    Creates a recording session named \lt_p{session_name} in
    \ref api-session-local-mode "local"
    or \ref api-session-net-mode "network streaming" mode, optionally
    setting its output URL to \lt_p{output_url}.

@deprecated
    Use lttng_create_session_ext() with a dedicated
    local or network streaming
    \ref api_session_descr "recording session descriptor".

@param[in] session_name
    Name of the new recording session.
@param[in] output_url
    @parblock
    The output URL of the recording session to create: a
    \ref api-session-local-url "local URL" or a
    \ref api-session-two-port-url "remote URL".

    May be \c NULL, in which case LTTng doesn't write any trace data
    for this recording session.
    @endparblock

@returns
    0 on success, or a \em negative #lttng_error_code enumerator
    otherwise.

@lt_pre_conn
@lt_pre_not_null{session_name}
@pre
    No available recording session is named \lt_p{session_name}.

@sa lttng_create_session_snapshot() --
    Creates a recording session in snapshot mode.
@sa lttng_create_session_live() --
    Creates a recording session in live mode.
*/
extern int lttng_create_session(const char *session_name,
		const char *output_url);

/*!
@brief
    Creates a recording session named \lt_p{session_name} in
    \ref api-session-snapshot-mode "snapshot" mode, optionally setting
    the URL of its initial snapshot output to \lt_p{output_url}.

@deprecated
    Use lttng_create_session_ext() with a dedicated snapshot
    \ref api_session_descr "recording session descriptor".

@param[in] session_name
    Name of the new recording session.
@param[in] output_url
    @parblock
    If not \c NULL, the URL of an initial snapshot output which
    LTTng adds for this recording session.

    If \c NULL, then the created recording session has no snapshot
    output: you need to either add one with lttng_snapshot_add_output()
    or provide one when you take a snapshot with
    lttng_snapshot_record().
    @endparblock

@returns
    0 on success, or a \em negative #lttng_error_code enumerator
    otherwise.

@lt_pre_conn
@lt_pre_not_null{session_name}
@pre
    No available recording session is named \lt_p{session_name}.

@sa lttng_create_session() --
    Creates a recording session in local or network streaming mode.
@sa lttng_create_session_live() --
    Creates a recording session in live mode.
*/
extern int lttng_create_session_snapshot(const char *session_name,
		const char *output_url);

/*
 * Create a session exclusively used for live reading.
 *
 * In this mode, the switch-timer parameter is forced for each UST channel, a
 * live-switch-timer is enabled for kernel channels, manually setting
 * switch-timer is forbidden. Synchronization beacons are sent to the relayd,
 * indexes are sent and metadata is checked for each packet.
 *
 * Name can't be NULL. If no URL is given, the default is to send the data to
 * net://127.0.0.1. The timer_interval is in usec.
 *
 * Return 0 on success else a negative LTTng error code.
 */

/*!
@brief
    Creates a recording session named \lt_p{session_name} in
    \ref api-session-live-mode "live" mode, optionally setting its
    URL to \lt_p{output_url}.

@deprecated
    Use lttng_create_session_ext() with a dedicated live
    \ref api_session_descr "recording session descriptor".

@param[in] session_name
    Name of the new recording session.
@param[in] output_url
    @parblock
    The output \ref api-session-two-port-url "remote" URL of the
    recording session to create.

    If \c NULL, this function uses <code>net://127.0.0.1</code>.
    @endparblock
@param[in] live_timer_period
    Period (µs) of the \ref api-channel-live-timer "live timers" of all
    the channels of the created recording session.

@returns
    0 on success, or a \em negative #lttng_error_code enumerator
    otherwise.

@lt_pre_conn
@lt_pre_not_null{session_name}
@pre
    No available recording session is named \lt_p{session_name}.
@pre
    \lt_p{live_timer_period}&nbsp;≥&nbsp;1

@sa lttng_create_session() --
    Creates a recording session in local or network streaming mode.
@sa lttng_create_session_snapshot() --
    Creates a recording session in snapshot mode.
*/
extern int lttng_create_session_live(const char *session_name,
		const char *output_url, unsigned int live_timer_period);

/*!
@brief
    Destroys the recording session named \lt_p{session_name}, blocking
    until the operation completes.

@deprecated
    Use lttng_destroy_session_ext().

"Destroying" a recording session means freeing the resources which the
LTTng daemons and tracers acquired for it, also making sure to flush all
the recorded trace data to either the local file system or the connected
LTTng relay daemon (see \lt_man{lttng-relayd,8}), depending on the
\ref api-session-modes "recording session mode".

This function stops any recording activity within the recording session
named \lt_p{session_name}.

This function implicitly calls lttng_stop_tracing(), blocking until the
trace data of the recording session becomes valid. Use
lttng_destroy_session_no_wait() to avoid a blocking call.

@param[in] session_name
    Name of the recording session to destroy.

@returns
    0 on success, or a \em negative #lttng_error_code enumerator
    otherwise.

@lt_pre_conn
@lt_pre_not_null{session_name}
@lt_pre_sess_exists{session_name}

@sa lttng_destroy_session_no_wait() --
    Destroys a recording session without waiting for the operation
    to complete.
*/
extern int lttng_destroy_session(const char *session_name);

/*!
@brief
    Destroys the recording session named \lt_p{session_name} without waiting
    for the operation to complete.

@deprecated
    Use lttng_destroy_session_ext().

"Destroying" a recording session means freeing the resources which the
LTTng daemons and tracers acquired for it, also making sure to flush all
the recorded trace data to either the local file system or the connected
LTTng relay daemon (see \lt_man{lttng-relayd,8}), depending on the
\ref api-session-modes "recording session mode".

Unlike lttng_destroy_session(), this function does \em not block until
the destruction operation is complete: it returns immediately. This
means the traces(s) of the recording session might not be valid when
this function returns, and there's no way to know when it/they become
valid.

@param[in] session_name
    Name of the recording session to destroy.

@returns
    0 on success, or a \em negative #lttng_error_code enumerator
    otherwise.

@lt_pre_conn
@lt_pre_not_null{session_name}
@lt_pre_sess_exists{session_name}
@pre
    No destruction operation is in progress for the recording session
    named \lt_p{session_name}.

@sa lttng_destroy_session() --
    Destroys a recording session, blocking until the operation
    completes.
*/
extern int lttng_destroy_session_no_wait(const char *session_name);

/*!
@brief
    Destroys the recording session named \lt_p{session_name}.

"Destroying" a recording session means freeing the resources which the
LTTng daemons and tracers acquired for it, also making sure to flush all
the recorded trace data to either the local file system or the connected
LTTng relay daemon (see \lt_man{lttng-relayd,8}), depending on the
\ref api-session-modes "recording session mode".

This function doesn't block until the destruction operation completes.
Use \lt_p{*handle} to wait for the operation to complete.

@param[in] session_name
    Name of the recording session to destroy.
@param[out] handle
    @parblock
    <strong>On success</strong>, this function sets \lt_p{*handle} to
    a handle which identifies this recording session destruction
    operation.

    May be \c NULL.

    Wait for the completion of this destruction operation with
    lttng_destruction_handle_wait_for_completion().

    Destroy \lt_p{*handle} with lttng_destruction_handle_destroy().
    @endparblock

@returns
    #LTTNG_OK on success, or a \em negative enumerator otherwise.

@lt_pre_conn
@lt_pre_not_null{session_name}
@lt_pre_sess_exists{session_name}
@pre
    No destruction operation is in progress for the recording session
    named \lt_p{session_name}.
*/
extern enum lttng_error_code lttng_destroy_session_ext(const char *session_name,
		struct lttng_destruction_handle **handle);

/*!
@brief
    Sets \lt_p{*sessions} to the summaries of all the available
    recording sessions.

@param[out] sessions
    @parblock
    <strong>On success</strong>, this function sets \lt_p{*sessions} to
    the summaries of the available recording sessions.

    Free \lt_p{*sessions} with <code>free()</code>.
    @endparblock

@returns
    The number of items in \lt_p{*sessions} on success, or a \em
    negative #lttng_error_code enumerator otherwise.

@lt_pre_conn
@lt_pre_not_null{sessions}
*/
extern int lttng_list_sessions(struct lttng_session **sessions);

/*!
@brief
    Sets \lt_p{*creation_timestamp} to the timestamp of the creation of
    the recording session summarized by \lt_p{session}.

@param[in] session
    Summary of the recording session for which to get the creation
    timestamp, as obtained with lttng_list_sessions().
@param[out] creation_timestamp
    <strong>On success</strong>, this function sets
    \lt_p{*creation_timestamp} to the Unix timestamp of the creation of
    \lt_p{session}.

@returns
    #LTTNG_OK on success, or a \em negative enumerator otherwise.

@lt_pre_conn
@lt_pre_not_null{session}
@pre
    The recording session summarized by \lt_p{session} is accessible
    within the connected session daemon.
@lt_pre_not_null{creation_timestamp}
*/
extern enum lttng_error_code lttng_session_get_creation_time(
		const struct lttng_session *session,
		uint64_t *creation_timestamp);

/*
 * Set the shared memory path for a session.
 *
 * Sets the (optional) file system path where shared memory buffers will
 * be created for the session. This is useful for buffer extraction on
 * crash, when used with filesystems like pramfs.
 *
 * Return 0 on success else a negative LTTng error code.
 */


/*!
@brief
    Sets the path of the directory containing the shared memory files
    holding the channel ring buffers of the recording session named
    \lt_p{session_name} on the local file sytem to \lt_p{shm_dir}.

Specifying a location on an
<a href="https://en.wikipedia.org/wiki/Non-volatile_random-access_memory">NVRAM</a>
file system makes it possible to recover the latest recorded trace data
when the system reboots after a crash with the \lt_man{lttng-crash,1}
utility.

@param[in] session_name
    Name of the recording session of which to set the shared memory
    file directory path.
@param[in] shm_dir
    Path of the directory containing the shared memory files of the
    recording session named \lt_p{session_name}.

@returns
    0 on success, or a \em negative #lttng_error_code enumerator
    otherwise.

@lt_pre_conn
@lt_pre_not_null{session_name}
@lt_pre_sess_exists{session_name}
@lt_pre_sess_never_active{session_name}
@lt_pre_not_null{shm_dir}
@pre
    \lt_p{shm_dir} is a writable directory.
*/
extern int lttng_set_session_shm_path(const char *session_name,
		const char *shm_dir);

/// @}

#ifdef __cplusplus
}
#endif

#endif /* LTTNG_SESSION_H */
