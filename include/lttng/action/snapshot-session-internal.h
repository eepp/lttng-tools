/*
 * Copyright (C) 2019 Simon Marchi <simon.marchi@efficios.com>
 *
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 */

#ifndef LTTNG_ACTION_SNAPSHOT_SESSION_INTERNAL_H
#define LTTNG_ACTION_SNAPSHOT_SESSION_INTERNAL_H

#include <sys/types.h>

#include <common/macros.h>

struct lttng_action;
struct lttng_buffer_view;

/*
 * Create a "snapshot session" action from a buffer view.
 *
 * On success, return the number of bytes consumed from `view`, and the created
 * action in `*action`.  On failure, return -1.
 */
LTTNG_HIDDEN
extern ssize_t lttng_action_snapshot_session_create_from_buffer(
		const struct lttng_buffer_view *view,
		struct lttng_action **action);

#endif /* LTTNG_ACTION_SNAPSHOT_SESSION_INTERNAL_H */
