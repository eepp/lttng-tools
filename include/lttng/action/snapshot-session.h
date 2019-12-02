/*
 * Copyright (C) 2019 Simon Marchi <simon.marchi@efficios.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 */

#ifndef LTTNG_ACTION_SNAPSHOT_SESSION_H
#define LTTNG_ACTION_SNAPSHOT_SESSION_H

struct lttng_action;

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Create a newly allocated snapshot-session action object.
 *
 * A snapshot session action object must have a session name set to be
 * considered valid when used with a trigger object (lttng_trigger). A name can
 * be set using `lttng_action_snapshot_session_set_session_name`.
 *
 * Returns a new action on success, NULL on failure. This action must be
 * destroyed using lttng_action_destroy().
 */
extern struct lttng_action *lttng_action_snapshot_session_create(void);

/*
 * Set the session name of an lttng_action object of type
 * LTTNG_ACTION_TYPE_SNAPSHOT_SESSION.
 */
extern enum lttng_action_status lttng_action_snapshot_session_set_session_name(
		struct lttng_action *action, const char *session_name);

/*
 * Set the snapshot name of an lttng_action object of type
 * LTTNG_ACTION_TYPE_SNAPSHOT_SESSION.
 */
extern enum lttng_action_status lttng_action_snapshot_session_set_snapshot_name(
		struct lttng_action *action, const char *snapshot_name);

#ifdef __cplusplus
}
#endif

#endif /* LTTNG_ACTION_SNAPSHOT_SESSION_H */
