#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <linux/uhid.h>

#include "common.h"

#include "uhid_input.h"
#include "minigui.h"

typedef struct LinuxEVCode2HIDCode_s {
    char linuxEVCodeName[32];
    unsigned char hidCode;
} LinuxEVCode2HIDCode_t;


LinuxEVCode2HIDCode_t EVCode2HIDcodeMap[] = {
    {"KEY_ESC", 0X29},
    {"KEY_1", 0X1E},
    {"KEY_2", 0X1F},
    {"KEY_3", 0X20},
    {"KEY_4", 0X21},
    {"KEY_5", 0X22},
    {"KEY_6", 0X23},
    {"KEY_7", 0X24},
    {"KEY_8", 0X25},
    {"KEY_9", 0X26},
    {"KEY_0", 0X27},
    {"KEY_MINUS", 0X2D},
    {"KEY_EQUAL", 0X2E},
    {"KEY_BACKSPACE", 0X2A},
    {"KEY_TAB", 0X2B},
    {"KEY_Q", 0X14},
    {"KEY_W", 0X1A},
    {"KEY_E", 0X08},
    {"KEY_R", 0X15},
    {"KEY_T", 0X17},
    {"KEY_Y", 0X1C},
    {"KEY_U", 0X18},
    {"KEY_I", 0X0C},
    {"KEY_O", 0X12},
    {"KEY_P", 0X13},
    {"KEY_LEFTBRACE", 0X26},
    {"KEY_RIGHTBRACE", 0X27},
    {"KEY_ENTER", 0X28},
    {"KEY_LEFTCTRL", 0XE0},
    {"KEY_A", 0X04},
    {"KEY_S", 0X16},
    {"KEY_D", 0X07},
    {"KEY_F", 0X09},
    {"KEY_G", 0X0A},
    {"KEY_H", 0X0B},
    {"KEY_J", 0X0D},
    {"KEY_K", 0X0E},
    {"KEY_L", 0X0F},
    {"KEY_SEMICOLON", 0X33},
    {"KEY_APOSTROPHE", 0X34},
    {"KEY_GRAVE", 0X32},
    {"KEY_LEFTSHIFT", 0XE1},
    {"KEY_BACKSLASH", 0X64},
    {"KEY_Z", 0X1D},
    {"KEY_X", 0X1B},
    {"KEY_C", 0X06},
    {"KEY_V", 0X19},
    {"KEY_B", 0X05},
    {"KEY_N", 0X11},
    {"KEY_M", 0X10},
    {"KEY_COMMA", 0X36},
    {"KEY_DOT", 0X37},
    {"KEY_SLASH", 0X38},
    {"KEY_RIGHTSHIFT", 0XE5},
    {"KEY_KPASTERISK", 0X55},
    {"KEY_LEFTALT", 0XE2},
    {"KEY_SPACE", 0X2C},
    {"KEY_CAPSLOCK", 0X39},
    {"KEY_F1", 0X3A},
    {"KEY_F2", 0X3B},
    {"KEY_F3", 0X3C},
    {"KEY_F4", 0X3D},
    {"KEY_F5", 0X3E},
    {"KEY_F6", 0X3F},
    {"KEY_F7", 0X40},
    {"KEY_F8", 0X41},
    {"KEY_F9", 0X42},
    {"KEY_F10", 0X43},
    {"KEY_NUMLOCK", 0X53},
    {"KEY_SCROLLLOCK", 0X54},
    {"KEY_KP7", 0X59},
    {"KEY_KP8", 0X60},
    {"KEY_KP9", 0X61},
    {"KEY_KPMINUS", 0X56},
    {"KEY_KP4", 0X5C},
    {"KEY_KP5", 0X5D},
    {"KEY_KP6", 0X5E},
    {"KEY_KPPLUS", 0X57},
    {"KEY_KP1", 0X59},
    {"KEY_KP2", 0X5A},
    {"KEY_KP3", 0X5B},
    {"KEY_KP0", 0X62},
    {"KEY_KPDOT", 0X63},
    {"KEY_DUMMY", 0xFF},
    {"KEY_ZENKAKUHANKAKU", 0xFF},
    {"KEY_102ND", 0XFF},
    {"KEY_F11", 0X44},
    {"KEY_F12", 0X45},
    {"KEY_RO", 0XFF},
    {"KEY_KATAKANA", 0XFF},
    {"KEY_HIRAGANA", 0XFF},
    {"KEY_HENKAN", 0XFF},
    {"KEY_KATAKANAHIRAGANA", 0XFF},
    {"KEY_MUHENKAN", 0XFF},
    {"KEY_KPJPCOMMA", 0XFF},
    {"KEY_KPENTER", 0XFF},
    {"KEY_RIGHTCTRL", 0XFF},
    {"KEY_KPSLASH", 0XFF},
    {"KEY_SYSRQ", 0XFF},
    {"KEY_RIGHTALT", 0XFF},
    {"KEY_LINEFEED", 0XFF},
    {"KEY_HOME", 0XFF},
    {"KEY_UP", 0XFF},
    {"KEY_PAGEUP", 0XFF},
    {"KEY_LEFT", 0XFF},
    {"KEY_RIGHT", 0XFF},
    {"KEY_END", 0XFF},
    {"KEY_DOWN", 0XFF},
    {"KEY_PAGEDOWN", 0XFF},
    {"KEY_INSERT", 0XFF},
    {"KEY_DELETE", 0XFF},
    {"KEY_MACRO", 0XFF},
    {"KEY_MUTE", 0XFF},
    {"KEY_VOLUMEDOWN", 0XFF},
    {"KEY_VOLUMEUP", 0XFF},
    {"KEY_POWER", 0XFF},
    {"KEY_KPEQUAL", 0XFF},
    {"KEY_KPPLUSMINUS", 0XFF},
    {"KEY_PAUSE", 0XFF},
    {"KEY_SCALE", 0XFF},
    {"KEY_KPCOMMA", 0XFF},
    {"KEY_HANGEUL", 0XFF},
    {"KEY_HANJA", 0XFF},
    {"KEY_YEN", 0XFF},
    {"KEY_LEFTMETA", 0XFF},
    {"KEY_RIGHTMETA", 0XFF},
    {"KEY_COMPOSE", 0XFF},
};


static unsigned char rdesc[] = {
    0x05, 0x01,	/* USAGE_PAGE (Generic Desktop) */
    0x09, 0x02,	/* USAGE (Mouse) */
    0xa1, 0x01,	/* COLLECTION (Application) */
    0x09, 0x01,		/* USAGE (Pointer) */
    0xa1, 0x00,		/* COLLECTION (Physical) */
    0x85, 0x01,			/* REPORT_ID (1) */
    0x05, 0x09,			/* USAGE_PAGE (Button) */
    0x19, 0x01,			/* USAGE_MINIMUM (Button 1) */
    0x29, 0x03,			/* USAGE_MAXIMUM (Button 3) */
    0x15, 0x00,			/* LOGICAL_MINIMUM (0) */
    0x25, 0x01,			/* LOGICAL_MAXIMUM (1) */
    0x95, 0x03,			/* REPORT_COUNT (3) */
    0x75, 0x01,			/* REPORT_SIZE (1) */
    0x81, 0x02,			/* INPUT (Data,Var,Abs) */
    0x95, 0x01,			/* REPORT_COUNT (1) */
    0x75, 0x05,			/* REPORT_SIZE (5) */
    0x81, 0x01,			/* INPUT (Cnst,Var,Abs) */
    0x05, 0x01,			/* USAGE_PAGE (Generic Desktop) */
    0x09, 0x30,			/* USAGE (X) */
    0x09, 0x31,			/* USAGE (Y) */
    0x09, 0x38,			/* USAGE (WHEEL) */
    0x15, 0x81,			/* LOGICAL_MINIMUM (-127) */
    0x25, 0x7f,			/* LOGICAL_MAXIMUM (127) */
    0x75, 0x08,			/* REPORT_SIZE (8) */
    0x95, 0x03,			/* REPORT_COUNT (3) */
    0x81, 0x06,			/* INPUT (Data,Var,Rel) */
    0xc0,			/* END_COLLECTION */
    0xc0,		/* END_COLLECTION */
    0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
    0x09, 0x06,        // Usage (Keyboard)
    0xA1, 0x01,        // Collection (Application)
    0x85, 0x02,        //   Report ID (1)
    0x05, 0x07,        //   Usage Page (Kbrd/Keypad)
    0x75, 0x01,        //   Report Size (1)
    0x95, 0x08,        //   Report Count (8)
    0x19, 0xE0,        //   Usage Minimum (0xE0)
    0x29, 0xE7,        //   Usage Maximum (0xE7)
    0x15, 0x00,        //   Logical Minimum (0)
    0x25, 0x01,        //   Logical Maximum (1)
    0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x95, 0x03,        //   Report Count (3)
    0x75, 0x08,        //   Report Size (8)
    0x15, 0x00,        //   Logical Minimum (0)
    0x25, 0x64,        //   Logical Maximum (100)
    0x05, 0x07,        //   Usage Page (Kbrd/Keypad)
    0x19, 0x00,        //   Usage Minimum (0x00)
    0x29, 0x65,        //   Usage Maximum (0x65)
    0x81, 0x00,        //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,              // End Collection
};

static int uhid_write(int fd, const struct uhid_event *ev)
{
    ssize_t ret;

    ret = write(fd, ev, sizeof(*ev));

    if(ret < 0) {
        fprintf(stderr, "Cannot write to uhid: %m\n");
        return -errno;
    } else if(ret != sizeof(*ev)) {
        fprintf(stderr, "Wrong size written to uhid: %d != %d\n",
                ret, sizeof(ev));
        return -EFAULT;
    } else {
        return 0;
    }
}

static int create(int fd)
{
    struct uhid_event ev;

    memset(&ev, 0, sizeof(ev));
    ev.type = UHID_CREATE;
    strcpy((char *)ev.u.create.name, "sstar-uhid-input");
    ev.u.create.rd_data = rdesc;
    ev.u.create.rd_size = sizeof(rdesc);
    ev.u.create.bus = BUS_USB;
    ev.u.create.vendor = 0x15d9;
    ev.u.create.product = 0x0a37;
    ev.u.create.version = 0;
    ev.u.create.country = 0;

    return uhid_write(fd, &ev);
}

static void destroy(int fd)
{
    struct uhid_event ev;

    memset(&ev, 0, sizeof(ev));
    ev.type = UHID_DESTROY;

    uhid_write(fd, &ev);
}

static int keyboard(int fd, unsigned char modifier, unsigned char identifier)
{
    struct uhid_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.type = UHID_INPUT;
    ev.u.input.size = 5;

    ev.u.input.data[0] = 0x2;

    ev.u.input.data[1] |= modifier;


    ev.u.input.data[2] = identifier;
    uhid_write(fd, &ev);
    //usleep(1000);
    ev.u.input.data[1] = 0;
    ev.u.input.data[2] = 0;
    return uhid_write(fd, &ev);
}


#define DELIMITER "-: |"
#include "hashtable.h"
#define MAX_KEYPAIR_STR_LEN  64
#include "ugpi_input.h"
#include <pthread.h>
typedef struct keypair_s {
    char pinName[MAX_KEYPAIR_STR_LEN];
    unsigned char pinIdx;
    unsigned char modifier;
    unsigned char identifier;
} keypair_t;




pthread_t gpi_thread;
keypair_t *key_map = NULL;

int gpi_threadbody(void *arg)
{
    struct pollfd *fdset;
    int gpio_fd, rc;
    char *buf[MAX_BUF];
    //unsigned int gpio=0;
    //int len;
    int keypair_num  = *((int *)arg);
    int fd,ret;
    const char *path = "/dev/uhid";
    fdset = calloc(sizeof(struct pollfd), keypair_num);

    fd = open(path, O_RDWR | O_CLOEXEC);

    if(fd < 0) {
        fprintf(stderr, "Cannot open uhid-cdev %s: %m\n", path);
        return -1;
    }

    ret = create(fd);

    if(ret) {
        close(fd);
        return -1;
    }

    for(int i = 0, gpio = key_map[i].pinIdx; i < keypair_num; i++, gpio = key_map[i].pinIdx) {
        gpio_export(gpio);
        gpio_set_dir(gpio, 0);
        gpio_set_edge(gpio, "rising");
        gpio_fd = gpio_fd_open(gpio);
        fdset[i].fd = gpio_fd;
        fdset[i].events = POLLPRI;
    }

    while(1) {
        rc = poll(fdset, keypair_num, POLL_TIMEOUT);

        if(rc < 0) {
            printf("\ngpi poll() failed!\n");
            return -1;
        }

        for(int i = 0; i < keypair_num; i++) {
            if(fdset[i].revents & POLLPRI) {
                lseek(fdset[i].fd, 0, SEEK_SET);    /* consume interrupt */
                read(fdset[i].fd, buf, MAX_BUF);
                //printf("\npoll() GPIO %d interrupt occurred ,len%d\n", gpio,len);
                keyboard(fd, key_map[i].modifier, key_map[i].identifier);

            }
        }
    }

    for(int i = 0, gpio = key_map[i].pinIdx; i < keypair_num; i++, gpio = key_map[i].pinIdx) {
        gpio_fd_close(fdset[i].fd);
        gpio_unexport(gpio);
    }

    destroy(fd);
    free(fdset);
    return 0;
}

int uhid_input_init(void)
{
    //int ret;
    char keymap[1024];
    int keyset[1024];
    char *keypair, *saveptr, *strtoken, *got;
    int key = 0, i = 0;
    size_t value_len;

    hash_table ht;
    hash_table ht_EVC2HIDC;
    GetMgEtcValue("console", "keymap", keymap, sizeof(keymap));
    ht_init(&ht, HT_KEY_CONST | HT_VALUE_CONST, 0.05);
    ht_init(&ht_EVC2HIDC, HT_KEY_CONST | HT_VALUE_CONST, 0.05);

    for(i = 0; i < sizeof(EVCode2HIDcodeMap) / sizeof(LinuxEVCode2HIDCode_t); i++) {
        ht_insert(&ht_EVC2HIDC, EVCode2HIDcodeMap[i].linuxEVCodeName,   \
                  strlen(EVCode2HIDcodeMap[i].linuxEVCodeName) + 1, &EVCode2HIDcodeMap[i].hidCode, sizeof(unsigned char));
    }

    for(i = 0; i < 1024; i++)
        keyset[i] = i;

    for(strtoken = keymap; ; strtoken = NULL, key++) {
        keypair = strtok_r(strtoken, DELIMITER, &saveptr);

        if(keypair == NULL)
            break;

        value_len = strlen(keypair) + 1;
        ht_insert(&ht, &keyset[key], sizeof(key), keypair, value_len);
        fprintf(stderr, "%s %d %s\n", __FUNCTION__, __LINE__, keypair);
    }

    unsigned int num_keys;
    void **keys;
    keys = ht_keys(&ht, &num_keys);
    key_map = calloc(sizeof(keypair_t), num_keys);

    for(i = 0; i < num_keys; i++) {

        strtoken = ht_get(&ht, keys[i], sizeof(key), &value_len);

        if(strtoken == NULL) {
            fprintf(stderr, "%s %d key %d missing\n", __FUNCTION__, __LINE__, *((int *)keys[i]));
            continue;
        }

        if(GetMgEtcValue("console", strtoken, keymap, sizeof(keymap)) != 0) {
            fprintf(stderr, "%s %d %s missing\n", __FUNCTION__, __LINE__, strtoken);
            break;
        } else {
            fprintf(stderr, "%s %d %s hit %s\n", __FUNCTION__, __LINE__, strtoken, keymap);
        }

        //1 //pin name
        strtoken = strtok_r(keymap, DELIMITER, &saveptr);
        snprintf(key_map[i].pinName, MAX_KEYPAIR_STR_LEN, "%s", strtoken);
        //1 //pin idx
        strtoken = strtok_r(NULL, DELIMITER, &saveptr);
        key_map[i].pinIdx = atoi(strtoken);
        //1 //modifier
        strtoken = strtok_r(NULL, DELIMITER, &saveptr);
        got = ht_get(&ht_EVC2HIDC, strtoken, strlen(strtoken) + 1, &value_len);

        if(got != NULL) {
            key_map[i].modifier = *((unsigned char *)got);
        }

        //1 //identifier
        strtoken = strtok_r(NULL, DELIMITER, &saveptr);

        if(strtoken == NULL) {
            key_map[i].identifier = key_map[i].modifier;
            key_map[i].modifier = 0;
        } else {
            got = ht_get(&ht_EVC2HIDC, strtoken, strlen(strtoken) + 1, &value_len);

            if(got != NULL) {
                key_map[i].identifier = *((unsigned char *)got);
            } else {
                fprintf(stderr, "invalid keyboard identifier\n");;
                key_map[i].identifier = 0;
            }
        }
    }

    pthread_create(&gpi_thread, NULL, (void*)gpi_threadbody, &num_keys);
    ht_destroy(&ht);
    ht_destroy(&ht_EVC2HIDC);
    return 0;
}


