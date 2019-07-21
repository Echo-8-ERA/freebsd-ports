# $FreeBSD$

PORTNAME=	libpciaccess
PORTVERSION=	0.14
CATEGORIES=	devel

MAINTAINER=	x11@FreeBSD.org
COMMENT=	Generic PCI access library

LICENSE=	MIT
LICENSE_FILE=	${WRKSRC}/COPYING

USES=		autoreconf lib32

BUILD_DEPENDS=	${LOCALBASE}/share/pciids/pci.ids:misc/pciids
RUN_DEPENDS=	${LOCALBASE}/share/pciids/pci.ids:misc/pciids

USE_LDCONFIG= yes
CONFIGURE_ARGS=	--with-pciids-path=${LOCALBASE}/share/pciids

XORG_CAT=	lib
INSTALL_TARGET=	install-strip

.include <bsd.port.mk>
