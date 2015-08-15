/**
 * @file ds.h
 * @brief Data sets
 * @note Copyright (C) 2011 Richard Cochran <richardcochran@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#ifndef HAVE_DS_H
#define HAVE_DS_H

#include "ddt.h"
#include "fault.h"
#include "filter.h"
#include "tsproc.h"

/* clock data sets */

#define DDS_TWO_STEP_FLAG (1<<0)
#define DDS_SLAVE_ONLY    (1<<1)

struct defaultDS {
	UInteger8            flags;
	UInteger8            reserved1;
	UInteger16           numberPorts;
	UInteger8            priority1;
	struct ClockQuality  clockQuality;
	UInteger8            priority2;
	struct ClockIdentity clockIdentity;
	UInteger8            domainNumber;
	UInteger8            reserved2;
} PACKED;

#define OUI_LEN 3
struct clock_description {
	struct static_ptp_text productDescription;
	struct static_ptp_text revisionData;
	struct static_ptp_text userDescription;
	Octet manufacturerIdentity[OUI_LEN];
};

struct default_ds {
	struct defaultDS dds;
	int free_running;
	int freq_est_interval; /*log seconds*/
	int grand_master_capable; /*802.1AS only*/
	int stats_interval; /*log seconds*/
	int kernel_leap;
	int sanity_freq_limit;
	int time_source;
	struct clock_description clock_desc;
	enum tsproc_mode tsproc_mode;
	enum filter_type delay_filter;
	int delay_filter_length;
	int boundary_clock_jbod;
};

struct dataset {
	UInteger8            priority1;
	struct ClockIdentity identity;
	struct ClockQuality  quality;
	UInteger8            priority2;
	UInteger16           stepsRemoved;
	struct PortIdentity  sender;
	struct PortIdentity  receiver;
};

struct currentDS {
	UInteger16   stepsRemoved;
	TimeInterval offsetFromMaster;
	TimeInterval meanPathDelay;
} PACKED;

struct parentDS {
	struct PortIdentity  parentPortIdentity;
	UInteger8            parentStats;
	UInteger8            reserved;
	UInteger16           observedParentOffsetScaledLogVariance;
	Integer32            observedParentClockPhaseChangeRate;
	UInteger8            grandmasterPriority1;
	struct ClockQuality  grandmasterClockQuality;
	UInteger8            grandmasterPriority2;
	struct ClockIdentity grandmasterIdentity;
} PACKED;

struct parent_ds {
	struct parentDS pds;
	struct ClockIdentity *ptl;
	unsigned int path_length;
};

#define CURRENT_UTC_OFFSET  35 /* 1 Jul 2012 */
#define INTERNAL_OSCILLATOR 0xA0

struct timePropertiesDS {
	Integer16    currentUtcOffset;
	UInteger8    flags;
	Enumeration8 timeSource;
} PACKED;

struct portDS {
	struct PortIdentity portIdentity;
	Enumeration8        portState;
	Integer8            logMinDelayReqInterval;
	TimeInterval        peerMeanPathDelay;
	Integer8            logAnnounceInterval;
	UInteger8           announceReceiptTimeout;
	Integer8            logSyncInterval;
	Enumeration8        delayMechanism;
	Integer8            logMinPdelayReqInterval;
	UInteger8           versionNumber;
} PACKED;

#define FRI_ASAP (-128)

struct port_defaults {
	int follow_up_info;
	int freq_est_interval; /*log seconds*/
	struct fault_interval flt_interval_pertype[FT_CNT];
	UInteger32 neighborPropDelayThresh; /*nanoseconds*/
	int min_neighbor_prop_delay; /*nanoseconds*/
	int tx_timestamp_offset; /*nanoseconds*/
	int rx_timestamp_offset; /*nanoseconds*/
};

#endif
