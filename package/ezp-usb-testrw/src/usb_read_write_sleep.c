#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define DEFAULT_FILE_NAME       "usb_test_read_write.txt"
#define DEFAULT_STORAGE_NAME    "USB"
#define DEFAULT_MOUNT_PATH      "/tmp/mnt"
#define DEFAULT_TEST_ROUND      100
// Use 128M as default test file size
#define DEFAULT_TEST_SIZE       1024*1024*128
#define SUSPENDTIME(X)          (X + 10)             
char wbuf[1024 << 3];
int main(int argc, char *argv[]) {
    int usbfd = -1;
    int idx, jdx, kdx;
    int testrnd = DEFAULT_TEST_ROUND;
    int testsz = DEFAULT_TEST_SIZE;
    int testszie = DEFAULT_TEST_ROUND;
    char *testfile = DEFAULT_FILE_NAME;
    char *testdev = DEFAULT_STORAGE_NAME;
    char *testmnt = DEFAULT_MOUNT_PATH;
    char test_fn_buf[128];
    char data;
    int suspend_time = 0;
    for(idx = 1; idx < argc; idx ++) {
        if(argv[idx] && argv[idx][0]) {
            if(!strcmp(argv[idx], "-f")) {
                idx ++;
                if(idx < argc && argv[idx] && argv[idx][0]) {
                    testfile = argv[idx];
                } else {
                    fprintf(stderr, "Error -f option\n");
                }
            } else if(!strcmp(argv[idx], "-d")) {
                idx ++;
                if(idx < argc && argv[idx] && argv[idx][0]) {
                    testdev = argv[idx];
                } else {
                    fprintf(stderr, "Error -d option\n");
                }
            } else if(!strcmp(argv[idx], "-m")) {
                idx ++;
                if(idx < argc && argv[idx] && argv[idx][0]) {
                    testmnt = argv[idx];
                } else {
                    fprintf(stderr, "Error -m option\n");
                }
            } else if(!strcmp(argv[idx], "-r")) {
                idx ++;
                if(idx < argc && argv[idx] && argv[idx][0]) {
                    testrnd = atoi(argv[idx]);
                } else {
                    fprintf(stderr, "Error -r option\n");
                }
            } else if(!strcmp(argv[idx], "-z")) {
                idx ++;
                if(idx < argc && argv[idx] && argv[idx][0]) {
                    testsz = atoi(argv[idx]);
                } else {
                    fprintf(stderr, "Error -z option\n");
                }
            }
        }
    }
    snprintf(test_fn_buf, sizeof(test_fn_buf), "%s/%s/%s", testmnt, testdev, testfile);
    printf("Using %s as test file, test %d rounds, each round %d bytes\n", test_fn_buf, testrnd, testsz);
    for(idx = 0;idx < testrnd; idx++) {
        printf("Round %d start, writing stage\n", idx);
        usbfd = open(test_fn_buf, O_CREAT | O_WRONLY);
        if(usbfd >= 0) {
            for(jdx = 0;jdx < testsz;jdx += (1024 << 3)) {
                printf("Write %d bytes, wrote %d bytes, total %d bytes\n", 1024 << 3, jdx, testsz);
                data = (jdx % 127) + 1;
                memset(wbuf, data, sizeof(wbuf));
                write(usbfd, wbuf, sizeof(wbuf));
                sync();
                if((jdx + (1024 << 3)) < testsz) {
                    printf("Sleep %d seconds to ensure device suspend\n", SUSPENDTIME(suspend_time));
                    sleep(SUSPENDTIME(suspend_time));
                    // suspend_time += 10;
                }
            }
            printf("Write finished, next\n");
            close(usbfd);
        }
        usbfd = open(test_fn_buf, O_RDONLY);
        if(usbfd >= 0) {
            for(jdx = 0;jdx < testsz;jdx += (1024 << 3)) {
                printf("Read %d bytes, read %d bytes, total %d bytes\n", 1024 << 3, jdx, testsz);
                data = (jdx % 127) + 1;
                read(usbfd, wbuf, sizeof(wbuf));
                for(kdx = 0;kdx < (1024 << 3);kdx++) {
                    if(wbuf[kdx] != data) {
                        printf("data error\n");
                    }
                }
                if((jdx + (1024 << 3)) < testsz) {
                    printf("Sleep %d seconds to ensure device suspend\n", SUSPENDTIME(suspend_time));
                    sleep(SUSPENDTIME(suspend_time));
                }
            }
            printf("Read finished, round finished\n");
            close(usbfd);
        }
    }
    return 0;
}
