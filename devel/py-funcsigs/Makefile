# Created by: Martin Wilke <miwi@FreeBSD.org>
# $FreeBSD$

PORTNAME=	funcsigs
PORTVERSION=	1.0.2
CATEGORIES=	devel python
MASTER_SITES=	CHEESESHOP
PKGNAMEPREFIX=	${PYTHON_PKGNAMEPREFIX}

MAINTAINER=	sunpoet@FreeBSD.org
COMMENT=	Python function signatures from PEP362

LICENSE=	APACHE20
LICENSE_FILE=	${WRKSRC}/LICENSE

USES=		python
USE_PYTHON=	autoplist concurrent distutils

NO_ARCH=	yes

do-test:
	cd ${WRKSRC}/ && ${PYTHON_CMD} ${PYDISTUTILS_SETUP} test

.include <bsd.port.mk>
