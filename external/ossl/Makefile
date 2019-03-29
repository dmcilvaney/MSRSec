TARGET=linux-generic32
PREFIX=/opt/openssl

INCLUDES = -I$(PREFIX)/include

LDFLAGS += -L$(PREFIX)/lib
LDFLAGS += -Wl,-whole-archive
LDFLAGS += -nodefaultlibs
OSSL_CROSS_COMPILE ?= $(TA_CROSS_COMPILE)

CC = $(OSSL_CROSS_COMPILE)gcc \
 -I$(CURDIR)/include \
 -fno-builtin \
 -ffreestanding \
 -nostdinc \
 -mno-unaligned-access \
 -fshort-wchar \
 -DOPENSSL_IMPLEMENTS_strncasecmp \
 -DOPENSSL_NO_SOCK \
 -DNO_SYSLOG \
 -DOPENSSL_NO_DEPRECATED \
 -DOPENSSL_NO_DGRAM \
 -DOPENSSL_NO_UI_CONSOLE \
 -DOPENSSL_NO_SOCK \
 -DOPENSSL_NO_HW \
 -DOPENSSL_NO_STDIO \
 -DNO_CHMOD \
 -DOPENSSL_NO_POSIX_IO \
 -DRAND_DRBG_GET_RANDOM_NONCE \
 -fPIC \
 -fPIE \
 -Os \
 -Werror \
 $(OPENSSL_FLAGS) \
 $(OPENSSL_EXFLAGS) \

OPENSSL_CONFIG = \
--with-rand-seed=getrandom \
no-asm \
no-async \
no-autoalginit \
no-deprecated \
no-engine \
no-posix-io \
no-rdrand \
no-shared \
no-stdio \
no-threads \
 \
no-dso \
no-hw \
no-zlib \
no-pinshared \
-static \
--static \
-g \
-Os \


OPENSSL=openssl
PREFIX=/opt/openssl

all: build

.PHONY: download
download: $(OPENSSL)/README

# If the OSSL readme is not pressent, update the submodule
$(OPENSSL)/README:
	git submodule update --init openssl

.PHONY: configure
configure: download $(OPENSSL)/Makefile

.PHONY: $(OPENSSL)/Makefile
$(OPENSSL)/Makefile:
	( cd $(OPENSSL); ./Configure $(OPENSSL_CONFIG) $(TARGET) --prefix=$(PREFIX) --openssldir=$(PREFIX) )

build: configure
	( cd $(OPENSSL); $(MAKE) CC="$(CC)" build_generated libcrypto.a )

clean:
	( cd $(OPENSSL); $(MAKE) clean; git clean -fdx )

distclean:
	rm -rf $(OPENSSL)
	mkdir $(OPENSSL)
