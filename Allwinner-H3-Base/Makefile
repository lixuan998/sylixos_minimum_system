COMPONENTS :=  \
libsylixos \
libcextern \
libVxWorks \
libreadline \
liblua \
libsqlite3 \
pciutils \
libzmodem \
libexpat \
libluaplugin \
libsunrpc \
unfsd \
libsalsa \
libpcap \
openssl \
libffi \
libwayland \
libgtest \
tcpdump

all: $(COMPONENTS)
	@for target in $(COMPONENTS); do make -C $$target all; done

clean: $(COMPONENTS)
	@for target in $(COMPONENTS); do make -C $$target clean; done
