# ========

MK_ROOT		= $(dir $(realpath $(firstword $(MAKEFILE_LIST))))
MK_NAME		= elfi

# ========

CC		= cc
CFLAGS	= -Wall -Wextra -Werror -std=c99 -ggdb3
DFLAGS	= # ...
LFLAGS  = # ...
IFLAGS  = # ...

# ========

OBJS	= $(SRCS:.c=.o)
SRCS	= $(MK_ROOT)elfi-main.c	\
		  $(MK_ROOT)elfi-x32.c	\
		  $(MK_ROOT)elfi-x64.c	\
		  $(MK_ROOT)elfi-util.c

# =======

TARGET	= $(MK_ROOT)$(MK_NAME)

# =======

.PHONY : all

all : $(TARGET)


.PHONY : clean

clean :
	rm -f $(OBJS)
	rm -f $(TARGET)


.PHONY : install

install : $(TARGET)
	cp -f $^ /usr/local/bin

# =======

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $^


$(OBJS) : %.o : %.c
	$(CC) $(CFLAGS) $(DFLAGS) $(IFLAGS) -o $@ -c $<
