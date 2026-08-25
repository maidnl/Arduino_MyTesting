#include "compliance_sdcard.h"

static FATFS fat_fs;
/* Mount point mapping to the SD disk area */
static struct fs_mount_t mp = {
    .type = FS_FATFS,
    .mnt_point = "/SD:",
    .fs_data = &fat_fs,
};

void init_sdcard() {}

static const char *test_file1 = "/SD:/test1.txt";
static const char *test_file2 = "/SD:/test2.txt";

static const char *test_data =
    "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod "
    "tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim "
    "veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea "
    "commodo consequat. Duis aute irure dolor in reprehenderit in voluptate "
    "velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint "
    "occaecat cupidatat non proident, sunt in culpa qui officia deserunt "
    "mollit anim id est laborum.Sed ut perspiciatis unde omnis iste natus "
    "error sit voluptatem accusantium doloremque laudantium, totam rem "
    "aperiam, eaque ipsa quae ab illo inventore veritatis et quasi architecto "
    "beatae vitae dicta sunt explicabo. Nemo enim ipsam voluptatem quia "
    "voluptas sit aspernatur aut odit aut fugit, sed quia consequuntur magni "
    "dolores eos qui ratione voluptatem sequi nesciunt. Neque porro quisquam "
    "est, qui dolorem ipsum quia dolor sit amet, consectetur, adipisci velit, "
    "sed quia non numquam eius modi tempora incidunt ut labore et dolore "
    "magnam aliquam quaerat voluptatem. Ut enim ad minima veniam, quis nostrum "
    "exercitationem ullam corporis suscipit laboriosam, nisi ut aliquid ex ea "
    "commodi consequatur? Quis autem vel eum iure reprehenderit qui in ea "
    "voluptate velit esse quam nihil molestiae consequatur, vel illum qui "
    "dolorem eum fugiat quo voluptas nulla pariatur?";

/* ________________________________________________________CARD IS AVAILABLE */
bool card_is_available() {
  bool rv = false;
  static const char *disk_pdrv = "SD";

#ifdef LOG_SDCARD_TASK
  if (Serial)
    Serial.println("Check for SD Card presence");
#endif

  /* Loop until card is detected */
    int rc = disk_access_init(disk_pdrv);
    if (rc == 0) {
#ifdef LOG_SDCARD_TASK
      if (Serial)
        Serial.println("Card detected");
#endif
      rv = true;
    } else {

#ifdef LOG_SDCARD_TASK
      if (Serial)
        Serial.println("Card not detected. Waiting...");
#endif
      k_msleep(SD_CARD_WAIT_FOR_CARD_DETECTION_ms);
    }
  return rv;
}

/* _________________________________________________________MOUNT FILESYSTEM */
bool mount_filesystem() {
  int rc = fs_mount(&mp);
  if (rc != 0) {
#ifdef LOG_SDCARD_TASK
    if (Serial)
      Serial.println("Error mounting SD card (rc=%d). Is it formatted as FAT?");
#endif
  } else {
#ifdef LOG_SDCARD_TASK
    if (Serial)
      Serial.println("SD card mounted successfully");
#endif
  }
  return (rc == 0) ? true : false;
}

/* ____________________________________________________________WRITE on FILE */
bool write_test(const char *fname, const char *text) {
  bool rv = true;
  struct fs_file_t file;
  fs_file_t_init(&file);
  int rc = fs_open(&file, fname, FS_O_CREATE | FS_O_WRITE);
  if (rc < 0) {
#ifdef LOG_SDCARD_TASK
    if (Serial)
      Serial.println("Failed to open file for writing");
#endif
    rv = false;
  } else {
    bool finished = false;

    int toTx = strlen(text);
    int sent = 0;

    while (!finished) {
      int data_size = (toTx - sent > 64) ? SD_WRITE_DIM : (toTx - sent);

      int rc = fs_write(&file, test_data + sent, data_size);
      if (rc < 0) {
#ifdef LOG_SDCARD_TASK
        if (Serial)
          Serial.println("Failed to write to file");
#endif
        finished = true;
        rv = false;
      } else {
        sent += rc;
        if (sent >= toTx) {
          finished = true;
        }
      }
    }
    fs_close(&file);
  }
  return rv;
}

bool read_test(const char *fname, const char *text) {
  bool rv = true;
  struct fs_file_t file;
  char read_buf[SD_READ_DIM];

  fs_file_t_init(&file);
  int rc = fs_open(&file, fname, FS_O_READ);
  if (rc < 0) {
#ifdef LOG_SDCARD_TASK
    if (Serial)
      Serial.println("Failed to open file for reading");
#endif
    rv = false;
  } else {

    int toRx = strlen(text);
    int rx = 0;
    bool finished = false;

#ifdef LOG_SDCARD_TASK
    Serial.print("\n-------- READ FROM FILE ");
    Serial.print(fname);
    Serial.println(" --------");
#endif

    while (!finished) {
      int data_size =
          (toRx - rx > (SD_READ_DIM - 1)) ? (SD_READ_DIM - 1) : (toRx - rx);
      memset(read_buf, 0x00, SD_READ_DIM);
      rc = fs_read(&file, read_buf, data_size);

      if (rc < 0) {
#ifdef LOG_SDCARD_TASK
        if (Serial)
          Serial.println("Failed to read from file");
#endif
        finished = true;
      } else {
#ifdef LOG_SDCARD_TASK
        Serial.println(read_buf);
#endif
        if (strncmp(read_buf, text + rx, rc) != 0) {
#ifdef LOG_SDCARD_TASK
          if (Serial)
            Serial.println("!!!! ERROR: data red are wrong!");
#endif
        }
        rx += rc;
        if (rx >= toRx) {
          finished = true;
        }
      }
    }
    fs_close(&file);
#ifdef LOG_SDCARD_TASK
    Serial.println();
    Serial.println("--------------------------------------------\n");
#endif
  }
  return rv;
}

void sdcard_task() {

  if (Serial) {
    Serial.println("[TASK]: SDCARD");
  }
  static bool tf1 = true;
  static const char *tf = test_file1;

  bool go_on = true;
    if (tf1) {
      tf1 = false;
      tf = test_file1;
    } else {
      tf1 = true;
      tf = test_file2;
    }
    go_on = go_on && card_is_available();
    if (!go_on)
      return;
    go_on = mount_filesystem();
    if (!go_on)
      return;
    go_on = write_test(tf, test_data);
    if (!go_on)
      return;
    go_on = read_test(tf, test_data);

    int rc = fs_unmount(&mp);
    if (rc != 0) {
#ifdef LOG_SDCARD_TASK
      if (Serial)
        Serial.println("Error unmounting SD card");
#endif
    } else {
#ifdef LOG_SDCARD_TASK
      if (Serial)
        Serial.println("SD card unmounted successfully");
#endif
    }
}
