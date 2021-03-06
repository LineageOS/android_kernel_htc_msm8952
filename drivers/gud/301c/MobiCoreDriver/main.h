/*
 * Copyright (c) 2013 TRUSTONIC LIMITED
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */
#ifndef _MC_MAIN_H_
#define _MC_MAIN_H_

#include <asm/pgtable.h>
#include <linux/semaphore.h>
#include <linux/completion.h>
#include <linux/mutex.h>

#include "public/mc_linux.h"
#include "platform.h"

#define MC_VERSION(major, minor) \
		(((major & 0x0000ffff) << 16) | (minor & 0x0000ffff))

struct mc_instance {
	
	struct mutex lock;
	
	unsigned int handle;
	bool admin;
};

struct mc_buffer {
	struct list_head	list;
	
	unsigned int		handle;
	
	atomic_t		usage;
	
	void			*addr;
	
	void			*uaddr;
	
	phys_addr_t		phys;
	
	unsigned int		order;
	uint32_t		len;
	struct mc_instance	*instance;
};

struct mc_context {
	
	struct mc_buffer	mci_base;
	
	struct mc_mcp_buffer	*mcp;
	
	struct completion	isr_comp;
	
	unsigned int		evt_counter;
	atomic_t		isr_counter;
	
	atomic_t		buffer_counter;
	atomic_t		instance_counter;
	
	struct mc_instance	*daemon_inst;
	
	struct task_struct	*daemon;
	
	struct list_head	cont_bufs;
	
	struct mutex		bufs_lock;
};

struct mc_sleep_mode {
	uint16_t	sleep_req;
	uint16_t	ready_to_sleep;
};

#define SCHEDULE_IDLE		0
#define SCHEDULE_NON_IDLE	1

struct mc_flags {
	uint32_t		schedule;
	
	struct mc_sleep_mode	sleep_mode;
	
	uint32_t		rfu[2];
};

struct mc_mcp_buffer {
	
	struct mc_flags	flags;
	uint32_t	rfu; 
};

static inline bool is_daemon(struct mc_instance *instance)
{
	if (!instance)
		return false;
	return instance->admin;
}


struct mc_instance *mc_alloc_instance(void);
int mc_release_instance(struct mc_instance *instance);

int mc_register_wsm_mmu(struct mc_instance *instance,
	void *buffer, uint32_t len,
	uint32_t *handle, phys_addr_t *phys);
int mc_unregister_wsm_mmu(struct mc_instance *instance, uint32_t handle);

int mc_get_buffer(struct mc_instance *instance,
	struct mc_buffer **buffer, unsigned long len);
int mc_free_buffer(struct mc_instance *instance, uint32_t handle);

bool mc_check_owner_fd(struct mc_instance *instance, int32_t fd);

bool mc_sleep_ready(void);

#endif 
