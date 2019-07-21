# Created by: Johannes Lundberg <johalun0@gmail.com>
# $FreeBSD$

PORTNAME=	libepoll-shim
PORTVERSION=	0.0.20190311
CATEGORIES=	devel

MAINTAINER=	x11@FreeBSD.org
COMMENT=	epoll shim implemented using kevent

LICENSE=	MIT

USE_GITHUB=	yes
GH_ACCOUNT=	FreeBSDDesktop
GH_PROJECT=	epoll-shim
GH_TAGNAME=	1d1135384395dbf38eea00680ca28ead04f8dce2

USE_LDCONFIG=	yes

USES=		compiler lib32 uidfix
MAKE_ARGS=	WITHOUT_PROFILE=yes

.include <bsd.port.pre.mk>

post-patch:
	@${REINPLACE_CMD} -e 's,/usr/local,${PREFIX},' ${WRKSRC}/Makefile
	@${REINPLACE_CMD} -e 's,/usr/local/lib$$,${LIBDIR},' ${WRKSRC}/Makefile
.if "${CHOSEN_COMPILER_TYPE}" == "gcc"
	@${REINPLACE_CMD} -e 's|Wno-missing-variable-declarations|Wno-missing-declarations|' \
		-e 's|-Wno-thread-safety-analysis||' ${WRKSRC}/Makefile
.endif
.if defined(USE_LIB32)
	@${REINPLACE_CMD} -e 's,libdir=$${exec_prefix}/lib,libdir=$${exec_prefix}/lib32,' ${WRKSRC}/src/epoll-shim.pc.in
.endif

pre-install:
	@${MKDIR} ${STAGEDIR}/${PREFIX}/include/libepoll-shim/sys
.if defined(USE_LIB32)
	@${MKDIR} ${STAGEDIR}/${PREFIX}/lib32
.endif

.include <bsd.port.post.mk>
