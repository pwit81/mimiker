#define KL_LOG KL_TIME
#include <callout.h>
#include <sched.h>
#include <klog.h>
#include <timer.h>
#include <sysinit.h>

#define SYSTIME_FREQ 1000 /* 1[tick] = 1[ms] */

static systime_t now = 0;
static timer_t *clock = NULL;

systime_t getsystime(void) {
  return now;
}

static void clock_cb(timer_t *tm, void *arg) {
  systime_t prev = now;
  timeval_t cpu = getcputime();
  now = bintime_mul(getbintime(), SYSTIME_FREQ).sec;
  klog("cpu: %d.%06d, now: %d, prev: %d", cpu.tv_sec, cpu.tv_usec, now, prev);
  assert(now >= prev);
  int diff = now - tv2st(cpu);
  assert(abs(diff) <= 1);
  callout_process(now);
  sched_clock();
}

static void clock_init(void) {
  clock = tm_reserve(NULL, TMF_PERIODIC);
  if (clock == NULL)
    panic("Missing suitable timer for maintenance of system clock!");
  tm_init(clock, clock_cb, NULL);
  if (tm_start(clock, TMF_PERIODIC | TMF_TIMESOURCE, (bintime_t){},
               HZ2BT(SYSTIME_FREQ)))
    panic("Failed to start system clock!");
  klog("System clock uses \'%s\' hardware timer.", clock->tm_name);
}

SYSINIT_ADD(clock, clock_init, DEPS("sched", "callout", "pit"));
