/* include/asm/mach-msm/htc_acoustic_alsa.h
 *
 * Copyright (C) 2012 HTC Corporation.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/wakelock.h>
#include <linux/sched.h>
#include <linux/qdsp6v2/apr.h>

#ifndef _ARCH_ARM_MACH_MSM_HTC_ACOUSTIC_QCT_ALSA_H_
#define _ARCH_ARM_MACH_MSM_HTC_ACOUSTIC_QCT_ALSA_H_

#define AUD_HW_NUM			8
#define HTC_AUDIO_TPA2051	0x01
#define HTC_AUDIO_TPA2026	0x02
#define HTC_AUDIO_TPA2028	0x04
#define HTC_AUDIO_A1028		0x08
#define HTC_AUDIO_TPA6185 	0x10
#define HTC_AUDIO_RT5501 	0x20
#define HTC_AUDIO_TFA9887 	0x40
#define HTC_AUDIO_TFA9887L 	0x80

//HTC_AUD_START
#define HTC_AUDIO_TFA9888   0x200
#define HTC_AUDIO_RT5503    0x400
//HTC_AUD_END

#define ACOUSTIC_IOCTL_MAGIC 'p'
#define ACOUSTIC_SET_Q6_EFFECT                  _IOW(ACOUSTIC_IOCTL_MAGIC, 43, unsigned)
#define ACOUSTIC_GET_HTC_REVISION               _IOR(ACOUSTIC_IOCTL_MAGIC, 44, unsigned)
#define ACOUSTIC_GET_HW_COMPONENT               _IOR(ACOUSTIC_IOCTL_MAGIC, 45, unsigned)
#define ACOUSTIC_GET_DMIC_INFO                  _IOR(ACOUSTIC_IOCTL_MAGIC, 46, unsigned)
#define ACOUSTIC_UPDATE_BEATS_STATUS            _IOW(ACOUSTIC_IOCTL_MAGIC, 47, unsigned)
#define ACOUSTIC_UPDATE_LISTEN_NOTIFICATION     _IOW(ACOUSTIC_IOCTL_MAGIC, 48, unsigned)
#define ACOUSTIC_GET_HW_REVISION                _IOR(ACOUSTIC_IOCTL_MAGIC, 49, struct device_info)
#define ACOUSTIC_CONTROL_WAKELOCK               _IOW(ACOUSTIC_IOCTL_MAGIC, 77, unsigned)
#define ACOUSTIC_DUMMY_WAKELOCK                 _IOW(ACOUSTIC_IOCTL_MAGIC, 78, unsigned)
#define ACOUSTIC_TFA_CONTROL_WAKELOCK           _IOW(ACOUSTIC_IOCTL_MAGIC, 79, unsigned)
#define ACOUSTIC_AMP_CTRL                       _IOWR(ACOUSTIC_IOCTL_MAGIC, 50, struct amp_ctrl)
#define ACOUSTIC_GET_MID                        _IOR(ACOUSTIC_IOCTL_MAGIC, 51, unsigned)
#define ACOUSTIC_RAMDUMP                        _IOW(ACOUSTIC_IOCTL_MAGIC, 99, unsigned)
#define ACOUSTIC_KILL_PID                       _IOW(ACOUSTIC_IOCTL_MAGIC, 88, unsigned)
#define ACOUSTIC_UPDATE_DQ_STATUS               _IOW(ACOUSTIC_IOCTL_MAGIC, 52, unsigned)
#define ACOUSTIC_ADSP_CMD                       _IOW(ACOUSTIC_IOCTL_MAGIC, 98, unsigned)
#define ACOUSTIC_NOTIFIY_FM_SSR                 _IOW(ACOUSTIC_IOCTL_MAGIC, 53, unsigned)
#define ACOUSTIC_GET_TFA_VER                    _IOR(ACOUSTIC_IOCTL_MAGIC, 97, unsigned)

/* For FTM mode BT PCM PINs */
#define ACOUSTIC_FTM_BTPCM_SET_GPIO             _IOW(ACOUSTIC_IOCTL_MAGIC, 60, unsigned)
#define ACOUSTIC_FTM_BTPCM_SET_PCM              _IOW(ACOUSTIC_IOCTL_MAGIC, 61, unsigned)
#define ACOUSTIC_FTM_BTPCM_READ                 _IOR(ACOUSTIC_IOCTL_MAGIC, 62, unsigned)

/* BT PCM GPIOs as GPIO input */
#define ACOUSTIC_FTM_STRING_BTPCM_GPIO	("btpcm-gpio")
/* BT PCM GPIOs as PCM */
#define ACOUSTIC_FTM_STRING_BTPCM_PCM	("btpcm-pcm")
/* BT PCM GPIOs simulation on non-BRCM projects */
#define ACOUSTIC_FTM_STRING_BTPCM_SIM	("btpcm-sim")

#define AUD_AMP_SLAVE_ALL	0xffff

#define AUD_FTM_DEBUG (1)

enum HTC_FEATURE {
	HTC_Q6_EFFECT = 0,
	HTC_AUD_24BIT,
};

enum SPK_AMP_TYPE {
	SPK_AMP_RIGHT = 0,
	SPK_AMP_LEFT,
	SPK_AMP_MAX,
};

enum ACOU_AMP_CTRL {
	AMP_READ = 0,
	AMP_WRITE,
};

enum AMP_TYPE {
	AMP_HEADPONE = 0,
	AMP_SPEAKER,
	AMP_RECEIVER,
};

enum HS_NOTIFY_TYPE {
	HS_AMP_N = 0,
	HS_CODEC_N,
	HS_N_MAX,
};

enum AUD_FTM_BTPCM_MODE {
	AUD_FTM_BTPCM_MODE_PCM = 0,
	AUD_FTM_BTPCM_MODE_GPIO,
	AUD_FTM_BTPCM_MODE_SIM,
	AUD_FTM_BTPCM_MODE_CNT,
};

struct hw_component {
	const char *name;
	const unsigned int id;
};

struct amp_register_s {
	int (*aud_amp_f)(int, int);
	struct file_operations* fops;
};

struct amp_ctrl {
	enum ACOU_AMP_CTRL ctrl;
	enum AMP_TYPE type;
	unsigned short slave;
	unsigned int reg;
	unsigned int val;
};

struct acoustic_ops {
	void (*set_q6_effect)(int mode);
	int (*get_htc_revision)(void);
	int (*get_hw_component)(void);
	char* (*get_mid)(void);
	int (*enable_digital_mic)(void);
	int (*enable_24b_audio)(void);
	int (*get_q6_effect) (void);
};

struct hs_notify_t {
	int used;
	void *private_data;
	int (*callback_f)(void*,int);
};

struct avcs_crash_params {
    struct apr_hdr  hdr;
    uint32_t crash_type;
};

struct aud_ftm_btpcm_func_t {
	/* structure initialized or not */
	int init;

	/* simulation on projects without BRCM BT chip */
	int sim;

	/* function to set BT PCM as GPIO input or PCM function */
	int (*gpio_config)(void *user_data, enum AUD_FTM_BTPCM_MODE mode);

	/* function to read BT PCM pin state
	* state[0]: SYNC/Frame clock
	* state[1]: Bit clock
	* state[2]: Data In (BT to ACPU)
	* state[3]: Data Out (ACPU to BT) */
	int (*gpio_read)(void *user_data, int *state);

	void *user_data;
};

void htc_acoustic_register_spk_version(void (*spk_func)(unsigned char*));
void htc_acoustic_register_ops(struct acoustic_ops *ops);
void htc_acoustic_register_hs_amp(int (*aud_hs_amp_f)(int, int), struct file_operations* ops);
int htc_acoustic_hs_amp_ctrl(int on, int dsp);
void htc_acoustic_register_spk_amp(enum SPK_AMP_TYPE type,int (*aud_spk_amp_f)(int, int), struct file_operations* ops);
int htc_acoustic_spk_amp_ctrl(enum SPK_AMP_TYPE type,int on, int dsp);
/* To query if feature is enable */
int htc_acoustic_query_feature(enum HTC_FEATURE feature);
void htc_acoustic_register_hs_notify(enum HS_NOTIFY_TYPE type, struct hs_notify_t *notify);

void aud_ftm_func_register(struct aud_ftm_btpcm_func_t *funcs);

struct amp_power_ops {
	void (*set_amp_power_enable)(bool enable);
};

void htc_amp_power_register_ops(struct amp_power_ops *ops);
void htc_amp_power_enable(bool enable);
uint32_t htc_aud_component_to_flag(const char* hw_s);
#endif

