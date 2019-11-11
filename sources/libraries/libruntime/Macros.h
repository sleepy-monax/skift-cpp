#pragma once

#define __packed __attribute__((packed))

#define __aligned(__align) __attribute__((aligned(__align)))

#define __unused(__arg) (void)(__arg)