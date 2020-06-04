idk if any of these compile on windows, I know you'd need to replace the sleep function (#include Widnows.h instead of unistd.h) at least.
something something make sure you tell your compiler to use ncurses library when its needed (`-l ncurses` for gcc)
