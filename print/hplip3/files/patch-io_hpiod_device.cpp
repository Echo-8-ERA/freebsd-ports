--- io/hpiod/device.cpp.orig	Thu Jun 15 02:02:11 2006
+++ io/hpiod/device.cpp	Sun Jul 16 16:57:30 2006
@@ -25,6 +25,9 @@
 \*****************************************************************************/
 
 #include "hpiod.h"
+#ifdef __FreeBSD__
+#include <dev/usb/usb.h>
+#endif
 
 const unsigned char Venice_Power_On[] = {0x1b, '%','P','u','i','f','p','.',
         'p','o','w','e','r',' ','1',';',
@@ -111,21 +114,240 @@
 
 #elif defined(__FreeBSD__)
 
-/* 
- * Anish Mistry amistry@am-productions.biz is working on libusb extensions for FreeBSD. His current implementation does not handle device exceptions (ie: paperout).
- * Once this issue is resolved we will be glade to add his code to the project.
- */
-
 int Device::Write(int fd, const void *buf, int size)
 {
-   syslog(LOG_ERR, "error Write: unimplemented (freebsd) %s %s %d\n", URI, __FILE__, __LINE__);
-   return -1;
+	int nwrote, total, write_len, r, ep, endpoint_fd;
+	void *tmp_buf = (void *)buf;
+	fd_set ready;
+	char *pEndPoint = NULL;
+
+	if (FD[fd].pHD == NULL)
+	{
+		syslog(LOG_ERR, "invalid Device::Write state: %s %s %d\n", URI,__FILE__, __LINE__);
+		return -1;
+	}
+	
+	if ((ep = GetOutEP(dev, FD[fd].Config, FD[fd].Interface, FD[fd].AltSetting, USB_ENDPOINT_TYPE_BULK)) < 0)
+	{
+		syslog(LOG_ERR, "invalid bulk out endpoint %s %s %d\n", URI, __FILE__, __LINE__);
+		return -1;
+	}
+
+	/* We only want the address, so mask out the direction bits */
+	ep = (ep & USB_ENDPOINT_ADDRESS_MASK);
+	/* open endpoint */
+	/* allocate data for the usb endpoint string */
+	pEndPoint = (char *)malloc(strlen(dev->filename)+20);
+	if(!pEndPoint)
+		return -1;
+	/* create device endpoint name string */
+	sprintf(pEndPoint, "%s.%d", dev->filename, ep);
+
+	/* open the endpoint with read write access */
+	endpoint_fd = open(pEndPoint, O_WRONLY, 0);
+	free(pEndPoint);
+	if(endpoint_fd < 0)
+	{
+		/* we failed to open the endpoint */
+		return -1;
+	}
+
+	total = size;
+	write_len = size;
+
+	while (total > 0)
+	{
+		FD_ZERO(&ready);
+		FD_SET(endpoint_fd, &ready);
+
+		if (!FD_ISSET(endpoint_fd, &ready))
+		{
+			close(endpoint_fd);
+			return -1;
+		}
+		nwrote = write(endpoint_fd, tmp_buf, write_len);
+
+		if (nwrote < 0)
+		{
+			close(endpoint_fd);
+			return -1;
+		}
+
+		write_len -= nwrote;
+		tmp_buf = (void *)((long)tmp_buf + nwrote);
+		total -= nwrote;
+	}
+
+	close(endpoint_fd);
+	return size;
 }
 
 int Device::Read(int fd, void *buf, int size, int usec)
 {
-   syslog(LOG_ERR, "error Read: unimplemented (freebsd) %s %s %d\n", URI, __FILE__, __LINE__);
-   return -2;
+	int nwrote, total, r, ep=0, endpoint_fd, i;
+	struct timeval t1, t2;
+	fd_set ready;
+	char *pEndPoint = NULL;
+	int bytes_want = 0;
+	int rlen = 0;
+	int bytes_read = 0;
+	int mili_timeout = usec/10;
+	unsigned char *pbuf_pos = NULL;
+	int total_usec, tmo_usec = usec;
+
+	if (FD[fd].pHD == NULL)
+	{
+		syslog(LOG_ERR, "invalid Device::Read state: %s %s %d\n", URI,__FILE__, __LINE__);
+		return -1;
+	}
+	
+	if ((ep = GetInEP(dev, FD[fd].Config, FD[fd].Interface, FD[fd].AltSetting, USB_ENDPOINT_TYPE_BULK)) < 0)
+	{
+		syslog(LOG_ERR, "invalid bulk in endpoint %s %s %d\n", URI, __FILE__, __LINE__);
+		return -1;
+	}
+
+	/* We only want the address, so mask out the direction bits */
+	ep = (ep & USB_ENDPOINT_ADDRESS_MASK);
+	/* open endpoint */
+	/* allocate data for the usb endpoint string */
+	pEndPoint = (char *)malloc(strlen(dev->filename)+20);
+	if(!pEndPoint)
+		return -1;
+	/* create device endpoint name string */
+	sprintf(pEndPoint, "%s.%d", dev->filename, ep);
+
+	/* open the endpoint with read write access */
+	endpoint_fd = open(pEndPoint, O_RDONLY, 0);
+	if(endpoint_fd < 0)
+	{
+		/* we failed to open the endpoint */
+		syslog(LOG_ERR, "unable to open bulk in endpoint (%s) %s %s %d\n", pEndPoint, URI, __FILE__, __LINE__);
+		free(pEndPoint);
+		return -1;
+	}
+	free(pEndPoint);
+
+	/* set short transfer so that we can allow "short" reads since we
+	   will don't know exactly
+	what is coming so we can't specify exact byte amounts */
+	i = 1;
+	if (ioctl(endpoint_fd, USB_SET_SHORT_XFER, &i) < 0)
+	{
+		syslog(LOG_ERR, "USB_SET_SHORT_XFER set failed Device::Read : %s %s %d\n", URI,__FILE__, __LINE__);
+	}
+
+	/* 0 timeout value will cause us the wait until the device has data
+           available or is disconnected */
+	if (ioctl(endpoint_fd, USB_SET_TIMEOUT, &mili_timeout) < 0)
+	{
+		syslog(LOG_ERR, "USB_SET_TIMEOUT set failed Device::Read : %s %s %d\n", URI,__FILE__, __LINE__);
+	}
+
+	gettimeofday (&t1, NULL);     /* get start time */
+
+	if (FD[fd].ucnt > 0)
+	{
+		/* copy buffered data to storage */
+		if (size <= FD[fd].ucnt)
+		{
+			memcpy(buf, FD[fd].ubuf, size);
+			FD[fd].uindex += size;
+			FD[fd].ucnt -= size;
+			bytes_read = size;
+		}
+		else
+		{
+			memcpy(buf, FD[fd].ubuf, FD[fd].ucnt);
+			bytes_read += FD[fd].ucnt;
+			FD[fd].ucnt = 0;
+		}
+	}
+	else if (FD[fd].ucnt == 0 && bytes_read < size)
+	{
+		/* reset data buffer */
+		FD[fd].uindex = 0;
+		/* offset storage to not overwrite data copied from the buffer */
+		pbuf_pos = (unsigned char *)((long)buf + bytes_read);
+		bytes_want = size - bytes_read;
+		do
+		{
+			if (bytes_want >= BUFFER_SIZE)
+			{
+				/* check to see if the device is ready for a read */
+				FD_ZERO(&ready);
+				FD_SET(endpoint_fd, &ready);
+				if (!FD_ISSET(endpoint_fd, &ready))
+				{
+					syslog(LOG_ERR, "timeout Device::Read : %s %s %d\n", URI,__FILE__, __LINE__);
+					errno = ETIMEDOUT;
+					return -1;
+				}
+				/* read directly into storage */
+				rlen = read(endpoint_fd, pbuf_pos, BUFFER_SIZE);
+				if (rlen < 0)
+				{
+					syslog(LOG_ERR, "error Device::Read : %s %s %d\n", URI,__FILE__, __LINE__);
+					return -1;
+				}
+				bytes_read += rlen;
+				bytes_want -= rlen;
+				pbuf_pos += rlen;
+			}
+			else if (bytes_want < BUFFER_SIZE)
+			{
+				/* check to see if the device is ready for a read */
+				FD_ZERO(&ready);
+				FD_SET(endpoint_fd, &ready);
+				if (!FD_ISSET(endpoint_fd, &ready))
+				{
+					syslog(LOG_ERR, "timeout Device::Read : %s %s %d\n", URI,__FILE__, __LINE__);
+					errno = ETIMEDOUT;
+					return -1;
+				}
+				/* read to buffer */
+				rlen = read(endpoint_fd, FD[fd].ubuf, BUFFER_SIZE);
+				if (rlen < 0)
+				{
+					syslog(LOG_ERR, "error Device::Read : %s %s %d\n", URI,__FILE__, __LINE__);
+					return -1;
+				}
+				if (rlen >= bytes_want)
+				{
+					memcpy(pbuf_pos, FD[fd].ubuf,
+					bytes_want);
+					FD[fd].uindex += bytes_want;
+					FD[fd].ucnt = rlen - bytes_want;
+					bytes_read += bytes_want;
+					bytes_want = 0;
+				}
+				else
+				{
+					memcpy(pbuf_pos, FD[fd].ubuf, rlen);
+					bytes_want -= rlen;
+					bytes_read += rlen;
+					pbuf_pos += rlen;
+				}
+			}
+			/* Bulk_read has a timeout, but bulk_read can return zero byte packet(s), so we must use our own timeout here. */
+			gettimeofday(&t2, NULL);   /* get current time */
+		
+			total_usec = (t2.tv_sec - t1.tv_sec)*1000000;
+			total_usec += (t2.tv_usec > t1.tv_usec) ? t2.tv_usec - t1.tv_usec : t1.tv_usec - t2.tv_usec;
+			if (total_usec > usec)
+			{
+				size = -1;   /* timeout */
+				errno = ETIMEDOUT;
+				break;
+			}
+			tmo_usec = usec - total_usec;    /* decrease timeout */
+			continue;
+		}
+		while (bytes_want > 0);
+	}
+
+	close(endpoint_fd);
+	return size;
 }
 
 #else
