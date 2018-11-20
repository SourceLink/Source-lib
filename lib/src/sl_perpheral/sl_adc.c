#include "so_lib.h"
#include "sl_adc.h"
DEBUG_SET_LEVEL(DEBUG_LEVEL_ERR);

//echo BB-ADC > /sys/devices/bone_capemgr.9/slots
//echo -BB-ADC > /sys/devices/bone_capemgr.9/slots
EXPORT int adc_set(const char *slots_dir, const char *cape_set)
{
	int ret = write_value_to_file(slots_dir, cape_set);
	if (ret > 0)
		return 0;
	else
		return -1;
}

// "/sys/bus/iio/devices/iio:device0/in-voltage%d_raw",pin
EXPORT int get_adc_value(const *fileName )
{
	return read_intvalue_from_file(fileName);
}