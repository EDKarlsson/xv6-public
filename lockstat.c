#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"
#include "amd64.h"
#include "lockstat.h"

static void
xwrite(int fd, char c)
{
  if (write(fd, &c, 1) != 1)
    die("lockstat: write failed");
}

static void
stats(void)
{
  static const u64 sz = sizeof(struct lockstat);
  struct lockstat ls;
  int fd;
  int r;

  fd = open("/dev/lockstat", O_RDONLY);
  if (fd < 0)
    die("lockstat: open failed");

  printf(1, "## name cycles acquires\n");
  while (1) {
    r = read(fd, &ls, sz);
    if (r < 0)
      die("lockstat: read failed");
    if (r == 0)
      break;
    if (r != sz)
      die("lockstat: unexpected read");

    u64 cycles = 0, acquires = 0;
    
    for (int i = 0; i < NCPU; i++) {
      cycles += ls.cpu[i].cycles;
      acquires += ls.cpu[i].acquires;
    }
    printf(1, "%s %lu %lu\n", ls.name, cycles, acquires);
  }
}

int
main(int ac, char *av[])
{
  int fd = open("/dev/lockstat", O_RDWR);
  if (fd < 0)
    die("lockstat: open failed");
  xwrite(fd, '2');
  xwrite(fd, '3');

  int pid = fork(0);
  if (pid < 0)
    die("lockstat: fork failed");

  if (pid == 0) {
    xwrite(fd, '1');
    exec(av[1], av+1);
    die("lockstat: exec failed");
  }
  
  wait();
  xwrite(fd, '2');
  stats();
  xwrite(fd, '3');
  exit();
}