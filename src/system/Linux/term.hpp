#pragma once

#include <stdarg.h>
#include <stdint.h>
#include <string.h>

#include "ms.h"

namespace System {
class Term {
 public:
  template <typename Arg>
  class Command {
   public:
    Command(Arg arg, void (*fun)(Arg, int, char *[]), const char *name,
            ms_item_t *dir)
        : arg_(arg), fun_(fun) {
      ms_file_init(&this->cmd_, name, this->Helper, NULL, NULL);
      if (dir) {
        ms_item_add(&this->cmd_, dir);
      } else {
        ms_cmd_add(&this->cmd_);
      }
    }

    static int Helper(ms_item_t *item, int argc, char *argv[]) {
      Command<Arg> *cmd = ms_container_of(item, Command<Arg>, cmd_);
      cmd->fun_(cmd->arg_, argc, argv);

      return 0;
    }

   private:
    ms_item_t cmd_;
    Arg arg_;
    void (*fun_)(Arg, int, char *[]);
  };

  Term();

  static bool Opened() { return true; }

  static uint32_t Available() { return true; }

  static char ReadChar() { return getchar(); }

  static ms_item_t *BinDir() { return ms_get_bin_dir(); }

  static ms_item_t *EtcDir() { return ms_get_etc_dir(); }

  static ms_item_t *DevDir() { return ms_get_dev_dir(); }
};
}  // namespace System
