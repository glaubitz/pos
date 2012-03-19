/*
 * pOS
 * device.c - device api
 * 01/15/05
 */

#include <kernel.h>

/* TODO:
 * - drvstartstop, drvinit, drvexit need update (major, minor as parameter)
 * - drvstartstop cannot output messages as the console-driver has not
 *   init'ed
 */

static int k_device_startstop (struct s_device * device, int start) {

	unsigned char i;

	if (!device) {

		if (!k_kernel.kernelmode) {
			k_setlasterror (c_ERRACCESSVIOLATION);
			return FALSE;
		}

		(start ? printf ("INITIALIZING DEVICES....") : printf ("EXITING DEVICES...."));

		for (i = 0; i < c_MAXNDEVICE; i++) {

			printf (k_kernel.device [i].devname); // MUST NOT PRINT RETURN

			printf ("...."); // MUST NOT PRINT RETURN

			if (start ? k_kernel.device [i].init () : k_kernel.device [i].exit ())
				printf ("[OK]");
			else
				printf ("[FAILED]");
		}
	}

	else {

		for (i = 0; i < c_MAXNDEVICE; i++) {

			if (k_kernel.device [i].devname == device->devname) // WARNING: devname is string, compare will fail
				break;					    // --> use ids to identify devices
									    // instead of strings
		}

		if (i == c_MAXNDEVICE) {
			k_setlasterror (c_ERRUNKNOWNDEVICE);
			return 0;
		}

		if (device->owner == k_kernel.current->owner->pid)
			return (start ? k_kernel.device [i].init () : k_kernel.device [i].exit ()); // WARNING: init(), exit() return unsigned long
		else {

			k_setlasterror (c_ERRACCESSVIOLATION);
			return 0;
		}
	}
}

int k_device_init (struct s_device * device) {

        return k_device_startstop (device, TRUE);
}

int k_device_exit (struct s_device * device) {

        return k_device_startstop (device, FALSE);
}

struct s_device k_device_get (unsigned char major, unsigned char minor) {

	struct s_device device;
	
	if (major > c_MAXNDEVICE)
		return 0;

	if (minor > k_kernel.driverlist [major].highestminor)
		return 0;

	device.driver = &k_kernel.driverlist [major];
	device.minor = minor;

	return device;
}

static unsigned long k_device_readwritedevice (struct s_device * device, unsigned char * buffer,
				unsigned long buffersize, unsigned long offset, int read) {

	if (read) {
		
		if (device->driver->type == c_DEVICECHAR)
			return device->driver->read.chardev (device->minor, buffer, buffersize);

		if (device->driver->type == c_DEVICEBLOCK)
			return device->driver->read.blockdev (device->minor, buffer, buffersize, offset);
	
		return 0;
	}
	else {

		if (device->driver->type == c_DEVICECHAR)
			return device->driver->write.chardev (device->minor, buffer, buffersize);

		if (device->driver->type == c_DEVICEBLOCK)
			return device->driver->write.blockdev (device->minor, buffer, buffersize, offset);
	
		return 0;
	}
}

unsigned long k_device_read (struct s_device * device, unsigned char * buffer,
				unsigned long buffersize, unsigned long offset) {

	return k_device_readwritedevice (device, buffer, buffersize, offset, TRUE);
}

unsigned long k_device_write (struct s_device * device, unsigned char * buffer,
				unsigned long buffersize, unsigned long offset) {

	return k_device_readwritedevice (device, buffer, buffersize, offset, FALSE);
}
