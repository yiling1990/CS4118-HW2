#include "types.h"
#include "user.h"
#include "record.h"
#include "fcntl.h"
#include "syscall.h"
#include "hw2-common.h"

int main(int argc, char *argv[]) {
  int ret;
  struct record *records;
  int num_records;
  int pid;
  
  ret = startrecording();
  if (ret == -1)
    handle_error("error startrecording");

  pid = getpid();

  ret = stoprecording();
  if (ret == -1)
    handle_error("error stoprecording");

  sleep(1);

  num_records = fetchrecords(0, 0);
  if (num_records == -1)
    handle_error("error fetching # of records\n");

  printf(stdout,"Num records: %d\n", num_records);
  if (num_records != 2)
    handle_error("# of records is wrong\n");

  records = malloc(sizeof(struct record) * num_records);
  if (!records)
    handle_error("error malloc");

  ret = fetchrecords(records, num_records);
  if (ret == -1)
    handle_error("error fetchrecords");

  check_syscall_no(&records[0], SYS_getpid);
  check_ret_value(&records[1], pid);

  free(records);

  printf(stdout, "hw2-test-pause succeeded\n");

  exit();
}

