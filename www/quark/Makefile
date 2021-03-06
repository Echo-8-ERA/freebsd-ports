# $FreeBSD$

PORTNAME=	quark
DISTVERSION=	g20180403
CATEGORIES=	www
MASTER_SITES=	https://git.suckless.org/quark/snapshot/
PKGNAMESUFFIX=	-server
DISTNAME=	quark-ba38b0969f542e6e7d3f01ba500189a81ca32355

MAINTAINER=	0mp@FreeBSD.org
COMMENT=	Small and simple HTTP GET/HEAD-only web server for static content

LICENSE=	ISCL
LICENSE_FILE=	${WRKSRC}/LICENSE

CONFLICTS_INSTALL=	quark

MAKE_ARGS=	CC=${CC} MANPREFIX=${PREFIX}/man

PLIST_FILES=	bin/quark \
		man/man1/quark.1.gz

.include <bsd.port.options.mk>

# _WITH_DPRINTF support was removed precisely in r303524.
.if ${OPSYS} == FreeBSD && ${OSVERSION} < 1200002
CFLAGS+=	-D_WITH_DPRINTF
.endif

pre-everything::
	@${ECHO_MSG} "You can build quark with your own config.h using the QUARK_CONF knob:"
	@${ECHO_MSG} "make QUARK_CONF=/path/to/quark/config.h install clean"

post-patch:
.if defined(QUARK_CONF)
	@${ECHO_MSG} "Creating config.h from ${QUARK_CONF}"
	@${LN} -sf ${QUARK_CONF} ${WRKSRC}/config.h
.endif

post-install:
	@${STRIP_CMD} ${STAGEDIR}${PREFIX}/bin/quark

.include <bsd.port.mk>
