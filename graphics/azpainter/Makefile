# Created by: Alexey Dokuchaev <danfe@FreeBSD.org>
# $FreeBSD$

PORTNAME=	azpainter
PORTVERSION=	2.1.4
CATEGORIES=	graphics
MASTER_SITES=	OSDN/${PORTNAME}/71051

MAINTAINER=	danfe@FreeBSD.org
COMMENT=	16-bit RGB full color painting application

LICENSE=	GPLv3

LIB_DEPENDS=	libfontconfig.so:x11-fonts/fontconfig \
		libfreetype.so:print/freetype2 \
		libpng.so:graphics/png

USES=		compiler:c11 desktop-file-utils jpeg localbase \
		shared-mime-info tar:xz
USE_XORG=	x11 xext xi
GNU_CONFIGURE=	yes
CONFIGURE_ARGS=	--with-freetype-dir=${LOCALBASE}/include/freetype2

PORTDOCS=	AUTHORS ChangeLog NEWS README README_ja manual

OPTIONS_DEFINE=	DOCS NLS
OPTIONS_SUB=	yes

post-patch-NLS-off:
	@${REINPLACE_CMD} -e 's, install-dist_trdataDATA$$,,' \
		${WRKSRC}/data/Makefile.in

pre-build-NLS-on:
	cd ${WRKSRC}/translation && ${CC} ${CFLAGS} -o mtrconv mtrconv.c
.for lang in be en ru uk zh_CN
	cd ${WRKSRC}/translation && ./mtrconv ${lang} ../data/tr/${lang}.mtr
.endfor

post-install-DOCS-on:
	@${MKDIR} ${STAGEDIR}${DOCSDIR}
	${CP} -a ${PORTDOCS:S|^|${WRKSRC}/|} ${STAGEDIR}${DOCSDIR}

.include <bsd.port.mk>
