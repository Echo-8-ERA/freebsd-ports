# $FreeBSD$

USE_LIB32=	yes
MASTERDIR=	${.CURDIR}/../openssl

.include "${MASTERDIR}/Makefile"

MACHINE=	i386
.export-env MACHINE

CONFIGURE_ARGS+=no-ec_nistp_64_gcc_128
